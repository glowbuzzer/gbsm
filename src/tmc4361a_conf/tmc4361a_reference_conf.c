
/**
 ******************************************************************************
 * @file           :  tmc4361a_reference_conf.c
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

void tmc4361a_reference_conf_set(const unsigned int *hspi) {
    /* REFERENCE_CONF */
    uint32_t tmc4361a_reference_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {

        // REFERENCE_CONF // Enabling STOPL reference pin
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_STOP_LEFT_EN_MASK, TMC4361A_STOP_LEFT_EN_SHIFT, (unsigned)gbsm_enable_phystop_l[i]);


        // REFERENCE_CONF // Enabling STOPR reference pin
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_STOP_RIGHT_EN_MASK, TMC4361A_STOP_RIGHT_EN_SHIFT, (unsigned)gbsm_enable_phystop_r[i]);

        // REFERENCE_CONF // Active STOPL polarity
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_POL_STOP_LEFT_MASK, TMC4361A_POL_STOP_LEFT_SHIFT, 0);

        // REFERENCE_CONF // Active STOPR polarity
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_POL_STOP_RIGHT_MASK, TMC4361A_POL_STOP_RIGHT_SHIFT, 0);

        // REFERENCE_CONF // Inverts STOPL/STOPR
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_INVERT_STOP_DIRECTION_MASK, TMC4361A_INVERT_STOP_DIRECTION_SHIFT, 0);

        // REFERENCE_CONF // Enabling soft stop
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_SOFT_STOP_EN_MASK, TMC4361A_SOFT_STOP_EN_SHIFT, 0);

        // REFERENCE_CONF // Enabling position limit VIRT_STOP_LEFT
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_VIRTUAL_LEFT_LIMIT_EN_MASK, TMC4361A_VIRTUAL_LEFT_LIMIT_EN_SHIFT, (unsigned) gbsm_vstop_config[i].enable_l);

        // REFERENCE_CONF // Enabling position limit VIRT_STOP_RIGHT
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_VIRTUAL_RIGHT_LIMIT_EN_MASK, TMC4361A_VIRTUAL_RIGHT_LIMIT_EN_SHIFT,  (unsigned) gbsm_vstop_config[i].enable_r);

        // REFERENCE_CONF // Virtual StopMode
        // min.: 1, max.: 2, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_VIRT_STOP_MODE_MASK, TMC4361A_VIRT_STOP_MODE_SHIFT, 0);

        // REFERENCE_CONF // X_LATCH = XACTUAL is stored in the case STOPL becomes inactive
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_LATCH_X_ON_INACTIVE_L_MASK, TMC4361A_LATCH_X_ON_INACTIVE_L_SHIFT, 0);

        // REFERENCE_CONF // X_LATCH = XACTUAL is stored in the case STOPL becomes active
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_LATCH_X_ON_ACTIVE_L_MASK, TMC4361A_LATCH_X_ON_ACTIVE_L_SHIFT, 0);

        // REFERENCE_CONF // X_LATCH = XACTUAL is stored in the case STOPR becomes inactive
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_LATCH_X_ON_INACTIVE_R_MASK, TMC4361A_LATCH_X_ON_INACTIVE_R_SHIFT, 0);

        // REFERENCE_CONF // X_LATCH = XACTUAL is stored in the case STOPR becomes active
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_LATCH_X_ON_ACTIVE_R_MASK, TMC4361A_LATCH_X_ON_ACTIVE_R_SHIFT, 0);

        // REFERENCE_CONF // STOPL input signal is also the HOME position
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_STOP_LEFT_IS_HOME_MASK, TMC4361A_STOP_LEFT_IS_HOME_SHIFT, 0);

        //#define TMC4361A_STOP_LEFT_IS_HOME_MASK                 0x8000U // REFERENCE_CONF // STOPR input signal is also the HOME position
        //#define TMC4361A_STOP_LEFT_IS_HOME_SHIFT                15U // min.: 0, max.: 1, default: 0

        // REFERENCE_CONF // Home event
        // min.: 0, max.: 13, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_HOME_EVENT_MASK, TMC4361A_HOME_EVENT_SHIFT, 0);

        // REFERENCE_CONF // Storage of XACTUAL as X_HOME at next regular home event. This switch is reset after an executed homing. An XLATCH_DONE event will be also released then
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_START_HOME_TRACKING_MASK, TMC4361A_START_HOME_TRACKING_SHIFT, 0);

        // REFERENCE_CONF // Set XACTUAL = 0 after XTARGET has been reached. The next ramp starts immediately
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_CLR_POS_AT_TARGET_MASK, TMC4361A_CLR_POS_AT_TARGET_SHIFT, 0);

        // REFERENCE_CONF // Range of XACTUAL is limited by X_RANGE: -X_RANGE = XACTUAL = X_RANGE - 1
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_CIRCULAR_MOVEMENT_EN_MASK, TMC4361A_CIRCULAR_MOVEMENT_EN_SHIFT, 0);

        // REFERENCE_CONF // TR pin output assignment
        // min.: 0, max.: 3, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_POS_COMP_OUTPUT_MASK, TMC4361A_POS_COMP_OUTPUT_SHIFT, 0);

        // REFERENCE_CONF // POS_COMP vs. XACTUAL (=0) or POS_COMP vs. ENC_POS (=1)
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_POS_COMP_SOURCE_MASK, TMC4361A_POS_COMP_SOURCE_SHIFT, 0);

        // REFERENCE_CONF // SPI and S/D output interface stops motion in case of an stall event (hard stop)
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_STOP_ON_STALL_MASK, TMC4361A_STOP_ON_STALL_SHIFT, 0);

        // REFERENCE_CONF // Enabling free motion after stop_on_stall event
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_DRV_AFTER_STALL_MASK, TMC4361A_DRV_AFTER_STALL_SHIFT, 0);

        // REFERENCE_CONF // POS_COMP_REACHED based on XACTUAL/ENC_POS vs.:
        // min.: 0, max.: 3, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_MODIFIED_POS_COPARE_MASK, TMC4361A_MODIFIED_POS_COPARE_SHIFT, 0);

        // REFERENCE_CONF // SPI output interface sends automatically cover datagrams when VACTUAL crosses SPI_SWITCH_VEL
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_AUTOMATIC_COVER_MASK, TMC4361A_AUTOMATIC_COVER_SHIFT, 0);

        // REFERENCE_CONF // Range of ENC_POS is limited by X_RANGE: -X_RANGE = XACTUAL = X_RANGE - 1
        // min.: 0, max.: 1, default: 0
        tmc4361a_reference_conf[i] = REGISTER_SET(tmc4361a_reference_conf[i], TMC4361A_CIRCULAR_ENC_EN_MASK, TMC4361A_CIRCULAR_ENC_EN_SHIFT, 0);


    }

    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_REFERENCE_CONF, tmc4361a_reference_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
}