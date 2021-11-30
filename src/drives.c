
/**
 ******************************************************************************
 * @file           :  drives.c
 * @brief          :  drive control functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "cia402.h"
#include "drives.h"
#include "stdio.h"
#include "SPI_TMC.h"
#include "TMC5160_Register.h"
#include "TMC5160_Fields.h"
#include "TMC5160_Constants.h"
#include "TMC4361A_Fields.h"
#include "TMC4361A_Register.h"
#include "TMC4361A_Constants.h"
#include "std_defs_and_macros.h"
#include "pigpio.h"
#include "log.h"
#include "user_message.h"
#include "gpio.h"
#include "dpm.h"
#include "signal.h"
#include "math.h"
#include "io.h"
#include "std_utils.h"
#include "tmc4361a_status.h"

#include "tmc4361a_current_conf.h"
#include "tmc4361a_enc_in_conf.h"
#include "tmc4361a_enc_in_data_conf.h"
#include "tmc4361a_enc_out_data_conf.h"
#include "tmc4361a_events_clear_conf.h"
#include "tmc4361a_general_conf.h"
#include "tmc4361a_input_filt_conf.h"
#include "tmc4361a_intr_conf.h"
#include "tmc4361a_reference_conf.h"
#include "tmc4361a_scale_values_conf.h"
#include "tmc4361a_spi_status_selection_conf.h"
#include "tmc4361a_spiout_conf.h"
#include "tmc4361a_start_conf.h"
#include "tmc4361a_step_conf.h"
#include "tmc4361a_ramp_mode_conf.h"
#include "tmc4361a_stp_length_add_conf.h"
#include "tmc4361a_freeze_conf.h"

#include "tmc5160_general_conf.h"
#include "tmc5160_ihold_irun_conf.h"
#include "tmc5160_chop_conf.h"
#include "tmc5160_cool_conf.h"

//for status bits read with every write
uint8_t tmc_status_from_spi[SM_NUM_DRIVES];


status_event_t g_drv_status_event[SM_NUM_DRIVES];
status_flag_t g_drv_status_flag[SM_NUM_DRIVES];
cia_state_t g_drv_cia_state[SM_NUM_DRIVES];
uint16_t g_drv_ctrlwrd[SM_NUM_DRIVES];

bool g_drv_enabled[SM_NUM_DRIVES] = {0};
uint32_t g_tmc5160_version[SM_NUM_DRIVES] = {0};
uint32_t g_tmc4361a_version[SM_NUM_DRIVES] = {0};


static void drv_reset_tmc4361a(bool use_nrst, const unsigned int *hspi);

static void drv_reset_tmc5160(void);

static void drv_init_tmc5160_covers(const unsigned int *hspi);

static void drv_read_ic_versions(const unsigned int *hspi);

static bool test_tmc4361a(void);

static uint32_t *read_motor_currents(void);

/*
 * Configure register 0x04 (SPI_OUT_CONF):
 * Polling activated: SPI_OUT_CONF = 0x4440100C (spi_output_format = 12, disable_polling = 0, cover_done_only_for_cover = 1, cover_data_length= 0)
 * Polling deactivated:  SPI_OUT_CONF = 0x4440104C (spi_output_format = 12, disable_polling = 1, cover_done_only_for_cover = 1, cover_data_length = 0)
 * Read data from register eg. GCONF (0x00):
 * With polling activated:
 * WRITE to register 0x6D -> 0x00 (GCONF)
 * WRITE to register 0x6C -> 0xXX
 * READ from register 0x6E
 * READ from register 0x6E -> This datagram holds the GCONF register value
 * With polling deactivated:
 * WRITE to register 0x6D -> 0x00 (GCONF)
 * WRITE to register 0x6C -> 0xXX
 * WRITE to register 0x6C -> 0xXX
 * READ from register 0x6E
 * READ from register 0x6E -> This datagram holds the GCONF register value
*/


/* TMC4361A maps the following status bits of TMC26x stepper drivers – which are transferred within each SPI response – to the STATUS register 0x0F:
 * SG - stallGuard2™ status flag STATUS(25)
 * OT - Over temperature flag STATUS(26)
 * OTPW - Temperature prewarning flag STATUS(27)
 * S2GA - Short-to-ground detection flag for high side MOSFET of coil A STATUS(28)
 * S2GB - Short-to-ground detection flag for high side MOSFET of coil B STATUS(29)
 * OLA - Open load flag for bridge A STATUS(30)
 * OLB - Open load flag for bridge B STATUS(31)
 * STST - Standstill flag
 *
*/



/**
 * @brief sets the enabled flag in g_drv_status (used when not running on mbed linux)
 */
void drv_set_enabled(void) {
    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        g_drv_enabled[i] = true;

    }
}

/**
 * @brief reads the IC versions from the 5160 and 4361 and store them in globals
 * @param hspi
 */
static void drv_read_ic_versions(const unsigned int *hspi) {

    /* read 4361 version register*/
    tmc4361A_read_uint32_all(TMC4361A_VERSION_NO_RD, g_tmc4361a_version, SM_NUM_DRIVES, hspi);

//    the response is stored in registers; and is thus available for the microcontroller. COVER_DRV_HIGH register 0x6F and COVER_DRV_LOW register 0x6E form

/*
 * no polling
 * WRITE to register 0x6D -> 0x00 (GCONF)
 * WRITE to register 0x6C -> 0xXX
 * WRITE to register 0x6C -> 0xXX
 * READ from register 0x6E
 * READ from register 0x6E -> This datagram holds the GCONF register value

  * WRITE to register 0x6D -> 0x00 (GCONF)
 * WRITE to register 0x6C -> 0xXX
 * READ from register 0x6E
 * READ from register 0x6E -> This datagram holds the GCONF register value


 */


    /* read 5160 version register through a cover*/
//    & 0x7f

    tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_INP_OUT, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    gpioDelay(1000);
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_LOW_WR, 0, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    gpioDelay(1000);
//    tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_LOW_WR, 0, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    gpioDelay(1000);
    tmc4361A_read_uint32_all(TMC4361A_COVER_DRV_LOW_RD, g_tmc5160_version, SM_NUM_DRIVES, hspi);
//    gpioDelay(1000);
//    tmc4361A_read_uint32_all(TMC4361A_COVER_DRV_LOW_RD, g_tmc5160_version, SM_NUM_DRIVES, hspi);

    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        g_tmc5160_version[i] = g_tmc5160_version[i] >> 24;
    }

//    printf("tmc5160_version [%u]\n", g_tmc5160_version[0]);
}

/**
 * @brief checks the 4361 and 5160 are enabled (reads version) and sets the enabled flag
 * @return E_INIT_FAILURE or E_SUCCESS
 */
gberror_t drv_check_and_enable(void) {

    bool tmc5160_ok[SM_NUM_DRIVES] = {false};
    bool tmc4361a_ok[SM_NUM_DRIVES] = {false};

    drv_read_ic_versions(&hspi1);

    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        if (g_tmc5160_version[i] >= 0x30) {
            tmc5160_ok[i] = true;
            LL_INFO(GBSM_GEN_LOG_EN, "GBSM: TMC5160 [%d] is initialised ok. IC version is [%#3x]", i, g_tmc5160_version[i]);
        } else {
            LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: TMC5160 [%d] initialisation >failed<", i);
        }
        if (g_tmc4361a_version[i] >= 0x2) {
            tmc4361a_ok[i] = true;
            LL_INFO(GBSM_GEN_LOG_EN, "GBSM: TMC4361A [%d] is  initialised ok. IC version is [%#3x]", i, g_tmc4361a_version[i]);
        } else {
            LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: TMC4361A [%d] initialisation >failed<", i);
        }
    }

    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        if (tmc5160_ok[i] && tmc4361a_ok[i]) {
            g_drv_enabled[i] = true;
        }
    }

    uint8_t num_enabled = 0;
    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        if (g_drv_enabled[i] == true) {
            num_enabled++;
        }
    }


    //belt and braces check of tmc4361a
    if (!test_tmc4361a()) {
        UM_ERROR(GBSM_UM_EN, "GBSM: Error testing write of tmc4361a xactual write/read");
    }

    uint32_t * motor_currents;

    motor_currents = read_motor_currents();

    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        UM_INFO(GBSM_UM_EN, "GBSM: Motor current [%u] motor [%d]", motor_currents[i], i);
        if (motor_currents[i]==0){
            UM_ERROR(GBSM_UM_EN, "GBSM: Error - current is zero on motor [%d]", i);
        }
    }




    if (num_enabled == SM_NUM_DRIVES) {
        return E_SUCCESS;
    } else {
        return E_INIT_FAILURE;
    }
}

/**
 * @brief configures the tmc5160 motor drivers using cover writes
 * @param hspi
 *
 * COVER DATAGRAMS WRITE DIRECTLY TO TEH DRIVER (PASS THROUGH)
*/
static void drv_init_tmc5160_covers(const unsigned int *hspi) {


    tmc5160_general_conf_set(hspi);
    tmc5160_ihold_irun_conf_set(hspi);
    tmc5160_chop_conf_set(hspi);
    tmc5160_cool_conf_set(hspi);


//    uint32_t check_tmc5160_gconf[SM_NUM_DRIVES];
//
//    tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_GCONF & 0x7f, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
//    tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_LOW_WR, 0, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
//    gpioDelay(1000);
//    tmc4361A_read_uint32_all(TMC4361A_COVER_DRV_HIGH_RD, check_tmc5160_gconf, SM_NUM_DRIVES, hspi);
//    tmc4361A_read_uint32_all(TMC4361A_COVER_DRV_LOW_RD, check_tmc5160_gconf, SM_NUM_DRIVES, hspi);
//
//    for (int i = 0; i < SM_NUM_DRIVES; i++) {
//        if (check_tmc5160_gconf[i]==g_conf){
//            LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: cover write test passed (the values written to the GCONF register matched those read back)");
//        }else{
//            LL_FATAL("GBSM: cover write test >failed< (the values written to the GCONF register did >not< matched those read back)");
//        }
//    }


    // TPOWERDOWN=10: Delay before power down in stand still
    tmc4361A_write_int32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_TPOWERDOWN | 0x80, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    tmc4361A_write_int32_to_all_same_val(TMC4361A_COVER_LOW_WR, 0x0000000A, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    gpioDelay(1000); // COVER_DONE flag: ~90µs -> 1 ms more than enough

    // TPWMTHRS=5000
    tmc4361A_write_int32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_TPWMTHRS | 0x80, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    tmc4361A_write_int32_to_all_same_val(TMC4361A_COVER_LOW_WR, 0x00001388, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    gpioDelay(1000); // COVER_DONE flag: ~90µs -> 1 ms more than enough

    //TCOOLTHRS
    tmc4361A_write_int32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_TCOOLTHRS | 0x80, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    tmc4361A_write_int32_to_all_same_val(TMC4361A_COVER_LOW_WR, 0xffff, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    gpioDelay(1000); // COVER_DONE flag: ~90µs -> 1 ms more than enough




}


/**
 * @brief resets TMC4361A using either NRST pins or write of magic value to a register
 * @param use_nrst
 * @param hspi pigpio spi handle
 */
static void drv_reset_tmc4361a(bool use_nrst, const unsigned int *hspi) {
    if (use_nrst) {
        gpioWrite(PIN_TMC4361A_NRST, 0);
        gpioDelay(20000);
        gpioWrite(PIN_TMC4361A_NRST, 1);
    } else {
        //Write magic value to the reset register
        tmc4361A_write_uint32_to_all_same_val(TMC4361A_RESET_REG, 0x52535400, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    }

}

/**
 * @brief resets TMC5160 by cycling the VCC_IO pin
 *
 * @warning on Trinamic eval boards the VCC_IO pin is hardwired and this wont work
 */
static void drv_reset_tmc5160(void) {
    gpioWrite(PIN_TMC5160_VCC_IO, 0); // TMC5160: Reset
    gpioDelay(1000000);
    gpioWrite(PIN_TMC5160_VCC_IO, 1); // TMC5160: Reset

}

/**
 * @brief initialises the TMC4361A
 * @param hspi pigpio spi handle
 */
static void drv_init_tmc4361a(const unsigned int *hspi) {

    //TMC4361A_GENERAL_CONF          0x00
    tmc4361a_general_conf_set(hspi);
    //TMC4361A_REFERENCE_CONF        0x01
    tmc4361a_reference_conf_set(hspi);
    //TMC4361A_START_CONF            0x02
    tmc4361a_start_conf_set(hspi);
    //TMC4361A_INPUT_FILT_CONF       0x03
    tmc4361a_input_filt_conf_set(hspi);
    //TMC4361A_SPIOUT_CONF           0x04
    tmc4361a_spiout_conf_set(hspi);
    //TMC4361A_CURRENT_CONF          0x05
    tmc4361a_current_conf_set(hspi);
    //TMC4361A_SCALE_VALUES          0x06
    tmc4361a_scale_values_conf_set(hspi);
    //TMC4361A_ENC_IN_CONF           0x07
    tmc4361a_enc_in_conf_set(hspi);
    //TMC4361A_ENC_IN_DATA           0x08
    tmc4361a_enc_in_data_conf_set(hspi);
    //TMC4361A_ENC_OUT_DATA          0x09
    tmc4361a_enc_out_data_conf_set(hspi);
    //TMC4361A_STEP_CONF             0x0A
    tmc4361a_step_conf_set(hspi);
    //TMC4361A_SPI_STATUS_SELECTION  0x0B
    tmc4361a_spi_status_selection_conf_set(hspi);
    //TMC4361A_EVENT_CLEAR_CONF      0x0C
    tmc4361a_events_clear_conf_set(hspi);
    //TMC4361A_INTR_CONF             0x0D
    tmc4361a_intr_conf_set(hspi);

    //TMC4361A_STP_LENGTH_ADD        0x10
    tmc4361a_stp_length_add_conf_set(hspi);

    //TMC4361A_DIR_SETUP_TIME        0x10
    //TMC4361A_START_OUT_ADD         0x11
    //TMC4361A_GEAR_RATIO            0x12
    //TMC4361A_START_DELAY           0x13
    tmc4361A_write_int32_to_all_same_val(TMC4361A_START_DELAY, 0, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    //TMC4361A_CLK_GATING_DELAY      0x14
    //TMC4361A_STDBY_DELAY           0x15
    //TMC4361A_FREEWHEEL_DELAY       0x16
    //TMC4361A_VDRV_SCALE_LIMIT      0x17
    //TMC4361A_PWM_VMAX              0x17
    //TMC4361A_UP_SCALE_DELAY        0x18
    //TMC4361A_CL_UPSCALE_DELAY      0x18
    //TMC4361A_HOLD_SCALE_DELAY      0x19
    //TMC4361A_CL_DOWNSCALE_DELAY    0x19
    //TMC4361A_DRV_SCALE_DELAY       0x1A
    //TMC4361A_BOOST_TIME            0x1B
    //TMC4361A_CL_BETA               0x1C
    //TMC4361A_CL_GAMMA              0x1C
    //TMC4361A_DAC_ADDR_A            0x1D
    //TMC4361A_DAC_ADDR_B            0x1D
    //TMC4361A_SPI_SWITCH_VEL        0x1D
    //TMC4361A_HOME_SAFETY_MARGIN    0x1E
    //TMC4361A_PWM_FREQ              0x1F
    //TMC4361A_CHOPSYNC_DIV          0x1F

    //TMC4361A_RAMPMODE              0x20
    tmc4361a_ramp_mode_conf_set(hspi);

    //TMC4361A_XACTUAL               0x21
    //TMC4361A_VACTUAL               0x22
    //TMC4361A_AACTUAL               0x23
    //TMC4361A_VMAX                  0x24
    //TMC4361A_VSTART                0x25
//    fixed_point_t vstart, vstop;
//    double vstart_d = 6000000;
//    vstart = float_to_fixed(vstart_d);
//    tmc4361A_write_uint32_to_all_same_val(TMC4361A_VSTART, (uint32_t) vstart, SM_NUM_DRIVES, hspi, tmc_status_from_spi);


    //TMC4361A_VSTOP                 0x26
//    double vstop_d = 6000000;
//    vstop = float_to_fixed(vstop_d);
//    tmc4361A_write_uint32_to_all_same_val(TMC4361A_VSTOP, (uint32_t) vstop, SM_NUM_DRIVES, hspi, tmc_status_from_spi);

    //TMC4361A_VBREAK                0x27
    //TMC4361A_AMAX                  0x28
    //TMC4361A_DMAX                  0x29
    //TMC4361A_ASTART                0x2A
    //TMC4361A_SIGN_AACT             0x2A
    //TMC4361A_DFINAL                0x2B
    //TMC4361A_DSTOP                 0x2C
    //TMC4361A_BOW1                  0x2D
    //TMC4361A_BOW2                  0x2E
    //TMC4361A_BOW3                  0x2F
    //TMC4361A_BOW4                  0x30
    //TMC4361A_CLK_FREQ              0x31
    //MIN = 4MHz, TYP = 16MHz, MAX = 30MHz
    /* CLK_FREQ=9.6MHz - 9600000 = 0x927C00 */
    /* CLK_FREQ=16MHz - 16000000 = 0xF42400 */
    /* CLK_FREQ=20MHz - 20000000 = 0x1312D00 */
    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        tmc4361A_write_uint32_to_all_same_val(TMC4361A_CLK_FREQ, 0x1312D00, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    }

    //TMC4361A_POS_COMP              0x32
    //TMC4361A_VIRT_STOP_LEFT        0x33
    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        tmc4361A_write_int32_to_individual(TMC4361A_VIRT_STOP_LEFT, gbsm_vstop_config[i].pos_l, i, hspi, tmc_status_from_spi);
    }

    //TMC4361A_VIRT_STOP_RIGHT       0x34
    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        tmc4361A_write_int32_to_individual(TMC4361A_VIRT_STOP_RIGHT, gbsm_vstop_config[i].pos_r, i, hspi, tmc_status_from_spi);
    }

    //TMC4361A_X_HOME                0x35
    //TMC4361A_X_LATCH_RD            0x36
    //TMC4361A_REV_CNT_RD            0x36
    //TMC4361A_X_RANGE_WR            0x36
    //TMC4361A_X_TARGET              0x37

    //TMC4361A_X_PIPE0               0x38
    //TMC4361A_X_PIPE1               0x39
    //TMC4361A_X_PIPE2               0x3A
    //TMC4361A_X_PIPE3               0x3B
    //TMC4361A_X_PIPE4               0x3C
    //TMC4361A_X_PIPE5               0x3D
    //TMC4361A_X_PIPE6               0x3E
    //TMC4361A_X_PIPE7               0x3F

    //TMC4361A_SH_REG0               0x40
    //TMC4361A_SH_REG1               0x41
    //TMC4361A_SH_REG2               0x42
    //TMC4361A_SH_REG3               0x43
    //TMC4361A_SH_REG4               0x44
    //TMC4361A_SH_REG5               0x45
    //TMC4361A_SH_REG6               0x46
    //TMC4361A_SH_REG7               0x47
    //TMC4361A_SH_REG8               0x48
    //TMC4361A_SH_REG9               0x49
    //TMC4361A_SH_REG10              0x4A
    //TMC4361A_SH_REG11              0x4B
    //TMC4361A_SH_REG12              0x4C
    //TMC4361A_SH_REG13              0x4D

    //TMC4361A_DFREEZE               0x4E
    //DFREEZE=0 = hard stop

    //TMC4361A_IFREEZE               0x4E
    tmc4361a_freeze_conf_set(hspi);

    //TMC4361A_CLK_GATING_REG        0x4F
    //TMC4361A_RESET_REG             0x4F

    //TMC4361A_ENC_POS               0x50
    //TMC4361A_ENC_LATCH_RD          0x51
    //TMC4361A_ENC_RESET_VAL_WR      0x51
    //TMC4361A_ENC_POS_DEV_RD        0x52
    //TMC4361A_CL_TR_TOLERANCE_WR    0x52
    //TMC4361A_ENC_POS_DEV_TOL_WR    0x53
    //TMC4361A_ENC_IN_RES_WR         0x54
    //TMC4361A_ENC_CONST_RD          0x54
    //TMC4361A_MANUAL_ENC_CONST0     0x54
    //TMC4361A_ENC_OUT_RES           0x55
    //TMC4361A_SER_CLK_IN_HIGH_WR    0x56
    //TMC4361A_SER_CLK_IN_LOW_WR     0x56
    //TMC4361A_SSI_IN_CLK_DELAY_WR   0x57
    //TMC4361A_SSI_IN_WTIME_WR       0x57
    //TMC4361A_SER_PTIME_WR          0x58

    //TMC4361A_CL_OFFSET             0x59
    //TMC4361A_PID_P_WR              0x5A
    //TMC4361A_CL_VMAX_CALC_P_WR     0x5A
    //TMC4361A_PID_VEL_RD            0x5A
    //TMC4361A_PID_I_WR              0x5B
    //TMC4361A_CL_VMAX_CALC_I_WR     0x5B
    //TMC4361A_PID_ISUM_RD           0x5B
    //TMC4361A_PID_D_WR              0x5C
    //TMC4361A_CL_DELTA_P_WR         0x5C
    //TMC4361A_PID_I_CLIP_WR         0x5D
    //TMC4361A_PID_D_CLKDIV_WR       0x5D
    //TMC4361A_PID_E_RD              0x5D
    //TMC4361A_PID_DV_CLIP_WR        0x5E
    //TMC4361A_PID_TOLERANCE_WR      0x5F
    //TMC4361A_CL_TOLERANCE_WR       0x5F

    //TMC4361A_FS_VEL_WR             0x60
    //TMC4361A_DC_VEL_WR             0x60
    //TMC4361A_CL_VMIN_EMF_WR        0x60
    //TMC4361A_DC_TIME_WR            0x61
    //TMC4361A_DC_SG_WR              0x61
    //TMC4361A_DC_BLKTIME_WR         0x61
    //TMC4361A_CL_VADD_EMF           0x61
    //TMC4361A_DC_LSPTM_WR           0x62
    //TMC4361A_ENC_VEL_ZERO_WR       0x62
    //TMC4361A_ENC_VMEAN_WAIT_WR     0x63
    //TMC4361A_ENC_VMEAN_FILTER_WR   0x63
    //TMC4361A_ENC_VMEAN_INT_WR      0x63
    //TMC4361A_SER_ENC_VARIATION_WR  0x63
    //TMC4361A_CL_CYCLE_WR           0x63
    //TMC4361A_SYNCHRO_SET           0x64
    //TMC4361A_V_ENC_RD              0x65
    //TMC4361A_V_ENC_MEAN_RD         0x66
    //TMC4361A_VSTALL_LIMIT_WR       0x67
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_VSTALL_LIMIT_WR, 1000, SM_NUM_DRIVES, hspi, tmc_status_from_spi);

    //TMC4361A_ADDR_TO_ENC           0x68
    //TMC4361A_DATA_TO_ENC           0x69
    //TMC4361A_ADDR_FROM_ENC         0x6A
    //TMC4361A_DATA_FROM_ENC         0x6B
    //TMC4361A_COVER_LOW_WR          0x6C
    //TMC4361A_POLLING_STATUS_RD     0x6C
    //TMC4361A_COVER_HIGH_WR         0x6D
    //TMC4361A_POLLING_REG_WR        0x6D
    //TMC4361A_COVER_DRV_LOW_RD      0x6E
    //TMC4361A_COVER_DRV_HIGH_RD     0x6F

    //TMC4361A_MSLUT_0_WR            0x70
    //TMC4361A_MSLUT_1_WR            0x71
    //TMC4361A_MSLUT_2_WR            0x72
    //TMC4361A_MSLUT_3_WR            0x73
    //TMC4361A_MSLUT_4_WR            0x74
    //TMC4361A_MSLUT_5_WR            0x75
    //TMC4361A_MSLUT_6_WR            0x76
    //TMC4361A_MSLUT_7_WR            0x77
    //TMC4361A_MSLUTSEL_WR           0x78
    //TMC4361A_MSCNT_RD              0x79
    //TMC4361A_MSOFFSET_WR           0x79
    //TMC4361A_CURRENTA_RD           0x7A
    //TMC4361A_CURRENTB_RD           0x7A
    //TMC4361A_CURRENTA_SPI_RD       0x7B
    //TMC4361A_CURRENTB_SPI_RD       0x7B
    //TMC4361A_TZEROWAIT_WR          0x7B
    //TMC4361A_SCALE_PARAM_RD        0x7C
    //TMC4361A_CIRCULAR_DEC_WR       0x7C
    //TMC4361A_ENC_COMP_XOFFSET      0x7D
    //TMC4361A_ENC_COMP_YOFFSET      0x7D
    //TMC4361A_ENC_COMP_AMPL         0x7D
    //TMC4361A_START_SIN_WR          0x7E
    //TMC4361A_START_SIN90_120_WR    0x7E
    //TMC4361A_DAC_OFFSET_WR         0x7E
    //TMC4361A_VERSION_NO_RD         0x7F


}

void drv_send_start_trigger(void) {
    //trigger TMC
    gpioWrite(PIN_TMC4361A_START, 0);
    gpioWrite(PIN_TMC4361A_START, 1);
}

/**
 * @brief umbrella function to reset both TMC5160 & TMC4361A
 * @param hspi pigpio spi handle
 * @return E_NOT_STOPPER if any motor is moving, E_SUCCESS otherwise
 *
 *@warning Does not reset unless motors are stopped
 */
static gberror_t resetMotorDrivers(const unsigned int *hspi) {

    int32_t vactual[SM_NUM_DRIVES];
    tmc4361A_read_int32_all(TMC4361A_VACTUAL, vactual, SM_NUM_DRIVES, hspi);

    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        if (vactual[i] != 0) {
            LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: driver [%d] has velocity [%d]", i, vactual[i]);
//                        return E_NOT_STOPPED;
        }
    }

    drv_reset_tmc5160();
    drv_reset_tmc4361a(true, hspi);

    return E_SUCCESS;
}


#define PI_SPI_CHANNEL 0
//fclk / 4 maximum
#define PI_SPI_SPEED 2500000
#define PI_SPI_MODE 0b100011

/*
 * 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
 * b  b  b  b  b  b  R  T  n  n  n  n   W  A u2 u1 u0 p2 p1 p0  m  m
 * 0  0  0  0  0  0  0  0  0  0  0  0   0  0 0  0  0  0  0  0   1  1
 *
 * mm mode
 * Mode POL PHA
 * 0    0   0
 * 1    0   1
 * 2    1   0
 * 3    1   1

 * MISO MOSI SCLK CE0 CE1 CE2
 * Main SPI  9   10   11    8   7   -
 * Aux SPI  19   20   21   18  17  16

 * px is 0 if CEx is active low (default) and 1 for active high.
 * ux is 0 if the CEx GPIO is reserved for SPI (default) and 1 otherwise. (if not using standard cs pins)
 * A is 0 for the standard SPI device, 1 for the auxiliary SPI.
 * W is 0 if the device is not 3-wire, 1 if the device is 3-wire. Standard SPI device only. (normal SPI is 4 wire)
 * nnnn defines the number of bytes (0-15) to write before switching the MOSI line to MISO to read data. This field is ignored if W is not set. Standard SPI device only.
 * T is 1 if the least significant bit is transmitted on MOSI first, the default (0) shifts the most significant bit out first. Auxiliary SPI device only.
 * R is 1 if the least significant bit is received on MISO first, the default (0) receives the most significant bit first. Auxiliary SPI device only.
 * bbbbbb defines the word size in bits (0-32). The default (0) sets 8 bits per word. Auxiliary SPI device only.
 * The spi_read, spi_write, and spi_xfer functions transfer data packed into 1, 2, or 4 bytes according to the word size in bits.
 * For bits 1-8 there will be one byte per character. For bits 9-16 there will be two bytes per character. For bits 17-32 there will be four bytes per character.
 * E.g. 32 12-bit words will be transferred in 64 bytes
*/

/**
 * @brief Initialises SPI interface and gpio pins
 * @param hspi pigpio spi handle
 * @return E_INIT_FAILURE if it fails
 */
static gberror_t drv_init_spi_and_pins(unsigned int *hspi) {
    gberror_t grc = E_GENERAL_FAILURE;
    int pigpio_ver;


    pigpio_ver = gpioInitialise();

    if (pigpio_ver < 0) {
        return E_INIT_FAILURE;
    } else {
        LL_INFO(GBSM_GEN_LOG_EN, "GBSM: PIGPIO version [%d]", pigpio_ver);
    }

    //initialise any configured gpio pins to their IN/OUT function
    io_init();

    //Configure signal PIGPIO handler to catch terminate signals so that we can clean up GPIO pin assignment
    gpioSetSignalFunc(SIGINT, gpio_terminate_pigpio);
    gpioSetSignalFunc(SIGHUP, gpio_terminate_pigpio);
    gpioSetSignalFunc(SIGTERM, gpio_terminate_pigpio);


    UM_INFO(GBSM_UM_EN, "GBSM: PI_SPI_CHANNEL [%d]", PI_SPI_CHANNEL);
    UM_INFO(GBSM_UM_EN, "GBSM: PI_SPI_SPEED [%d]", PI_SPI_SPEED);

    //Open spi channel
    *hspi = (unsigned) spiOpen(PI_SPI_CHANNEL, PI_SPI_SPEED, PI_SPI_MODE);

    //todo-crit this is reapplied from gpio.c ?
    gpioSetMode(PIN_TMC4361A_SPI_CS, PI_OUTPUT);
    gpioWrite(PIN_TMC4361A_SPI_CS, 1); // TMC4361A: SPI CS is low active

    if (*hspi < 0) {
        switch (*hspi) {
            case (unsigned) PI_BAD_SPI_CHANNEL:
                LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: SPI initialisation failed [bad SPI channel]");
                break;
            case (unsigned) PI_BAD_SPI_SPEED:
                LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: SPI initialisation failed [bad SPI speed]");
                break;
            case (unsigned) PI_BAD_FLAGS:
                LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: SPI initialisation failed [bad flags]");
                break;
            case (unsigned) PI_NO_AUX_SPI:
                LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: SPI initialisation failed [no aux SPI]");
                break;
            case (unsigned) PI_SPI_OPEN_FAILED:
                LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: SPI initialisation failed [SPI open failed]");
                break;
            default:
                LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: SPI initialisation failed [unknown error]");
        }
        return E_INIT_FAILURE;
    } else {
        LL_INFO(GBSM_GEN_LOG_EN, "GBSM: SPI handle [%d]", *hspi);
        UM_INFO(GBSM_UM_EN, "GBSM: SPI initialised successfully");
    }

    //configure GPIO pins for TMC5160 and TMC4361
    gpio_configure_TMC5160_and_TMC4361_pins();

    // Reset the TMC4361A & TMC5160
    grc = resetMotorDrivers(hspi);

    while (grc != E_SUCCESS) {
        if (grc == E_NOT_STOPPED) {
            UM_WARN(GBSM_UM_EN, "GBSM: Motor driver reset failed. Tried to reset motor drivers whilst motion is occurring. Will retry");
        } else {
            UM_WARN(GBSM_UM_EN, "GBSM: Motor driver reset failed. Will retry");
        }
        gpioDelay(10000000);
        grc = resetMotorDrivers(hspi);
    }
    return E_SUCCESS;

}


static bool test_tmc4361a(void) {

    int rand_xactual = rand();

    int read_xactual[SM_NUM_DRIVES];

    tmc4361A_write_int32_to_all_same_val(TMC4361A_XACTUAL, rand_xactual, SM_NUM_DRIVES, &hspi1, tmc_status_from_spi);

    tmc4361A_read_int32_all(TMC4361A_XACTUAL, read_xactual, SM_NUM_DRIVES, &hspi1);

    tmc4361A_write_int32_to_all_same_val(TMC4361A_XACTUAL, 0, SM_NUM_DRIVES, &hspi1, tmc_status_from_spi);


    int num_matching = 0;

    for (int i = 0; i < SM_NUM_DRIVES; i++) {

        if (read_xactual[i] == rand_xactual) {
            num_matching++;
        }

    }

    if (num_matching == SM_NUM_DRIVES) {
        return true;
    }
    return false;


}


/**
 * @brief Initialises SPI interface and driver chips
 * @return gberror_t
 */
gberror_t drv_init_spi_and_drivers(void) {
    gberror_t grc;

    grc = drv_init_spi_and_pins(&hspi1);
    if (grc != E_SUCCESS) {
        return grc;
    }

    drv_init_tmc4361a(&hspi1);
//must come after the tmc4631 config
    drv_init_tmc5160_covers(&hspi1);
    return grc;
}


gberror_t drv_reset_and_enable(void) {
    gberror_t grc = E_SUCCESS;

    // Reset the TMC4361A & TMC5160
    grc = resetMotorDrivers(&hspi1);

    drv_init_tmc4361a(&hspi1);
//must come after the tmc4631 config
    drv_init_tmc5160_covers(&hspi1);

    return grc;
}


bool check_for_error(status_flag_t *status) {
    //todo-crit add etxra conditiobs
//    return ( status->open_load_a || status->overtemperature_shutdown || status->overtemperature_warning || status->overtemperature_warning || status->stall || status->stallguard_active);

    return ((bool) status->overtemperature_shutdown || (bool) status->overtemperature_warning || (bool) status->overtemperature_warning || (bool) status->stall || (bool) status->stallguard_active);

}

/**
 * @brief runs state machine for a single motor
 * @param ctrlwrd
 * @param state
 * @param status
 */
static void drive_process_state(uint16_t ctrlwrd, cia_state_t *state, status_flag_t *status, bool en) {

    bool error = check_for_error(status);

    if (error) {
        *state = CIA_FAULT_REACTION_ACTIVE;
        return;
    }

    //if we don't have a ready to enable bit, state is always Not Ready To Switch On
    if (!en) {
        LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: Drive not enabled");
        *state = CIA_NOT_READY_TO_SWITCH_ON;
        return;
    }

    switch (*state) {
        case CIA_FAULT_REACTION_ACTIVE:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_FAULT_RESET && !check_for_error(status)) {
                *state = CIA_FAULT;
            } else {
                *state = CIA_FAULT_REACTION_ACTIVE;
            }
            break;
        case CIA_FAULT:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_FAULT_RESET) {

                *state = CIA_SWITCH_ON_DISABLED;
            } else {
                *state = CIA_FAULT;
            }
            break;
        case CIA_NOT_READY_TO_SWITCH_ON:
//            if (el7031_drive_state_bits[drive].ready_to_enable){
//                *state = CIA_SWITCH_ON_DISABLED;
//            } else {
//                *state = CIA_NOT_READY_TO_SWITCH_ON;
//            }
            *state = CIA_SWITCH_ON_DISABLED;

            break;
        case CIA_SWITCH_ON_DISABLED:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_SHUTDOWN) {
                *state = CIA_READY_TO_SWITCH_ON;
            } else {
                *state = CIA_SWITCH_ON_DISABLED;
            }

            break;
        case CIA_READY_TO_SWITCH_ON:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_SWITCH_ON) {
                *state = CIA_SWITCHED_ON;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_VOLTAGE) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else {
                *state = CIA_READY_TO_SWITCH_ON;
            }
            break;
        case CIA_SWITCHED_ON:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_ENABLE_OPERATION) {
                *state = CIA_OPERATION_ENABLED;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_VOLTAGE) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_SHUTDOWN) {
                *state = CIA_READY_TO_SWITCH_ON;
            } else {
                *state = CIA_SWITCHED_ON;
            }

            break;
        case CIA_OPERATION_ENABLED:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_VOLTAGE) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_OPERATION) {
                *state = CIA_SWITCHED_ON;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_SHUTDOWN) {
                *state = CIA_READY_TO_SWITCH_ON;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_QUICK_STOP) {
                *state = CIA_QUICK_STOP_ACTIVE;
            } else {
                *state = CIA_OPERATION_ENABLED;
            }

            //if we are in Operation enabled and lose the ready bit on the drive ?
//            if (!el7031_drive_state_bits[drive].ready){
//                printf("B\n");
//                *state = CIA_FAULT_REACTION_ACTIVE;
//            }

            break;
        case CIA_QUICK_STOP_ACTIVE:
            if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_DISABLE_VOLTAGE) {
                *state = CIA_SWITCH_ON_DISABLED;
            } else if (cia_ctrlwrd_to_command(ctrlwrd) == CIA_ENABLE_OPERATION) {
                *state = CIA_OPERATION_ENABLED;
            } else {
                *state = CIA_QUICK_STOP_ACTIVE;
            }
            break;
        default:
            break;
    }

}

/**
 * @brief reads status information off the TMC4361a chips and records the state in the g_drv_status struct
 */
void drv_update_state(void) {
    static uint64_t ticks_in_fault_reaction_active[SM_NUM_DRIVES] = {0};

    bool trigger_reset = false;
#if FLAVOUR == PLATFORM_PI
    uint32_t tmc4361a_status_flag[SM_NUM_DRIVES] = {0};
    uint32_t tmc4361a_status_event[SM_NUM_DRIVES] = {0};


    tmc4361A_read_uint32_all(TMC4361A_STATUS, tmc4361a_status_flag, SM_NUM_DRIVES, &hspi1);
//    tmc4361A_read_uint32_all(TMC4361A_EVENTS, tmc4361a_status_event, SM_NUM_DRIVES, &hspi1);

    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        g_drv_status_flag[i] = flag_register_to_struct(tmc4361a_status_flag[i], i);
        g_drv_status_event[i] = event_register_to_struct(tmc4361a_status_event[i], i);
    }

//    printf("flags\n");
//    printBits(4, tmc4361a_status_flag);

//    printf("events\n");
//    printBits(4, tmc4361a_status_event);

#endif

#define TICKS_IN_FAULT_REACTION_ACTIVE_BEFORE_RESET 1000

    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        drive_process_state(g_drv_ctrlwrd[i], &g_drv_cia_state[i], &g_drv_status_flag[i], g_drv_enabled[i]);
//        printf("%u [%s]\n",i, cia_state_names[g_drv_cia_state[i]]);

        if (g_drv_cia_state[i] == CIA_FAULT_REACTION_ACTIVE) {
            ticks_in_fault_reaction_active[i]++;
        } else {
            ticks_in_fault_reaction_active[i] = 0;
            trigger_reset = false;
        }
        if (ticks_in_fault_reaction_active[i] > TICKS_IN_FAULT_REACTION_ACTIVE_BEFORE_RESET) {
            trigger_reset = true;
            printf("Want to trigger reset\n");
        }

    }

    if (trigger_reset) {
        drv_reset_and_enable();
    }


//    printf("drive:%u : control word:%s\n", drv_num, cia_command_names[cia_command_to_ctrlwrd(drv_ctrlwrd[drv_num])]);
}


void drv_set_ctrl_wrd(const uint16_t ctrl_wrd) {
    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        g_drv_ctrlwrd[i] = ctrl_wrd;
    }
}

void drv_get_stat_wrd(uint16_t *stat_wrd) {
    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        stat_wrd[i] = cia_state_to_statwrd(g_drv_cia_state[i]);
    }
}


void drv_set_setpos(const int32_t *setpos) {
    static int last_set_pos[SM_NUM_DRIVES];

    int setpos_to_apply_to_drive[SM_NUM_DRIVES];
    fixed_point_t vmax[SM_NUM_DRIVES] = {0};
    int32_t move_distance[SM_NUM_DRIVES] = {0};
    for (int i = 0; i < SM_NUM_DRIVES; i++) {

        if (g_drv_cia_state[i] != CIA_OPERATION_ENABLED) {
            setpos_to_apply_to_drive[i] = dpm_in->joint_actual_position[i];
            vmax[i] = 0;
        } else {
//            move_distance[i] = setpos[i] - dpm_in->joint_actual_position[i];
//vmax is abs in positioning mode
            move_distance[i] = setpos[i] - last_set_pos[i];
            if (move_distance[i] == 0) {
                setpos_to_apply_to_drive[i] = dpm_in->joint_actual_position[i];
                vmax[i] = 0;
                break;
            } else {
                setpos_to_apply_to_drive[i] = setpos[i];
                vmax[i] = float_to_fixed(fabs((double) 1000 * ((double) move_distance[i] / (double) BUS_CYCLE_TIME)));
//                printBits(4, &vmax[i]);
            }
            printf("Drive [%d] Vmax [%f] Distance [%d] Setpos [%d] Actpos [%d]\n", i, fixed_to_float(vmax[i]), move_distance[i], setpos[i], dpm_in->joint_actual_position[i]);
            last_set_pos[i] = setpos[i];
        }
    }


#if FLAVOUR == PLATFORM_PI


    tmc4361A_write_int32_to_all_different_vals(TMC4361A_VMAX, vmax, SM_NUM_DRIVES, &hspi1, tmc_status_from_spi);
    tmc4361A_write_int32_to_all_different_vals(TMC4361A_X_TARGET, setpos_to_apply_to_drive, SM_NUM_DRIVES, &hspi1, tmc_status_from_spi);

#else
    //loop back position on LINUX

    for (int i = 0; i < SM_NUM_DRIVES; i++) {

        dpm_in->joint_actual_position[i] = setpos[i];

    }
#endif

}

/**
 * @brief get actpos with SPI read to chip
 * @param drv_num
 * @return int32_t *actpos
 */
void drv_get_actpos(int32_t *actpos) {
#if FLAVOUR == PLATFORM_PI
    tmc4361A_read_int32_all(TMC4361A_XACTUAL, actpos, SM_NUM_DRIVES, &hspi1);

//    for (int i = 0; i < SM_NUM_DRIVES; i++) {
//        dpm_in->joint_actual_position[i]=actpos[i];
//    }
#else
    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        actpos[i] = dpm_in->joint_actual_position[i];
    }
#endif
}


void tmc4361a_terminate(void) {

    resetMotorDrivers(&hspi1);
}


static uint32_t *read_motor_currents(void) {
    uint32_t drvstatus[SM_NUM_DRIVES] = {0};
    static uint32_t cs_actual[SM_NUM_DRIVES] = {0};
    uint8_t spi_status;

    gpioDelay(1000);
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_DRVSTATUS, SM_NUM_DRIVES, &hspi1, &spi_status);
    gpioDelay(1000);
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_LOW_WR, 0, SM_NUM_DRIVES, &hspi1, &spi_status);
    gpioDelay(1000);
    tmc4361A_read_uint32_all(TMC4361A_COVER_DRV_LOW_RD, drvstatus, SM_NUM_DRIVES, &hspi1);

    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        cs_actual[i] = REGISTER_GET(drvstatus[0], TMC5160_CS_ACTUAL_MASK, TMC5160_CS_ACTUAL_SHIFT);
    }

    return cs_actual;

}


