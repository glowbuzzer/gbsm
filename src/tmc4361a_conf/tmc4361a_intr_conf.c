
/**
 ******************************************************************************
 * @file           :  tmc4361a_intr_conf.c
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

void tmc4361a_intr_conf_set(const unsigned int *hspi) {
    /* INTR_CONF */

    /* Event selection for INTR output:
     * All Event bits of EVENTS register 0x0E that are selected here (=1) are ORed with interrupt event register set:
     * if any of the selected events is active, an interrupt at INTR is generated.
    */

    uint32_t tmc4361a_intr_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {


// EVENTS // XACTUAL has matched XTARGET
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_TARGET_REACHED_MASK ,TMC4361A_TARGET_REACHED_SHIFT , 0);

// EVENTS // XACTUAL has matched POS_COMP
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_POS_COMP_REACHED_MASK ,TMC4361A_POS_COMP_REACHED_SHIFT , 0);

// EVENTS // VACTUAL has reached VMAX
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_VEL_REACHED_MASK ,TMC4361A_VEL_REACHED_SHIFT , 0);

// EVENTS // VEL_STATE_F = 00 was triggered
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i], TMC4361A_VEL_STATE_00_MASK,TMC4361A_VEL_STATE_00_SHIFT , 0);

// EVENTS // VEL_STATE_F = 01 was triggered
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i], TMC4361A_VEL_STATE_01_MASK,TMC4361A_VEL_STATE_01_SHIFT , 0);

// EVENTS // VEL_STATE_F = 10 was triggered
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_VEL_STATE_10_MASK ,TMC4361A_VEL_STATE_10_SHIFT , 0);

// EVENTS // RAMP_STATE_F = 00 was triggered
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i], TMC4361A_RAMP_STATE_00_MASK,TMC4361A_RAMP_STATE_00_SHIFT , 0);

// EVENTS // RAMP_STATE_F = 01 was triggered
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_RAMP_STATE_01_MASK ,TMC4361A_RAMP_STATE_01_SHIFT , 0);

// EVENTS // RAMP_STATE_F = 10 was triggered
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_RAMP_STATE_10_MASK ,TMC4361A_RAMP_STATE_10_SHIFT , 0);

// EVENTS // abs(VACTUAL) > VBREAK
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_MAX_PHASE_TRAP_MASK ,TMC4361A_MAX_PHASE_TRAP_SHIFT , 0);

// EVENTS // NFREEZE was switched to low level
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i], TMC4361A_FROZEN_MASK, TMC4361A_FROZEN_SHIFT, 0);

// EVENTS // STOPL was triggered
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i], TMC4361A_STOPL_EVENT_MASK,TMC4361A_STOPL_EVENT_SHIFT , 0);

// EVENTS // STOPR was triggered
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i], TMC4361A_STOPR_EVENT_MASK,TMC4361A_STOPR_EVENT_SHIFT , 0);

// EVENTS // Left virtual switch was triggered
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_VSTOPL_ACTIVE_MASK ,TMC4361A_VSTOPL_ACTIVE_SHIFT , 0);

// EVENTS // Right virtual switch was triggered
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_VSTOPL_ACTIVE_MASK ,TMC4361A_VSTOPL_ACTIVE_SHIFT , 0);

// EVENTS // Unmatched HOME_REF polarity
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i], TMC4361A_HOME_ERROR_MASK, TMC4361A_HOME_ERROR_SHIFT, 0);

// EVENTS // Homing process finished or X_LATCH was rewritten
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_XLATCH_DONE_MASK , TMC4361A_XLATCH_DONE_SHIFT, 0);

// EVENTS // Fullstep motion was activated
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_FS_ACTIVE_MASK ,TMC4361A_FS_ACTIVE_SHIFT , 0);

// EVENTS // Mismatch between ENC_POS and XACTUAL has exceeded ENC_POS_DEV_TOL
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_ENC_FAIL_MASK , TMC4361A_ENC_FAIL_SHIFT, 0);

// EVENTS // N event was triggered
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i], TMC4361A_N_ACTIVE_MASK,TMC4361A_N_ACTIVE_SHIFT , 0);

// EVENTS // ENC_LATCH was rewritten
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_ENC_DONE_MASK ,TMC4361A_ENC_DONE_SHIFT , 0);

// EVENTS // Serial encoder data fail
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_SER_ENC_DATA_FAIL_MASK , TMC4361A_SER_ENC_DATA_FAIL_SHIFT, 0);

// EVENTS // Conf data from serial SPI encoderwas received
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_SER_DATA_DONE_MASK ,TMC4361A_SER_DATA_DONE_SHIFT , 0);

// EVENTS // One or more of the four serial encoder flags were set
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_SERIAL_ENC_FLAGS_MASK , TMC4361A_SERIAL_ENC_FLAGS_SHIFT, 0);

// EVENTS // SPI datagram received from driver
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_COVER_DONE_MASK ,TMC4361A_COVER_DONE_SHIFT , 0);

// EVENTS // Encoder velocity has reached 0
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_ENC_VEL0_MASK ,TMC4361A_ENC_VEL0_SHIFT , 0);

// EVENTS // Closed-loop commutation angle has reached maximum value
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i], TMC4361A_CL_MAX_MASK,TMC4361A_CL_MAX_SHIFT , 0);

// EVENTS // Closed-loop deviation has reached inner limit
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_CL_FIT_MASK ,TMC4361A_CL_FIT_SHIFT , 0);

// EVENTS // Motor stall detected. Motor was stopped
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i], TMC4361A_STOP_ON_STALL_MASK, TMC4361A_STOP_ON_STALL_SHIFT, 0);

// EVENTS // One or more of the selected motor event were triggered
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_MOTOR_EV_MASK , TMC4361A_MOTOR_EV_SHIFT, 0);

// EVENTS // Motion controller was reset
// min.: 0, max.: 1, default: 0
        tmc4361a_intr_conf[i] = REGISTER_SET(tmc4361a_intr_conf[i],TMC4361A_RST_EV_MASK ,TMC4361A_RST_EV_SHIFT , 0);
        }
    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_INTR_CONF, tmc4361a_intr_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
}