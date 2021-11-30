
/**
 ******************************************************************************
 * @file           :  tmc4361a_general_conf.c
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
#include "log.h"

void tmc4361a_general_conf_set(const unsigned int *hspi) {
    /* GENERAL_CONF - default = 0x00006020 = 0b110 0000 0010 0000 (5, 13, 14 = stdny clock fwd & dir out pol switch)*/
    uint32_t tmc4361a_general_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {

        // GENERAL_CONF // Sets AACTUAL = ASTART or -ASTART at ramp start and in the case of VSTART ? 0
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_USE_ASTART_AND_VSTART_MASK, TMC4361A_USE_ASTART_AND_VSTART_SHIFT, 0);

        // GENERAL_CONF // Acceleration values are set directly as steps per clock cycle.
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_DIRECT_ACC_VAL_EN_MASK, TMC4361A_DIRECT_ACC_VAL_EN_SHIFT, 0);

        // GENERAL_CONF // Bow values are set directly as steps per clock cycle
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_DIRECT_BOW_VAL_EN_MASK, TMC4361A_DIRECT_BOW_VAL_EN_SHIFT, 0);

        // GENERAL_CONF // Inactive STPOUT polarity
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_STEP_INACTIVE_POL_MASK, TMC4361A_STEP_INACTIVE_POL_SHIFT, 0);

        // GENERAL_CONF // Every level change of STPOUT indicates a step
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_TOGGLE_STEP_MASK, TMC4361A_TOGGLE_STEP_SHIFT, 0);

        // GENERAL_CONF // DIROUT negative direction polarity
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_POL_DIR_OUT_MASK, TMC4361A_POL_DIR_OUT_SHIFT, 0);

        // GENERAL_CONF // External step control configuration
        // min.: 0, max.: 3, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_SDIN_MODE_MASK, TMC4361A_SDIN_MODE_SHIFT, 0);

        // GENERAL_CONF // DIRIN negative direction polarity
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_POL_DIR_IN_MASK, TMC4361A_POL_DIR_IN_SHIFT, 0);

        // GENERAL_CONF // STPIN/DIRIN input signals will manipulate XTARGET register value, the internal ramp generator is used
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_SD_INDIRECT_CONTROL_MASK, TMC4361A_SD_INDIRECT_CONTROL_SHIFT, 0);

        // GENERAL_CONF // Connected encoder type
        // min.: 0, max.: 3, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_SERIAL_ENC_IN_MODE_MASK, TMC4361A_SERIAL_ENC_IN_MODE_SHIFT, 0);

        /*default encoder is an incremental encoder */

        /*absolute SSI encoder is connected */

        if (gbsm_encoder_config[i].encoder_type == GBSM_ENCODER_ABS_SSI) {
            LL_INFO(GBSM_GEN_LOG_EN, "GBSM: Configuring ABS SSI encoder");
            tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_SERIAL_ENC_IN_MODE_MASK, TMC4361A_SERIAL_ENC_IN_MODE_SHIFT, 1);
        }

        /*absolute SPI encoder is connected */

        if (gbsm_encoder_config[i].encoder_type == GBSM_ENCODER_ABS_SPI) {
            LL_INFO(GBSM_GEN_LOG_EN, "GBSM: Configuring ABS SPI encoder");

            tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_SERIAL_ENC_IN_MODE_MASK, TMC4361A_SERIAL_ENC_IN_MODE_SHIFT, 3);
        }

        // GENERAL_CONF // Differential encoder interface inputs is disabled - enable encoders overall
        // min.: 0, max.: 1, default: 0

        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_DIFF_ENC_IN_DISABLE_MASK, TMC4361A_DIFF_ENC_IN_DISABLE_SHIFT, gbsm_encoder_config[i].enable);

        // GENERAL_CONF // STDBY_CLK pin assignment
        // min.: 0, max.: 3, default: 0

        //0 - Standby signal becomes forwarded with an active low level at STDBY_CLK output.
        //1 - Standby signal becomes forwarded with an active high level at STDBY_CLK output.
        //2 - STDBY_CLK passes ChopSync clock (TMC23x, TMC24x stepper motor drivers only).
        //3 - Internal clock is forwarded to STDBY_CLK output pin.

        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_STDBY_CLK_PIN_ASSIGNMENT_MASK, TMC4361A_STDBY_CLK_PIN_ASSIGNMENT_SHIFT, 3);

        // GENERAL_CONF // Active INTR pin polarity
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_INTR_POL_MASK, TMC4361A_INTR_POL_SHIFT, 0);

        // GENERAL_CONF // Inactive TARGET_REACHED pin polarity
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_INVERT_POL_TARGET_REACHED_MASK, TMC4361A_INVERT_POL_TARGET_REACHED_SHIFT, 0);

        // GENERAL_CONF // SPI output forwards fullsteps, if ?VACTUAL? > FS_VEL
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_FS_EN_MASK, TMC4361A_FS_EN_SHIFT, 0);

        // GENERAL_CONF // Fullsteps are forwarded via Step/Dir output also if fullstep operation is active
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_FS_SDOUT_MASK, TMC4361A_FS_SDOUT_SHIFT, 0);

        // GENERAL_CONF // dcStep mode
        // min.: 0, max.: 3, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_DCSTEP_MODE_MASK, TMC4361A_DCSTEP_MODE_SHIFT, 0);

        // GENERAL_CONF // Enabling Volatage controlled PWM
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_PWM_OUT_EN_MASK, TMC4361A_PWM_OUT_EN_SHIFT, 0);

        // GENERAL_CONF // SPI output is used as SSI encoder interface to forward absolute SSI encoder data
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_SERIAL_ENC_OUT_ENABLE_MASK, TMC4361A_SERIAL_ENC_OUT_ENABLE_SHIFT, 0);

        // GENERAL_CONF // Differential serial encoder output is disabled
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_SERIAL_ENC_OUT_DIFF_DISABLE_MASK, TMC4361A_SERIAL_ENC_OUT_DIFF_DISABLE_SHIFT, 0);

        // GENERAL_CONF // VACTUAL = VSTART and AACTUAL = ASTART after switching off direct external step control
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_AUTOMATIC_DIRECT_SDIN_SWITCH_OFF_MASK, TMC4361A_AUTOMATIC_DIRECT_SDIN_SWITCH_OFF_SHIFT, 0);

        // GENERAL_CONF // The register value of REV_CNT (#internal revolutions) is forwarded at register 0x36
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_CIRCULAR_CNT_AS_XLATCH_MASK, TMC4361A_CIRCULAR_CNT_AS_XLATCH_SHIFT, 0);

        // GENERAL_CONF // Reverse internal MSLUT direction
        // min.: 0, max.: 1, default: 0

        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_REVERSE_MOTOR_DIR_MASK, TMC4361A_REVERSE_MOTOR_DIR_SHIFT, gbsm_motor_directions[i]);

        // GENERAL_CONF // INTR and TARGET_REACHED are used as outputs with gated pull-up and / or pull-down functionality
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_INTR_TR_PU_PD_EN_MASK, TMC4361A_INTR_TR_PU_PD_EN_SHIFT, 0);


        // GENERAL_CONF // INTR output function is used as Wired-And
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_INTR_AS_WIRED_AND_MASK, TMC4361A_INTR_AS_WIRED_AND_SHIFT, 0);

        // GENERAL_CONF // TARGET_REACHED output function is used as Wired-And
        // min.: 0, max.: 1, default: 0
        tmc4361a_general_conf[i] = REGISTER_SET(tmc4361a_general_conf[i], TMC4361A_TR_AS_WIRED_AND_MASK, TMC4361A_TR_AS_WIRED_AND_SHIFT, 0);

    }
    /* write general configuration register - 0x00 */
    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_GENERAL_CONF, tmc4361a_general_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);

}

