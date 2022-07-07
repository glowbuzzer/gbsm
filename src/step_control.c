/**
 ******************************************************************************
 * @file           : step_control.c
 * @brief          : main control functions - runs drive state machine
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "std_headers.h"
#include "std_defs_and_macros.h"
#include "stateMachine.h"
#include "log.h"
#include "dpm.h"
#include "fault_cause.h"
#include "step_control.h"
#include "user_message.h"
#include "cyclic_message.h"
#include "cia402.h"
#include "gbsm_config.h"
#include "drives.h"
#include "io.h"

/* Types of sm events */
enum eventType {
    Event_cyclic,
};

step_event_data_t step_event_data = {};
uint32_t gbsm_heartbeat = 0;
bool estop = true;
cia_state_t current_state = CIA_NOT_READY_TO_SWITCH_ON;


bool homing_failed = false;

#define NUM_CONTROL_EVENTS 16
typedef enum {
    CONTROL_EVENT_ESTOP,
    CONTROL_EVENT_DRIVE_FAULT,
    CONTROL_EVENT_GBC_FAULT_REQUEST,
    CONTROL_EVENT_GBC_MOVE_NOT_OP_END_REQUEST,
    CONTROL_EVENT_GBC_INTERNAL_FAULT,
    CONTROL_EVENT_HEARTBEAT_LOST,
    CONTROL_EVENT_LIMIT_REACHED,
    CONTROL_EVENT_DRIVE_STATE_CHANGE_TIMEOUT,
    CONTROL_EVENT_DRIVE_FOLLOW_ERROR,
    CONTROL_EVENT_DRIVE_NO_REMOTE,
    CONTROL_EVENT_ECAT_ERROR,
    CONTROL_EVENT_DRIVE_ALARM,
//    CONTROL_EVENT_GBC_TO_GBEM_CON_ERROR,
    CONTROL_EVENT_DRIVE_MOOERROR,
    CONTROL_EVENT_ECAT_SLAVE_ERROR,
    CONTROL_EVENT_PLC_SIGNALLED_ERROR,
    CONTROL_EVENT_HOMING_ERROR
} control_event_type_t;

//@formatter:off
cyclic_event_t control_event[NUM_CONTROL_EVENTS] = {
        [CONTROL_EVENT_ESTOP] = {.message="An error has been detected [ESTOP event]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_FAULT] = {.message="An error has been detected [Drive fault]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_GBC_FAULT_REQUEST] = {.message="An error has been detected [GBC requesting fault state]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_GBC_MOVE_NOT_OP_END_REQUEST] = {.message="An error has been detected [Move when not in op en]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_GBC_INTERNAL_FAULT] = {.message="An error has been detected [GBC internal fault]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_HEARTBEAT_LOST] = {.message="An error has been detected [Heartbeat to GBC lost]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_LIMIT_REACHED] = {.message="An error has been detected [Drive reached limit]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_STATE_CHANGE_TIMEOUT] = {.message="An error has been detected [Drive state change timeout]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_FOLLOW_ERROR] = {.message="An error has been detected [Drive follow error too large]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_NO_REMOTE] = {.message="An error has been detected [Drive lost remote]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_ECAT_ERROR] = {.message="An error has been detected [EtherCAT network]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_ALARM] = {.message="An error has been detected [Drive alarm]", .type=CYCLIC_MSG_ERROR},
//gbc_to_plc con error is not used on GBEM it signals an error when gbc loses contact with GBEM
//        [CONTROL_EVENT_GBC_TO_GBEM_CON_ERROR] = {.message="An error has been detected [GBC-to-GBEM connection error]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_DRIVE_MOOERROR] = {.message="An error has been detected [modes of operation]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_ECAT_SLAVE_ERROR] = {.message="An error has been detected [EtherCAT slave]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_PLC_SIGNALLED_ERROR] = {.message="An error has been detected [PLC signalled an error]", .type=CYCLIC_MSG_ERROR},
        [CONTROL_EVENT_HOMING_ERROR] = {.message="\"An error has been detected [Homing error]", .type=CYCLIC_MSG_ERROR},
};

//@formatter:on



/**global variable to check how long drives take to response to state change request */
uint32_t step_ctrl_state_change_cycle_count = 1;
uint32_t step_ctrl_state_change_timeout = CTRL_DRIVE_CHANGE_STATE_TIMEOUT;

/* functions to copy between EC and DPM */
static void step_ctrl_copy_actpos(void);
static void step_ctrl_copy_setpos(void);
static void step_ctrl_copy_drive_statuswords(void);



/* sm guard functions */
static bool cia_trn13_guard(void *condition, struct event *event);

static bool cia_trn14_guard(void *condition, struct event *event);

static bool cia_trn2_guard(void *condition, struct event *event);

static bool cia_trn3_guard(void *condition, struct event *event);

static bool cia_trn4_guard(void *condition, struct event *event);

static bool cia_trn5_guard(void *condition, struct event *event);

static bool cia_trn6_guard(void *condition, struct event *event);

static bool cia_trn7_guard(void *condition, struct event *event);

static bool cia_trn8_guard(void *condition, struct event *event);

static bool cia_trn9_guard(void *condition, struct event *event);

static bool cia_trn10_guard(void *condition, struct event *event);

static bool cia_trn11_guard(void *condition, struct event *event);

static bool cia_trn12_guard(void *condition, struct event *event);

static bool cia_trn15_guard(void *condition, struct event *event);

static bool cia_trn16_guard(void *condition, struct event *event);

/* sm entry action functions */
static void cia_generic_entry_action(void *stateData, struct event *event);

static void sm_error_state_entry_action(void *stateData, struct event *event);

/* sm transition action functions */
static void cia_set_current_fault_causes_action(void *oldStateData, struct event *event, void *newStateData);

/* Forward declaration of sm states so that they can be defined in an logical order: */
static struct state cia_not_ready_to_switch_on_state, cia_switch_on_disabled_state, cia_ready_to_switch_on_state, cia_switched_on_state, cia_operation_enabled_state, cia_quick_stop_active_state,
        cia_fault_reaction_active_state, cia_fault_state, sm_error_state;

static struct state cia_not_ready_to_switch_on_state = {
        .parentState = NULL,
        .entryState = NULL,
        .transitions = (struct transition[])
                {
                        {
                                Event_cyclic,
                                (void *) ((intptr_t) CIA_NOT_READY_TO_SWITCH_ON),
                                &cia_trn13_guard,
                                NULL,
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                NULL,
                                NULL,
                                &cia_switch_on_disabled_state},},
        .numTransitions = 2,
        .data = (void *) CIA_NOT_READY_TO_SWITCH_ON,
        .entryAction = &cia_generic_entry_action,
        .exitAction =
        NULL,};

static struct state cia_switch_on_disabled_state = {
        .parentState = NULL,
        .entryState = NULL,
        .transitions = (struct transition[])
                {
                        {
                                Event_cyclic,
                                (void *) ((intptr_t) CIA_SWITCH_ON_DISABLED),
                                &cia_trn13_guard,
                                NULL,
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn2_guard,
                                NULL,
                                &cia_ready_to_switch_on_state},},
        .numTransitions = 2,
        .data =  (void *) CIA_SWITCH_ON_DISABLED,
        .entryAction = &cia_generic_entry_action,
        .exitAction = NULL,};

static struct state cia_ready_to_switch_on_state = {
        .parentState = NULL,
        .entryState = NULL,
        .transitions = (struct transition[])
                {
                        {
                                Event_cyclic,
                                (void *) ((intptr_t) CIA_READY_TO_SWITCH_ON),
                                &cia_trn13_guard,
                                NULL,
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn3_guard,
                                NULL,
                                &cia_switched_on_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn7_guard,
                                NULL,
                                &cia_switch_on_disabled_state},},
        .numTransitions = 3,
        .data =  (void *) CIA_READY_TO_SWITCH_ON,
        .entryAction = &cia_generic_entry_action,
        .exitAction = NULL,};

static struct state cia_switched_on_state = {
        .parentState = NULL,
        .entryState = NULL,
        .transitions = (struct transition[])
                {
                        {
                                Event_cyclic,
                                (void *) ((intptr_t) CIA_SWITCHED_ON),
                                &cia_trn13_guard,
                                NULL,
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn6_guard,
                                NULL,
                                &cia_ready_to_switch_on_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn4_guard,
                                NULL,
                                &cia_operation_enabled_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn10_guard,
                                NULL,
                                &cia_switch_on_disabled_state},},
        .numTransitions = 4,
        .data =  (void *) CIA_SWITCHED_ON,
        .entryAction = &cia_generic_entry_action,
        .exitAction = NULL,};

static struct state cia_operation_enabled_state = {
        .parentState = NULL,
        .entryState = NULL,
        .transitions = (struct transition[])
                {
                        {
                                Event_cyclic,
                                (void *) ((intptr_t) CIA_OPERATION_ENABLED),
                                &cia_trn13_guard,
                                NULL,
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn5_guard,
                                NULL,
                                &cia_switched_on_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn8_guard,
                                NULL,
                                &cia_ready_to_switch_on_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn9_guard,
                                NULL,
                                &cia_switch_on_disabled_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn11_guard,
                                NULL,
                                &cia_quick_stop_active_state},},
        .numTransitions = 5,
        .data =  (void *) CIA_OPERATION_ENABLED,
        .entryAction = &cia_generic_entry_action,
        .exitAction = NULL,};

static struct state cia_quick_stop_active_state = {
        .parentState = NULL,
        .entryState = NULL,
        .transitions = (struct transition[])
                {
                        {
                                Event_cyclic,
                                (void *) ((intptr_t) CIA_QUICK_STOP_ACTIVE),
                                &cia_trn13_guard,
                                NULL,
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn16_guard,
                                NULL,
                                &cia_operation_enabled_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn12_guard,
                                NULL,
                                &cia_switch_on_disabled_state},},
        .numTransitions = 3,
        .data = (void *) CIA_QUICK_STOP_ACTIVE,
        .entryAction = &cia_generic_entry_action,
        .exitAction = NULL,};

//this state has a unguarded transition to itself with a transition action to set the current fault cause bits
//when a state transitions to itself, entry and exit actions are not called
static struct state cia_fault_reaction_active_state = {
        .parentState = NULL,
        .entryState = NULL,
        .transitions = (struct transition[])
                {
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn14_guard,
                                NULL,
                                &cia_fault_state},
                        {
                                Event_cyclic,
                                NULL,
                                NULL,
                                &cia_set_current_fault_causes_action,
                                &cia_fault_reaction_active_state},},
        .numTransitions = 2,
        .data =  (void *) CIA_FAULT_REACTION_ACTIVE,
        .entryAction = &cia_generic_entry_action,
        .exitAction = NULL,};

static struct state cia_fault_state = {
        .parentState = NULL,
        .entryState = NULL,
        .transitions = (struct transition[])
                {
                        {
                                Event_cyclic,
                                (void *) ((intptr_t) CIA_FAULT),
                                &cia_trn13_guard,
                                NULL,
                                &cia_fault_reaction_active_state},
                        {
                                Event_cyclic,
                                NULL,
                                &cia_trn15_guard,
                                NULL,
                                &cia_switch_on_disabled_state},},
        .numTransitions = 2,
        .data =  (void *) CIA_FAULT,
        .entryAction = &cia_generic_entry_action,
        .exitAction =
        NULL,};

static struct state errorState = {
        .entryAction = &sm_error_state_entry_action};




/**
 * @brief Guards transition from Switch on disabled to Ready to switch on - expected machine controlword: SHUTDOWN. Commands drives with CIA_SHUTDOWN_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn2_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter
    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN2 Guard (Switch on disabled to Ready to switch on)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_SHUTDOWN) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN2 Guard - machine command matches Shutdown");
        if (step_ctrl_check_all_drives_state(CIA_READY_TO_SWITCH_ON)) {
            LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN2 Guard - All drives are ready to switch on - return true");
            return true;
        } else {
            step_ctrl_change_all_drives_states(CIA_SHUTDOWN_CTRLWRD);
            LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN2 Guard - changing drive states with a shutdown controlword");
            return false;
        }
    }
    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN2 Guard - machine command is not Shutdown");
    return false;
}

/**
 * @brief Guards transition from Ready to switch on to Switched on - expected machine controlword: SWITCH_ON or SWITCH_ON_AND_ENABLE_OPERATION. Commands drives with CIA_SWITCH_ON_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn3_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN3 Guard (Ready to switch on to Switched on)");
    if ((cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_SWITCH_ON)) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN3 Guard - machine command matches Switch on");
        if (step_ctrl_check_all_drives_state(CIA_SWITCHED_ON)) {
            LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN3 Guard - All drives are switched on - return true");
            step_ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            step_ctrl_change_all_drives_states(CIA_SWITCH_ON_CTRLWRD);
            step_ctrl_state_change_cycle_count++;
            LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN3 Guard - changing drive states with a Switch on controlword");
            return false;
        }
    }
    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN3 Guard - machine command is not Switch on");
    return false;
}

/**
 * @brief Guards transition from Switched on to Operation enabled - expected machine controlword: ENABLE_OPERATION or SWITCH_ON_AND_ENABLE_OPERATION. Commands drives with CIA_ENABLE_OPERATION_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn4_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN4 Guard (Switched on to Operation enabled)");
    if ((cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_ENABLE_OPERATION)) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN4 Guard - machine command matches Enable operation");
        if (step_ctrl_check_all_drives_state(CIA_OPERATION_ENABLED)) {
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN4 Guard - All drives are Operation enabled - return true");
            step_ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            step_ctrl_change_all_drives_states(CIA_ENABLE_OPERATION_CTRLWRD);
            step_ctrl_state_change_cycle_count++;
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN4 Guard - changing drive states with a Enabled operation controlword");
            return false;
        }
    }
    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN4 Guard - machine command is not Enable operation");
    return false;
}

/**
 * @brief Guards transition from Operation enabled to Switched on - expected machine controlword: DISABLE_OPERATION >>OR<< SWITCH_ON. Commands drives with CIA_DISABLE_OPERATION_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn5_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN5 Guard (Operation enabled to Switched on)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_DISABLE_OPERATION ||
        cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_SWITCH_ON) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN5 Guard - machine command matches Disable operation");
        if (step_ctrl_check_all_drives_state(CIA_SWITCHED_ON)) {
            LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN5 Guard - All drives are Switched on - return true");
            step_ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            step_ctrl_change_all_drives_states(CIA_DISABLE_OPERATION_CTRLWRD);
            step_ctrl_state_change_cycle_count++;
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN5 Guard - changing drive states with a Disabled operation controlword (switch on controword is same)");
            return false;
        }
    }
    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN5 Guard - Machine command is not Disable operation");
    return false;
}

/**
 * @brief Guards transition from Switched on to Ready to switch on - expected machine controlword: SHUTDOWN. Commands drives with CIA_SHUTDOWN_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn6_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN6 Guard (Switched on to Ready to switch on)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_SHUTDOWN) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN6 Guard - machine command matches Shutdown");
        if (step_ctrl_check_all_drives_state(CIA_READY_TO_SWITCH_ON)) {
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN6 Guard - All drives are Ready to switch on - return true");
            step_ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            step_ctrl_change_all_drives_states(CIA_SHUTDOWN_CTRLWRD);
            step_ctrl_state_change_cycle_count++;
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN6 Guard - changing drive states with a Shutdown controlword");
            return false;
        }
    }
    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN6 Guard - Machine command is not Shutdown");
    return false;
}

/**
 * @brief Guards transition from Ready to switch on to Switch on disabled - expected machine controlword: DISABLE_VOLTAGE OR QUICK_STOP. Commands drives with CIA_DISABLE_VOLTAGE_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn7_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN7 Guard (Ready to switch on to Switch on disabled)");
    if ((cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_DISABLE_VOLTAGE) ||
        (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_QUICK_STOP)) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN7 Guard - Machine command matches Disable voltage");
        if (step_ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN7 Guard - All drives are Switch on disabled - return true");
            step_ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            step_ctrl_change_all_drives_states(CIA_DISABLE_VOLTAGE_CTRLWRD);
            step_ctrl_state_change_cycle_count++;
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN7 Guard - changing drive states with a Disabled voltage controlword");
            return false;
        }
    }
    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN7 Guard - Machine command is not Disable voltage");
    return false;
}

/**
 * @brief Guards transition from Operation enabled to Ready to switch on - expected machine controlword: SHUTDOWN. Commands drives with CIA_SHUTDOWN_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn8_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN8 Guard (Operation enabled to Ready to switch on)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_SHUTDOWN) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN8 Guard - Machine command matches Shutdown");
        if (step_ctrl_check_all_drives_state(CIA_READY_TO_SWITCH_ON)) {
            step_ctrl_state_change_cycle_count = 0;
            LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN8 Guard - All drives are Ready to swithc on - return true");
            return true;
        } else {
            step_ctrl_change_all_drives_states(CIA_SHUTDOWN_CTRLWRD);
            step_ctrl_state_change_cycle_count++;
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN8 Guard - changing drive states with a Shutdown controlword");
            return false;
        }
    }
    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN8 Guard - Machine command is not Shutdown");
    return false;
}

/**
 * @brief Guards transition from Operation enabled to Switch on disabled - expected machine controlword: DISABLE_VOLTAGE. Commands drives with CIA_DISABLE_VOLTAGE_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn9_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN9 Guard (Operation enabled to Switch on disabled)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_DISABLE_VOLTAGE) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN9 Guard - Machine command matches Disable voltage");
        if (step_ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN9 Guard - All drives are Switch on disabled - return true");
            step_ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            step_ctrl_change_all_drives_states(CIA_DISABLE_VOLTAGE_CTRLWRD);
            step_ctrl_state_change_cycle_count++;
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN9 Guard - changing drive states with a Disable voltage controlword");
            return false;
        }
    }
    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN9 Guard - Machine command is not Disable voltage");
    return false;
}

/**
 * @brief Guards transition from Switched on to Switch on disabled - expected machine controlword: DISABLE_VOLTAGE or QUICK_STOP. Commands drives with CIA_DISABLE_VOLTAGE_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn10_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN10 Guard (Switched on to Switch on disabled)");
    if ((cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_DISABLE_VOLTAGE) ||
        (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_QUICK_STOP)) {
        LL_TRACE(GBSM_SM_LOG_EN,
                 "sm: TRN10 Guard - Machine command matches Disable voltage OR Quick stop");
        if (step_ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN10 Guard - All drives are Switch on disabled - return true");
            step_ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            step_ctrl_change_all_drives_states(CIA_DISABLE_VOLTAGE_CTRLWRD);
            step_ctrl_state_change_cycle_count++;
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN10 Guard - changing drive states with a Disable voltage controlword");
            return false;
        }
    }
    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN10 Guard - Machine command is not Disable voltage");
    return false;
}


/**
 * @brief Guards transition from Operation Enabled to Quick stop active - expected machine controlword: QUICK_STOP. Commands drives with CIA_QUICK_STOP_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */

static bool cia_trn11_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN11 Guard (Operation Enabled to Quick stop active)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_QUICK_STOP) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN11 Guard - Machine command matches Quick stop");
        if (step_ctrl_check_all_drives_state(CIA_QUICK_STOP_ACTIVE) || step_ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN11 Guard - All drives are Quick stop - return true");
            step_ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            step_ctrl_change_all_drives_states(CIA_QUICK_STOP_CTRLWRD);
            step_ctrl_state_change_cycle_count++;
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN11 Guard - changing drive states with a Quick stop controlword");
            return false;
        }
    }
    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN11 Guard - Machine command is not Quick stop");
    return false;
}


//todo-crit review
/**
 * @brief Guards transition from Quick stop active to Switch on disabled - expected machine controlword: DISABLE_VOLTAGE. Commands drives with CIA_DISABLE_VOLTAGE_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn12_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN12 Guard (Quick stop active to Switch on disabled)");
    if (step_ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
        LL_TRACE(GBSM_SM_LOG_EN,
                 "sm: TRN12 Guard - All drives are Switch on disabled - return true");
        step_ctrl_state_change_cycle_count = 0;
        return true;
    } else {
//			step_ctrl_change_all_drives_states(CIA_DISABLE_VOLTAGE_CTRLWRD);
        step_ctrl_state_change_cycle_count++;
//			LL_TRACE(SM_LOG_EN, ecm_status.cycle_count, "sm: TRN12 Guard - changing drive states with a Disable voltage controlword");
        return false;
    }
//	}
//	LL_TRACE(SM_LOG_EN,  "sm: TRN12 Guard - Machine command is not Disable voltage");
//	return false;
}


/**
 * @brief Guards transition from Fault to Switch on disabled - expected machine controlword: FAULT_RESET. Commands drives with CIA_FAULT_RESET_CTRLWRD
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn15_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN15 Guard (Fault to Switch on disabled)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_FAULT_RESET) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN15 Guard - Machine command matches Fault reset");
        if (step_ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN15 Guard - All drives are Switch on disabled - return true");
            step_ctrl_state_change_cycle_count = 0;
            return true;
        } else {
            step_ctrl_change_all_drives_states(CIA_FAULT_RESET_CTRLWRD);
            step_ctrl_state_change_cycle_count++;
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN15 Guard - changing drive states with a Fault reset controlword");
            return false;
        }
    }
    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN15 Guard - Machine command is not Fault reset");
    return false;
}

/**
 * @brief Guards transition from Quick stop active to Operation enabled - expected machine controlword: ENABLE_OPERATION. Commands drives with CIA_ENABLE_OPERATION
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn16_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter
    (void) event; //unused parameter

    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN16 Guard (Quick stop active to Operation enabled)");
    if (cia_ctrlwrd_to_command(dpm_out->machine_word) == CIA_ENABLE_OPERATION) {
        LL_TRACE(GBSM_SM_LOG_EN,
                 "sm: TRN16 Guard - Machine command matches Enable operation");
        if (step_ctrl_check_all_drives_state(CIA_OPERATION_ENABLED)) {
            step_ctrl_state_change_cycle_count = 0;
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN16 Guard - All drives are Operation enabled - return true");
            return true;
        } else {
            step_ctrl_change_all_drives_states(CIA_ENABLE_OPERATION);
            step_ctrl_state_change_cycle_count++;
            LL_TRACE(GBSM_SM_LOG_EN,
                     "sm: TRN15 Guard - changing drive states with a Enabled operation controlword");
            return false;
        }
    }
    LL_TRACE(GBSM_SM_LOG_EN, "sm: TRN16 Guard - Machine command is not Enable operation");
    return false;
}

/**
 * @brief Checks heartbeat stop, estop=true, follow_error=true, drives in fault or fault reaction active, machine controlword = fault trigger
 * @param event
 * @return true there is a fault condition, false no fault condition
 * @warning doesn't check for drive states being out of sync
 */
bool cia_is_fault_condition(struct event *event) {

    ((step_event_data_t *) event->data)->fault_cause = 0;

    //reset all control events (errors)
    for (int i = 0; i < NUM_CONTROL_EVENTS; i++) {
        control_event[i].active = false;
    }


    bool have_fault = false;
    if (((step_event_data_t *) event->data)->heartbeat_lost == true) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: Fault > heart beat lost");
        BIT_SET(((step_event_data_t *) event->data)->fault_cause, FAULT_CAUSE_HEARTBEAT_LOST_BIT_NUM);
        control_event[CONTROL_EVENT_HEARTBEAT_LOST].active = true;
        have_fault = true;
    }
    if (((step_event_data_t *) event->data)->follow_error == true) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: Fault > follow error");
        BIT_SET(((step_event_data_t *) event->data)->fault_cause, FAULT_CAUSE_DRIVE_FOLLOW_ERROR_BIT_NUM);
        control_event[CONTROL_EVENT_DRIVE_FOLLOW_ERROR].active = true;
        have_fault = true;
    }
    if (((step_event_data_t *) event->data)->estop == true) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: Fault > estop");
        BIT_SET(((step_event_data_t *) event->data)->fault_cause, FAULT_CAUSE_ESTOP_BIT_NUM);
        control_event[CONTROL_EVENT_ESTOP].active = true;
        have_fault = true;
    }
    if (((step_event_data_t *) event->data)->machine_request_error == true) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: Fault > machine word is requesting error state");
        BIT_SET(((step_event_data_t *) event->data)->fault_cause, FAULT_CAUSE_GBC_FAULT_REQUEST_BIT_NUM);
        control_event[CONTROL_EVENT_GBC_FAULT_REQUEST].active = true;
        have_fault = true;
    }
    if (((step_event_data_t *) event->data)->machine_move_not_op_enabled_fault_req == true) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: Fault > machine word is requesting an error because a move has been attempted and we are not in operation enabled");
        BIT_SET(((step_event_data_t *) event->data)->fault_cause, FAULT_CAUSE_MOVE_NOT_OP_EN_BIT_NUM);
        control_event[CONTROL_EVENT_GBC_MOVE_NOT_OP_END_REQUEST].active = true;
        have_fault = true;
    }
//    CTRL_GBC_INTERNAL_FAULT_REQ_BIT_NUM

    if (((step_event_data_t *) event->data)->gbc_internal_fault == true) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: Fault > machine word is signalling GBC had an internal fault");
        BIT_SET(((step_event_data_t *) event->data)->fault_cause, FAULT_CAUSE_GBC_INTERNAL_ERROR_BIT_NUM);
        control_event[CONTROL_EVENT_GBC_MOVE_NOT_OP_END_REQUEST].active = true;
        have_fault = true;
    }

    if (step_ctrl_check_any_drives_state(CIA_FAULT_REACTION_ACTIVE)) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: Fault > one or more drives are in FAULT REACTION ACTIVE");
        BIT_SET(((step_event_data_t *) event->data)->fault_cause, FAULT_CAUSE_DRIVE_FAULT_BIT_NUM);
        control_event[CONTROL_EVENT_DRIVE_FAULT].active = true;
        have_fault = true;
    }
    if (step_ctrl_check_any_drives_state(CIA_FAULT)) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: Fault > one or more drives are in FAULT");
        BIT_SET(((step_event_data_t *) event->data)->fault_cause, FAULT_CAUSE_DRIVE_FAULT_BIT_NUM);
        control_event[CONTROL_EVENT_DRIVE_FAULT].active = true;
        have_fault = true;
    }
    if (step_ctrl_state_change_cycle_count > step_ctrl_state_change_timeout) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: Fault > one or more drives took too long reponding to a state change request");
        BIT_SET(((step_event_data_t *) event->data)->fault_cause, FAULT_CAUSE_DRIVE_STATE_CHANGE_TIMEOUT_BIT_NUM);
        control_event[CONTROL_EVENT_DRIVE_STATE_CHANGE_TIMEOUT].active = true;
        have_fault = true;
    }

    if (((step_event_data_t *) event->data)->internal_limit == true) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: Fault > a drive has hit a limit");
        BIT_SET(((step_event_data_t *) event->data)->fault_cause, FAULT_CAUSE_LIMIT_REACHED_BIT_NUM);
        control_event[CONTROL_EVENT_LIMIT_REACHED].active = true;
        have_fault = true;
    }

    if (((step_event_data_t *) event->data)->slave_reported_error == true) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: Fault > a slave reported an error");
        BIT_SET(((step_event_data_t *) event->data)->fault_cause, FAULT_CAUSE_ECAT_SLAVE_ERROR_BIT_NUM);
        control_event[CONTROL_EVENT_ECAT_SLAVE_ERROR].active = true;
        have_fault = true;
    }

    if (((step_event_data_t *) event->data)->plc_signalled_error == true) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: Fault > the PLC signalled an error");
        BIT_SET(((step_event_data_t *) event->data)->fault_cause, FAULT_CAUSE_PLC_SIGNALLED_ERROR_BIT_NUM);
        control_event[CONTROL_EVENT_ECAT_SLAVE_ERROR].active = true;
        have_fault = true;
    }

    if (((step_event_data_t *) event->data)->homing_failed == true) {
        LL_TRACE(GBSM_SM_LOG_EN, "sm: Fault > the homing operation failed for one or more drives");
        BIT_SET(((step_event_data_t *) event->data)->fault_cause, FAULT_CAUSE_HOMING_ERROR_BIT_NUM);
        control_event[CONTROL_EVENT_HOMING_ERROR].active = true;
        have_fault = true;
    }




    dpm_in->active_fault_word = ((step_event_data_t *) event->data)->fault_cause;

    return have_fault;
}

/**
 * @brief Guards transition from any state into fault reaction active - expected machine controlword drive this (apart from the trigger fault bit)
 * @param condition
 * @param event
 * @return true=transition allowed, false=transition not allowed
 */
static bool cia_trn13_guard(void *condition, struct event *event) {

    if (cia_is_fault_condition(event)) {
        //a QUICK_STOP command should send all drives back to switch on disabled
        step_ctrl_change_all_drives_states(CIA_QUICK_STOP_CTRLWRD);
        return true;
    }
    // condition will be the state that we are currently in

    /*
     * we are in op enabled
     * controlword is switch on disabled
     * drives are in quick stop
     * cycle change count will be ticking up
     * eventually if state does not match the controlword we will error (state change timeout)
     *
     * BUT
     * if we are in op enabled
     * controlword is enable op
     * drive drops to switch on disabled
     *
     *
     */
    bool state_mismatch = false;

    switch (cia_ctrlwrd_to_command(dpm_out->machine_word)) {

        //    uint32_t x=(uint32_t) ((intptr_t) condition & 0xFFFFFFFF);

// machine is commanding SHUTDOWN, our state = READY_TO_SWITCH_ON
        case CIA_SHUTDOWN:
            if (((cia_state_t) ((void *) condition) != CIA_READY_TO_SWITCH_ON) &&
                (step_ctrl_state_change_cycle_count > step_ctrl_state_change_timeout)) {
                state_mismatch = true;
            }
            break;
        case CIA_SWITCH_ON:
            if (((cia_state_t) ((uintptr_t *) condition) != CIA_SWITCHED_ON) &&
                (step_ctrl_state_change_cycle_count > step_ctrl_state_change_timeout)) {
                state_mismatch = true;
            }
            break;
//        case CIA_SWITCH_ON_AND_ENABLE_OPERATION:
//            if ((((cia_state_t) ((intptr_t *) condition) != CIA_SWITCHED_ON) ||
//                 ((cia_state_t) ((intptr_t *) condition) != CIA_OPERATION_ENABLED))
//                && step_ctrl_state_change_cycle_count > step_ctrl_state_change_timeout) {
//                state_mismatch = true;
//            }
//            break;
        case CIA_DISABLE_VOLTAGE:
            if ((((cia_state_t) ((intptr_t *) condition) != CIA_SWITCHED_ON) ||
                 ((cia_state_t) ((intptr_t *) condition) != CIA_OPERATION_ENABLED))
                && step_ctrl_state_change_cycle_count > step_ctrl_state_change_timeout) {
                state_mismatch = true;
            }
            break;
        case CIA_QUICK_STOP:
            if ((((cia_state_t) ((intptr_t *) condition) != CIA_QUICK_STOP_ACTIVE) ||
                 ((cia_state_t) ((intptr_t *) condition) != CIA_SWITCH_ON_DISABLED)) &&
                step_ctrl_state_change_cycle_count > step_ctrl_state_change_timeout) {
                state_mismatch = true;
            }
            break;
        case CIA_DISABLE_OPERATION:
            if (((cia_state_t) ((intptr_t *) condition)) != CIA_SWITCHED_ON &&
                (step_ctrl_state_change_cycle_count > step_ctrl_state_change_timeout)) {
                state_mismatch = true;
            }
            break;
        case CIA_ENABLE_OPERATION:
            if (((cia_state_t) ((intptr_t *) condition) == CIA_OPERATION_ENABLED) &&
                (step_ctrl_state_change_cycle_count > step_ctrl_state_change_timeout)) {
                state_mismatch = true;
            }
            break;
        case CIA_FAULT_RESET:
            break;
    }
    if (state_mismatch) {
        LL_TRACE(GBSM_SM_LOG_EN,
                 "sm: Mismatch between drive state and the controlword and the current state & no transitions occurring");
        step_ctrl_change_all_drives_states(CIA_QUICK_STOP_CTRLWRD);
        return false;
    }
    //	!! we transition to fault reaction active then loop in that state until drives have stopped
    return false;
}
//check that the drives are in sync with the statemachine state

//we have the problem here that drives might be one step ahead of the state machine or one step behind

/*
 * we are in state op enabled
 * control word is disabled voltage
 * we set drive control words to disable voltage
 * state remains op enabled until drives report switch on disabled
 *
 */


/**
 * @brief Guards the transition from fault reaction to fault that occurs when a fault is cleared - doesn't depend on machine controlword
 * @param condition
 * @param event
 * @return
 */
static bool cia_trn14_guard(void *condition, struct event *event) {
    (void) condition; //unused parameter

    static int reset_in_progress = 0;
//todo-crit this isnt correct
    if (step_ctrl_check_all_drives_state(CIA_SWITCH_ON_DISABLED)) {
        // all drives have transitioned to switch on so the state machine can go to fault state if no error
        if (!cia_is_fault_condition(event)) {
            return true;
        }
    } else {

        reset_in_progress++;
        if ((reset_in_progress % 5) == 0) {
            step_ctrl_change_all_drives_states(CIA_FAULT_RESET_CTRLWRD);
//            printf("reset\n");
        } else {
            step_ctrl_change_all_drives_states(0b00000000);
//            printf("reset 0\n");
        }
    }
    return false;
}

static void sm_error_state_entry_action(void *stateData, struct event *event) {
    (void) stateData; //unused parameter
    (void) event; //unused parameter

    LL_FATAL("GBSM: Reached error state of statemachine");
}
//GB-EM sends the data from dpmIN over spi (this is the IN i.e. stuff read from fieldbus or created by state machine)
//GB-EM receives data into DPMout over spi (this is the OUT i.e. stuff to be written to the fieldbus or be processed by the state machine)

/**
 * @brief checks all drives against a provided state
 * @param drive_status array of drive statuses
 * @param state to check
 * @return true=all drives are in the state, one or more drives are not in the state
 */
bool step_ctrl_check_all_drives_state(cia_state_t state) {

    uint16_t drive_stat_wrd[SM_NUM_DRIVES];

    drv_get_stat_wrd(drive_stat_wrd);

    int j = 0;
    for (int i = 0; i < SM_NUM_DRIVES; i++) {

        if (cia_statwrd_to_state(drive_stat_wrd[i]) == state) {
            j++;
        }
    }
    if (j == SM_NUM_DRIVES) {
        return true;
    } else {
        return false;
    }
}


bool step_ctrl_check_any_drives_state(cia_state_t state) {

    uint16_t drive_stat_wrd[SM_NUM_DRIVES];
    drv_get_stat_wrd(drive_stat_wrd);

    for (int i = 0; i < SM_NUM_DRIVES; i++) {

        if (cia_statwrd_to_state(drive_stat_wrd[i]) == state) {
            //we found a drive in the given state
            return true;
        }
    }
    //we didn't find any drives in the given state
    return false;
}


/**
 * @brief sends a given controlword to all drives
 * @param controlword
 * @param event
 */
void step_ctrl_change_all_drives_states(uint16_t controlword) {
    LL_TRACE(GBSM_SM_LOG_EN, "sm: Change All drives control word: %s",
             cia_command_names[cia_ctrlwrd_to_command(controlword)]);

    drv_set_ctrl_wrd(controlword);

}

/**
 * @brief action called by an unguarded transition on the fault reaction active state to set the current fault cause bits
 * @param oldStateData
 * @param event
 * @param event
 * @param newStateData
 * @attention this is set every cycle in fault reaction active - it is current fault cause
 */
static void cia_set_current_fault_causes_action(void *oldStateData, struct event *event, void *newStateData) {
    (void) oldStateData; //unused parameter
    (void) newStateData; //unused parameter

/*this will record in the event struct the fault bits */
    cia_is_fault_condition(event);

//        dpm_in->active_fault_word = ((event_data_t *) event->data)->fault_cause;
}

/**
 * @brief entry action called when the state machine enters a state for the first time
 * @param stateData
 * @param event
 */
static void cia_generic_entry_action(void *stateData, struct event *event) {
//The drives have been commanded to change state and checked they have changed state in the transition
    LL_TRACE(GBSM_SM_LOG_EN, "sm: Entry action: current state = %s", cia_state_names[(int) ((cia_state_t) stateData)]);

    /* We need to:
     * a) set the ecm_status overall sm state (in the shared mem struct)
     * b) set the ecm_status drive statuses (in the shared mem struct)
     * c) set the machine status word (this is a write to the SPI DPM)
     * d)
     */
    //first set the overall current state of the gb-em statemachine
    //it is ok to do this in the generic entry action as every time we change state we pass through here

    dpm_in->machine_word = 0;
    switch ((cia_state_t) stateData) {
        case CIA_NOT_READY_TO_SWITCH_ON:
            dpm_in->machine_word = CIA_NOT_READY_TO_SWITCH_ON_STATWRD;
            break;
        case CIA_SWITCH_ON_DISABLED:
            dpm_in->machine_word = CIA_SWITCH_ON_DISABLED_STATWRD;
            break;
        case CIA_READY_TO_SWITCH_ON:
            dpm_in->machine_word = CIA_READY_TO_SWITCH_ON_STATWRD;
            break;
        case CIA_SWITCHED_ON:
            dpm_in->machine_word = CIA_SWITCHED_ON_STATWRD;
            break;
        case CIA_OPERATION_ENABLED:
            dpm_in->machine_word = CIA_OPERATION_ENABLED_STATWRD;
            break;
        case CIA_QUICK_STOP_ACTIVE:
            dpm_in->machine_word = CIA_QUICK_STOP_ACTIVE_STATWRD;
            break;
        case CIA_FAULT_REACTION_ACTIVE:
            dpm_in->machine_word = CIA_FAULT_REACTION_ACTIVE_STATWRD;
            /* trn13 should have called cia_is_fault_condition to set current faults in event struct*/
            dpm_in->fault_history_word = ((step_event_data_t *) event->data)->fault_cause;
            break;
        case CIA_FAULT:
            dpm_in->machine_word = CIA_FAULT_STATWRD;
            break;
    }
}

/**
 * @brief malloc a state machine struct
 * @return ptr to stateMachine struct
 */
struct stateMachine *step_ctrl_statemachine_alloc(void) {

#ifdef GB_APP_LINUX
    struct stateMachine *smachine = malloc(sizeof(struct stateMachine));
#else
    struct stateMachine *smachine = pvPortMalloc(sizeof(struct stateMachine));
#endif
    return smachine;
}

/**
 * @brief free a stateMachine struct
 * @param smachine
 */
void step_ctrl_statemachinefree(struct stateMachine *smachine) {
#ifdef GB_APP_LINUX
    free(smachine);
#else
    vPortFree(smachine);
#endif
}


/**
 * @brief initialise the state machine so start in not ready to switch on
 * @param smachine
 */
void step_ctrl_init_statemachine(struct stateMachine *smachine) {
    stateM_init(smachine, &cia_not_ready_to_switch_on_state, &sm_error_state);
}

/**
 * @brief check heartbeat (incremented by GBSM and echoed by GBC)
 * @param gbsm_heartbeat
 * @param gbc_heartbeat
 * @return true when heartbeat is within tolerance (CTRL_HEARTBEAT_TOLERANCE) return false when lost
 */
bool step_ctrl_check_heartbeat_ok(uint32_t gbsm_heartbeat_to_check, uint32_t gbc_heartbeat_to_check) {


//    printf("gbsm hb [%u]\n", gbsm_heartbeat_to_check);
//    printf("gbc hb [%u]\n", gbc_heartbeat_to_check);

    if ((gbsm_heartbeat_to_check - gbc_heartbeat_to_check) > CTRL_HEARTBEAT_TOLERANCE) {
        return false;
    }

    /*
     * cycle 1 - gb-em send out 0 to gbc - gbc echos 0 back
     * cycle 2 - gb-em send out 1 to gbc - gbc echos 0 back
     * cycle 2 - gb-em send out 2 to gbc - gbc echos 1 back
     * cycle 3 - gb-em send out 3 to gbc - gbc echos 2 back
     */
    return true;
}


/**
 * @brief main control loop
 * @param m state machine object
 * @param first_run bool indicating that this is the first execution cycle
 * @param
 */
void step_ctrl_main(struct stateMachine *m, bool first_run) {


    if (first_run) {
    }

//#if FLAVOUR == PLATFORM_PI
//    drv_send_start_trigger();
//#endif


    //copy actpos to DPM_IN (write) (do this every cycle irrespective of state)

    step_ctrl_copy_actpos();
    step_ctrl_copy_setpos();

#if FLAVOUR == PLATFORM_PI
    drv_send_start_trigger();
#endif



//    printf("actpos [%d]\n", dpm_in->joint_actual_position[0]);

    //copy statuswords from to DPM_IN (write)
    step_ctrl_copy_drive_statuswords();
    //copy digital ins from EC_IN to DPM_IN (write)
    step_ctrl_process_iomap_in();


    //increment heartbeat (this is the value stored in a static var on gb-em and sent to gbc and echoed back from there
    gbsm_heartbeat++;
    //write heartbeat to DPM_IN (Write)
    dpm_in->heartbeat = gbsm_heartbeat;




    //We check for estop against the DPM copy of the data from EC - we copied it in above
    /*
     * if estop digin is false then set estop=true
     * if estop digin is false AND estopreset is true estop=false
     */



#if USE_ESTOP_RESET == 1 && DISABLE_ESTOP_CHECKING != 1


    if (!din[CTRL_ESTOP_DIN]){
//        printf("estop=true");
        estop = true;
    } else {
//        if (BIT_CHECK(dpm_in->digital, CTRL_ESTOP_RESET_DIN)) {
if (din[CTRL_ESTOP_RESET_DIN]){
            estop = false;
        }
    }
#endif

#if  USE_ESTOP_RESET == 0 && DISABLE_ESTOP_CHECKING != 1

    if (!din[CTRL_ESTOP_DIN]) {
        estop = true;
    }
    else {
        estop = false;
    }
#endif




    step_event_data.heartbeat_lost = !step_ctrl_check_heartbeat_ok(gbsm_heartbeat, dpm_out->heartbeat);

#if DISABLE_HEARTBEAT_CHECKING == 1
    event_data.heartbeat_lost = false;
#endif

    step_event_data.estop = estop;
// warning estop frig!!!!!!!!!!!!!!!!
#if DISABLE_ESTOP_CHECKING == 1
    step_event_data.estop = false;
#endif
    gberror_t grc;



    step_event_data.internal_limit = step_ctrl_check_for_internal_limit(&grc);
    if (grc != E_SUCCESS) {
        UM_ERROR(GBSM_UM_EN, "GBSM: error checking drive for internal limit [%s]", gb_strerror(grc));
    }

    step_event_data.follow_error = step_ctrl_check_for_follow_error(&grc);
    if (grc != E_SUCCESS) {
        UM_ERROR(GBSM_UM_EN, "GBSM: error checking drive for follow error [%s]", gb_strerror(grc));
    }

    step_event_data.homing_failed = homing_failed;

    if (step_ctrl_check_remote()) {
        step_event_data.remote_ok = true;
    } else {
        step_event_data.remote_ok = false;
    }

    step_event_data.machine_request_error = BIT_CHECK(dpm_out->machine_word, CTRL_MACHINE_CTRL_WRD_REQUEST_FAULT_BIT_NUM);



//    event_data.homing_failed = homing_failed;



    /* This prints out the drive status words */
//    for (int i = 0; i < SM_NUM_DRIVES; i++) {
//        printf("drive:%u : status word:%s\n", i, cia_state_names[cia_statwrd_to_state(dpm_in->joint_statusword[i])]);
//    }


    drv_update_state();


    /*	 run the state machine */
    int ret = stateM_handleEvent(m, &(struct event)
            {
                    Event_cyclic,
                    (void *) (intptr_t) &step_event_data});

    if (ret == stateM_errArg) {
        LL_FATAL("GBSM: Erroneous arguments state machine");
    }
    if (ret == stateM_errorStateReached) {
        LL_FATAL("GBSM: State machine reached error state");
    }
    if (ret == stateM_noStateChange) {

    }
    /* state returned to itself */
    if (ret == stateM_stateLoopSelf) {

    }

    if (ret == stateM_finalStateReached) {
        LL_FATAL(
                "GBSM: State machine reached a final state - this should never happen unless something has gone inexplicably pear shaped");
    }

//read current state of state machine
    current_state = (cia_state_t) (stateM_currentState(m)->data);


    LL_TRACE(GBSM_SM_LOG_EN, "sm: Current state of state machine - %s", cia_state_names[current_state]);

//    printf("fault word:%u\n", dpm_in->active_fault_word);





//copy setpos from DPM_OUT (read) to EC_OUT
//    step_ctrl_copy_setpos();


    //    printf("setpos [%d]\n", dpm_out->joint_set_position[0]);




    if (current_state == CIA_OPERATION_ENABLED) {
        step_ctrl_process_iomap_out(false);

    } else {
        step_ctrl_process_iomap_out(true);
    }




//todo-crit
    //output user messages for any faults that have occurred
//    print_cyclic_user_message(NUM_CONTROL_EVENTS, control_event, print_re);



}

//copy joint actpos - from ec to dpm
/**
 * @brief actpos to be sent to GBC via DPM in is set from actpos received over EC from the drive
 * @param slave_ptrs
 * @param inout_ptrs
 */
static void step_ctrl_copy_actpos(void) {
    drv_get_actpos(dpm_in->joint_actual_position);

}



/**
 * @brief setpos on drive is set to setpos from GBC (from DPM out)
 * @param slave_ptrs
 * @param inout_ptrs
 */
static void step_ctrl_copy_setpos(void) {

    drv_set_setpos(dpm_out->joint_set_position);
}

/**
 * @brief copy drive status words from EC to DPM_IN
 */
static void step_ctrl_copy_drive_statuswords(void) {

    drv_get_stat_wrd(dpm_in->joint_statusword);

}


void step_ctrl_process_iomap_in (void) {
    gberror_t grc = E_GENERAL_FAILURE;
    dpm_in->digital = 0;

    for (int i = 0; i < DPM_NUM_DIGITALS; i++) {
        if (io_get_din(i, &grc) == true) {
            BIT_SET(dpm_in->digital, true);
            if (grc != E_SUCCESS) {
                if (grc == E_GPIO_FAILURE) {
                    LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: failed to read to GPIO pin - GPIO problem");
                } else if (grc == E_NOT_FOUND) {
//                    LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: failed to read to GPIO pin - GPIO not found");
                } else {
                    LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: failed to read to GPIO pin - unknown problem");
                }
            }
        }

    }
}

void step_ctrl_process_iomap_out (const bool zero){
    gberror_t grc = E_GENERAL_FAILURE;
    for (int i = 0; i < DPM_NUM_DIGITALS; i++) {
        if (!zero) {
            io_set_dout(i, BIT_CHECK(dpm_out->digital, i), &grc);
            if (grc!=E_SUCCESS){
                if(grc==E_GPIO_FAILURE){
                    LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: failed to write to GPIO pin - GPIO problem");
                }else if (grc==E_NOT_FOUND){
//                    LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: failed to write to GPIO pin - GPIO not found");
                }else{
                    LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: failed to write to GPIO pin - unknown problem");
                }

            }
            BIT_SET(dpm_in->digital, true);
        }else{
            io_set_dout(i, false, &grc);
        }

    }

}

bool step_ctrl_check_for_follow_error(gberror_t *grc){
    int number_of_follow_errors = 0;
    for (int i = 0; i < SM_NUM_DRIVES; i++) {

        if (g_drv_status_flag[i].stall) {
            //we have a stall/follow-error
            number_of_follow_errors++;

        }
    }
    *grc = E_SUCCESS;

    if(number_of_follow_errors == 0){
        return false;
    }else{
        return true;
    }


}


bool step_ctrl_check_for_internal_limit(gberror_t *grc) {

    int number_of_vstops = 0;
    for (int i = 0; i < SM_NUM_DRIVES; i++) {

        if (g_drv_status_flag[i].l_vstop || g_drv_status_flag[i].r_vstop) {
            //we have a vstop
            number_of_vstops++;

        }
    }
    *grc = E_SUCCESS;

    if (number_of_vstops == 0) {
        return false;
    } else {
        return true;
    }
}

bool step_ctrl_check_remote(void){
    return true;
}