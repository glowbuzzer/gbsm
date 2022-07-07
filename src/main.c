/**
 ******************************************************************************
 * @file           :  main.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "log.h"
#include "gberror.h"
#include "user_message.h"
#include "std_utils.h"
#include <stdio.h>
#include "linux_shm.h"
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "gbsm_config.h"
#include "dpm.h"
#include "stateMachine.h"
#include "step_control.h"
#include "version.h"
#include "cyclic_message.h"
#include "drives.h"
#include "read_register.h"
#include "command_line.h"
#include "stepper_routines.h"

int log_run_level = LOG_LEVEL;


#define GBC_PROCESS_NAME_MAX_LENGTH                     100
char proc_name[GBC_PROCESS_NAME_MAX_LENGTH] = {0};

/** global for pid of gbc*/
int gbc_pid = 0;

/** struct shared with gbc shared memory */
struct shm_msg *shmp;

pthread_t thread_step_cyclic;

/** dpm storage - overlaid with dpm structs*/
uint8_t inA[SIZE_OF_GBC_PDO];
uint8_t inB[SIZE_OF_GBC_PDO];
uint8_t outA[SIZE_OF_GBC_PDO];
uint8_t outB[SIZE_OF_GBC_PDO];

bool gbc_connected = false;

#define NUM_CYCLIC_EVENTS 5
typedef enum {
    CYCLIC_EVENT_OVERRUN, CYCLIC_EVENT_TIMEWARN, CYCLIC_EVENT_SEND_FAIL, CYCLIC_EVENT_NOT_OPMODE, CYCLIC_EVENT_GBC_NOT_CONNECTED
} cyclic_event_type_t;
//@formatter:off
cyclic_event_t ec_cyclic_event[NUM_CYCLIC_EVENTS] = {
        [CYCLIC_EVENT_OVERRUN] = {.message="The execution time has overrun!", .type=CYCLIC_MSG_ERROR},
        [CYCLIC_EVENT_TIMEWARN] = {.message="The execution time is over safe threshold", .type=CYCLIC_MSG_WARN},
        [CYCLIC_EVENT_SEND_FAIL] = {.message="Send process data over EtherCAT failed", .type=CYCLIC_MSG_ERROR},
        [CYCLIC_EVENT_NOT_OPMODE] = {.message="OPMODE is not set yet", .type=CYCLIC_MSG_INFO},
        [CYCLIC_EVENT_GBC_NOT_CONNECTED] = {.message="GBC is not connected", .type=CYCLIC_MSG_INFO},
};

int osal_thread_create_rt(void *thandle, int stacksize, void *func, void *param) {
    int ret;
    pthread_attr_t attr;
    struct sched_param schparam;
    pthread_t *threadp;

    threadp = thandle;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, stacksize);
    ret = pthread_create(threadp, &attr, func, param);
    pthread_attr_destroy(&attr);
    if (ret < 0) {
        return 0;
    }
    memset(&schparam, 0, sizeof(schparam));
    schparam.sched_priority = 50;
    //    schparam.sched_priority = 30;

    //VIP change the kernel scheduler scheme here
    ret = pthread_setschedparam(*threadp, SCHED_FIFO, &schparam);
    if (ret < 0) {
        return 0;
    }

    return 1;
}

#define NSEC_PER_SEC 1000000000

struct period_info {
    struct timespec next_period;
    long period_ns;
};

static void inc_period(struct period_info *pinfo)
{
    pinfo->next_period.tv_nsec += pinfo->period_ns;

    while (pinfo->next_period.tv_nsec >= 1000000000) {
        /* timespec nsec overflow */
        pinfo->next_period.tv_sec++;
        pinfo->next_period.tv_nsec -= 1000000000;
    }
}

static void periodic_task_init(struct period_info *pinfo)
{
    pinfo->period_ns = BUS_CYCLE_TIME*1000000;
    clock_gettime(CLOCK_MONOTONIC, &(pinfo->next_period));
}

struct timespec timespec_diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

uint64_t exec_time_usec;

static void wait_rest_of_period(struct period_info *pinfo)
{
    inc_period(pinfo);
    struct timespec remainder;
    struct timespec current;
    struct timespec diff;
    clock_gettime(CLOCK_MONOTONIC, &current);

    diff = timespec_diff(current, pinfo->next_period);
    exec_time_usec = (uint64_t)((pinfo->period_ns - diff.tv_nsec)/1000);
//    printf("%lld\n", exec_time_usec);

//    printf("%lld.%.9ld", (long long)ts.tv_sec, ts.tv_nsec);
    /* for simplicity, ignoring possibilities of signal wakes */
    if(clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &pinfo->next_period, &remainder) == -1){

        UM_FATAL("GBSM: nanosleep was interrupted");
//        if (errno == EINTR) {
//            printf("nanosleep interrupted\n");
//            printf("Remaining secs: %d\n", remainder.tv_sec);
//            printf("Remaining nsecs: %d\n", remainder.tv_nsec);
//        }
//        else perror("nanosleep");
    }

}

void step_cyclic(void *argument) {
    (void)argument;

    uint64_t bus_cycle_tick = 0;
    gberror_t grc;
    bool first_run = true;
    static uint64_t ms_tick = 0;
    bool print_i_am_alive_message = false;
    bool print_repeater_message = false;
    bool time_to_check_gbc = false;
    /* Allocated storage for statemachine struct and initialise */
    struct stateMachine *m;
    m = step_ctrl_statemachine_alloc();
    step_ctrl_init_statemachine(m);
    struct period_info pinfo;
    periodic_task_init(&pinfo);
    int kill_rc = 0;

    while (1) {

        bool reminder_message_flag = false;
        bool check_gbc_flag = false;
        bool gbem_i_am_alive_flag = false;

        bus_cycle_tick++;
        for (int i = 0; i < BUS_CYCLE_TIME; i++) {
            ms_tick++;
            if ((ms_tick) % GBSM_REMINDER_MESSAGE_INTERVAL_MS == 0) {
                reminder_message_flag = true;
            }
            if ((ms_tick) % GBSM_GBC_CONNECTION_CHECK_INTERVAL_MS == 0) {
                check_gbc_flag = true;
            }
            if ((ms_tick) % GBSM_I_AM_ALIVE_MESSAGE_INTERVAL_MS == 0) {
                gbem_i_am_alive_flag = true;
            }
        }

        print_i_am_alive_message = gbem_i_am_alive_flag ? true : false;
        print_repeater_message = reminder_message_flag ? true : false;
        time_to_check_gbc = check_gbc_flag ? true : false;

        if (gbc_connected) {
            /* do the memcpy to shared mem */
            memcpy(outA, shmp->sm_buf_out, SIZE_OF_GBC_PDO);
            memcpy(shmp->sm_buf_in, inA, SIZE_OF_GBC_PDO);
            /* send the signal to GBC to do the shared mem memcpy */
            kill_rc = kill((pid_t) gbc_pid, SIGNAL_TO_SEND);
        }
        if (kill_rc != 0 && gbc_connected)  {
            gbc_connected = false;
            if (errno == ESRCH) {
                UM_ERROR(GBSM_UM_EN,
                         "GBEM: Signal sending failure. The GBC process does not exist. This could be because the GBC process has been terminated");
            } else if (errno == EPERM) {
                UM_FATAL (
                        "GBSM: Signal sending failure. The GBEM process does not have permission to send signal to recipient. This error is irrecoverable. GBEM will exit");
            } else {
                UM_FATAL ("GBSM: Unknown error sending signal. This error is irrecoverable. GBEM will exit");
            }
        }

        if (!gbc_connected && time_to_check_gbc) {
            grc = establish_shared_mem_and_signal_con(&shmp, proc_name, false, &gbc_pid,  1);
            if (grc == E_SUCCESS) {
                UM_INFO(GBSM_UM_EN, "GBSM: Connection to shared memory and GBC process >successfully< established ");
                memset(shmp->sm_buf_in, 0, sizeof (uint8_t) * SHM_BUF_SIZE);
                memset(shmp->sm_buf_out, 0, sizeof (uint8_t) * SHM_BUF_SIZE);
                gbc_connected = true;
            } else {
                UM_INFO(GBSM_UM_EN, "GBSM: Connection to shared memory and/or GBC process >failed<");
            }
        }

        if (gbc_connected){
            ec_cyclic_event[CYCLIC_EVENT_GBC_NOT_CONNECTED].active = false;


            if (first_run) {
                step_ctrl_main(m, first_run);
                first_run = false;
            } else {
                step_ctrl_main(m, first_run);
            }
        }else{
            ec_cyclic_event[CYCLIC_EVENT_GBC_NOT_CONNECTED].active = true;
        }

        if ((uint32_t) exec_time_usec >
            (uint32_t) (BUS_CYCLE_TIME * 1000 * CYCLIC_EXEC_TIME_ERROR_PERCENTAGE / 100)) {
            ec_cyclic_event[CYCLIC_EVENT_OVERRUN].active = true;
//                            printf("warn: %u\n", (uint32_t)exec_time_usec);

        } else if ((uint32_t) exec_time_usec >
                   (uint32_t) (BUS_CYCLE_TIME * 1000 * CYCLIC_EXEC_TIME_WARNING_PERCENTAGE / 100)) {
            ec_cyclic_event[CYCLIC_EVENT_TIMEWARN].active = true;
//                            printf("error: %u\n", (uint32_t) exec_time_usec);
        } else {
            ec_cyclic_event[CYCLIC_EVENT_OVERRUN].active = false;
            ec_cyclic_event[CYCLIC_EVENT_TIMEWARN].active = false;
        }

        if (ms_tick > GBSM_DELAY_TO_START_MESSAGES_SEC * 1000) {
            print_cyclic_user_message(NUM_CYCLIC_EVENTS, ec_cyclic_event, print_repeater_message);
        }
        if (print_i_am_alive_message) {
            UM_INFO(GBSM_UM_EN, "GBSM: Is running. Current bus cycle count is [%llu]", bus_cycle_tick);
        }
//        #if FLAVOUR == PLATFORM_PI
//    drv_send_start_trigger();
//#endif

        wait_rest_of_period(&pinfo);
    }
}


int main(int argc, char *argv[]) {

    gberror_t grc = E_GENERAL_FAILURE;
    strcpy(proc_name, GBC_PROCESS_NAME);


    /* These set where the user message output will be sent. Uncomment the one you want.
     * This is just for user messages UM_* not logging LL_ which, if enabled, always goes to stdout
    */
    logger_set_stdout();
    //    logger_set_log_file("gbem.log", GBSM_UM_EN);
    //    logger_set_syslog("Glowbuzzer");

    //solves missing output in debugger log output (a gdb thing)
    setbuf(stdout, 0);


    UM_INFO(GBSM_UM_EN, "GBSM: **************************************************************************");
    UM_INFO(GBSM_UM_EN, "GBSM: ***                  Starting GB Stepper Controller                    ***");
    UM_INFO(GBSM_UM_EN, "GBSM: **************************************************************************");

    UM_INFO(GBSM_UM_EN, "GBSM: Software project name [%s]", PROJECT_NAME);
    UM_INFO(GBSM_UM_EN, "GBSM: Software project version [%s]", PROJECT_VER);
    UM_INFO(GBSM_UM_EN, "GBSM: Release Git Tag [%s]", GIT_TAG);

    // process any command line arguments supplied to GBC
    process_command_line(argc, argv, &command_line_vars);

    switch (command_line_vars.program){
        case GBSM_PROG_CYCLIC:
            break;
        case GBSM_PROG_TEST_MOVE:
            test_motors_oscillating_move();
            exit(0);
        case GBSM_PROG_INTERACTIVE_REG_READ:
            interactive_reg_print();
            exit(0);
        case GBSM_PROG_HYST_TUNE:
            calibrate_spread_cycle();
            exit(0);
        case GBSM_PROG_NONE:
            LL_FATAL("GBSM: Program has not been set");
            break;
        default:
            LL_FATAL("GBSM: Program has not been set to an invalid value");
    }


    if (SIGNAL_TO_SEND > 31) {
        UM_FATAL(
                "GBSM: We have a signal number defined (with SIGNAL_TO_SEND) to a number greater than 31. This is outside the range of normal Linux signals. We must exit");
    }
    char *str = strdup(sys_siglist[SIGNAL_TO_SEND]);
    if (str) {
        upcase(str);
        UM_INFO(GBSM_UM_EN,
                "GBSM: We are using Linux signal [SIG %s] (we are sending this out to GBC to advance its cycle)", str);
        free(str);
    } else {
        UM_ERROR(GBSM_UM_EN, "GBSM: Error matching the signal number [%u], to the standard Linux signals",
                 SIGNAL_TO_SEND);
    }

    char *username_buf;
    username_buf = (char *) malloc(10 * sizeof(char));
    if (getlogin_r(username_buf, 10) != 0) {
        strncpy(username_buf, "<unknown>", 10);
    }
    UM_INFO(GBSM_UM_EN, "GBSM: We are running as user [%s]", username_buf);

    grc = establish_shared_mem_and_signal_con(&shmp, proc_name, true, &gbc_pid, 1);

    if (grc != E_SUCCESS) {
        gbc_connected = false;
        UM_ERROR(GBSM_UM_EN,
                 "GBSM: Connection to shared memory & GBC process could not be established - we will continue without a connection to GBC");
    } else {
        gbc_connected = true;
        UM_INFO(GBSM_UM_EN, "GBSM: We have a connection to shared memory & GBC process >successfully< established ");
        LL_INFO(GBSM_GEN_LOG_EN, "GBSM: Shared memory address [%p] (this is a virtual address so will not match across processes)", shmp);
        memset(shmp->sm_buf_in, 0, sizeof (uint8_t) * SHM_BUF_SIZE);
        memset(shmp->sm_buf_out, 0, sizeof (uint8_t) * SHM_BUF_SIZE);
    }

#if FLAVOUR == PLATFORM_PI
    grc = drv_init_spi_and_drivers();
    if (grc !=E_SUCCESS){
        UM_FATAL( "GBSM: The SPI / Driver initialisation failed. This means we can't talk to the motor drivers.");

    }
//confirm the driver ICs are ok by reading their version numbers
    grc = drv_check_and_enable();
    if (grc !=E_SUCCESS){
        UM_FATAL( "GBSM: The check of the status of the SPI / Drivers initialisation failed. This means we can't talk to the motor drivers");

    }

#define GBSM_TEST_MOTORS 0

#if GBSM_TEST_MOTORS == 1
calibrate_spread_cycle();
    //    test_motors_oscillating_move();
//    interactive_reg_read();
#endif

#endif

    #if FLAVOUR != PLATFORM_PI
    //set enabled - a frig for linux
    drv_set_enabled();
    #endif

    //spin up RT loop

    int rc = osal_thread_create_rt(&thread_step_cyclic, STACK64K * 2, &step_cyclic, (void *) proc_name);
    if (rc != 1) {
        UM_FATAL(
                "GBSM: An error occurred whilst creating the pthread ????? will exit. This error message implies that a Linux system call (pthread_create) has failed. This could be because the system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process would be exceeded. Neither of these should occur normally. Something bad has happened deep down");
    }
    //join the main to the thread to stop it exiting until the thread has finished - pthread jiggerypokery
    pthread_join(thread_step_cyclic, NULL);
}
