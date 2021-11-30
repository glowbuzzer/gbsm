
/**
 ******************************************************************************
 * @file           :  tmc4361a_spi_status_selection_conf.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */



#include "TMC4361A_Fields.h"
#include "TMC4361A_Register.h"
#include <stdint.h>
#include "std_defs_and_macros.h"
#include "drives.h"
#include "SPI_TMC.h"

void tmc4361a_spi_status_selection_conf_set(const unsigned int *hspi) {
    /* SPI_STATUS_SELECTION_CONF - 0x82029805 = 0b1000 0010 0000 0010 1001 1000 0000 0101*/

    /* Event bits of EVENTS register 0x0E that are selected (=1) in this register are forwarded
     * to the eight status bits that are transferred with every SPI datagram (first eight bits from LSB are significant!)
    */


    /* Default:
     * 0: target reached
     * 2: vel reached
     * 11: stop l triggered
     * 12: stop r triggered
     * 15: home error
     * 17: fs active
     * 25: cover done
     * 31: reset triggered
     */

    /* Our config:
     * 3: VACTUAL==0
     * 10: FROZEN
     * 11: stop l triggered
     * 12: stop r triggered
     * 13: vstop l triggered
     * 14: vstop r triggered
     * 29: stop on stall
     * 31: reset triggered
     */


    uint32_t tmc4361a_spi_status_selection_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {


// EVENTS // XACTUAL has matched XTARGET - 0
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_TARGET_REACHED_MASK ,TMC4361A_TARGET_REACHED_SHIFT , 0);

// EVENTS // XACTUAL has matched POS_COMP - 1
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_POS_COMP_REACHED_MASK ,TMC4361A_POS_COMP_REACHED_SHIFT , 0);

// EVENTS // VACTUAL has reached VMAX - 2
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_VEL_REACHED_MASK ,TMC4361A_VEL_REACHED_SHIFT , 0);

// EVENTS // VEL_STATE_F = 00 was triggered - 3
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i], TMC4361A_VEL_STATE_00_MASK,TMC4361A_VEL_STATE_00_SHIFT , 1);

// EVENTS // VEL_STATE_F = 01 was triggered - 4
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i], TMC4361A_VEL_STATE_01_MASK,TMC4361A_VEL_STATE_01_SHIFT , 0);

// EVENTS // VEL_STATE_F = 10 was triggered - 5
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_VEL_STATE_10_MASK ,TMC4361A_VEL_STATE_10_SHIFT , 0);

// EVENTS // RAMP_STATE_F = 00 was triggered - 6
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i], TMC4361A_RAMP_STATE_00_MASK,TMC4361A_RAMP_STATE_00_SHIFT , 0);

// EVENTS // RAMP_STATE_F = 01 was triggered - 7
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_RAMP_STATE_01_MASK ,TMC4361A_RAMP_STATE_01_SHIFT , 0);

// EVENTS // RAMP_STATE_F = 10 was triggered - 8
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_RAMP_STATE_10_MASK ,TMC4361A_RAMP_STATE_10_SHIFT , 0);

// EVENTS // abs(VACTUAL) > VBREAK - 9
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_MAX_PHASE_TRAP_MASK ,TMC4361A_MAX_PHASE_TRAP_SHIFT , 0);

// EVENTS // NFREEZE was switched to low level - 10
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i], TMC4361A_FROZEN_MASK, TMC4361A_FROZEN_SHIFT, 1);

// EVENTS // STOPL was triggered - 11
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i], TMC4361A_STOPL_EVENT_MASK,TMC4361A_STOPL_EVENT_SHIFT , 1);

// EVENTS // STOPR was triggered - 12
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i], TMC4361A_STOPR_EVENT_MASK,TMC4361A_STOPR_EVENT_SHIFT , 1);

// EVENTS // Left virtual switch was triggered - 13
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_VSTOPL_ACTIVE_MASK ,TMC4361A_VSTOPL_ACTIVE_SHIFT , 1);

// EVENTS // Right virtual switch was triggered - 14
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_VSTOPL_ACTIVE_MASK ,TMC4361A_VSTOPL_ACTIVE_SHIFT , 1);

// EVENTS // Unmatched HOME_REF polarity - 15
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i], TMC4361A_HOME_ERROR_MASK, TMC4361A_HOME_ERROR_SHIFT, 0);

// EVENTS // Homing process finished or X_LATCH was rewritten - 16
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_XLATCH_DONE_MASK , TMC4361A_XLATCH_DONE_SHIFT, 0);

// EVENTS // Fullstep motion was activated - 17
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_FS_ACTIVE_MASK ,TMC4361A_FS_ACTIVE_SHIFT , 0);

// EVENTS // Mismatch between ENC_POS and XACTUAL has exceeded ENC_POS_DEV_TOL - 18
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_ENC_FAIL_MASK , TMC4361A_ENC_FAIL_SHIFT, 0);

// EVENTS // N event was triggered - 19
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i], TMC4361A_N_ACTIVE_MASK,TMC4361A_N_ACTIVE_SHIFT , 0);

// EVENTS // ENC_LATCH was rewritten - 20
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_ENC_DONE_MASK ,TMC4361A_ENC_DONE_SHIFT , 0);

// EVENTS // Serial encoder data fail - 21
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_SER_ENC_DATA_FAIL_MASK , TMC4361A_SER_ENC_DATA_FAIL_SHIFT, 0);

        //22 is reserved

// EVENTS // Conf data from serial SPI encoderwas received - 23
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_SER_DATA_DONE_MASK ,TMC4361A_SER_DATA_DONE_SHIFT , 0);

// EVENTS // One or more of the four serial encoder flags were set - 24
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_SERIAL_ENC_FLAGS_MASK , TMC4361A_SERIAL_ENC_FLAGS_SHIFT, 0);

// EVENTS // SPI datagram received from driver - 25
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_COVER_DONE_MASK ,TMC4361A_COVER_DONE_SHIFT , 0);

// EVENTS // Encoder velocity has reached 0 - 26
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_ENC_VEL0_MASK ,TMC4361A_ENC_VEL0_SHIFT , 0);

// EVENTS // Closed-loop commutation angle has reached maximum value - 27
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i], TMC4361A_CL_MAX_MASK,TMC4361A_CL_MAX_SHIFT , 0);

// EVENTS // Closed-loop deviation has reached inner limit - 28
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_CL_FIT_MASK ,TMC4361A_CL_FIT_SHIFT , 0);

// EVENTS // Motor stall detected. Motor was stopped - 29
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i], TMC4361A_STOP_ON_STALL_MASK, TMC4361A_STOP_ON_STALL_SHIFT, 1);

// EVENTS // One or more of the selected motor event were triggered - 30
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_MOTOR_EV_MASK , TMC4361A_MOTOR_EV_SHIFT, 0);

// EVENTS // Motion controller was reset - 31
// min.: 0, max.: 1, default: 0
        tmc4361a_spi_status_selection_conf[i] = REGISTER_SET(tmc4361a_spi_status_selection_conf[i],TMC4361A_RST_EV_MASK ,TMC4361A_RST_EV_SHIFT , 1);


    }

    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_SPI_STATUS_SELECTION, tmc4361a_spi_status_selection_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
}