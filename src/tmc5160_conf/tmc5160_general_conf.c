
/**
 ******************************************************************************
 * @file           :  tmc5160_general_conf.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "TMC5160_Fields.h"
#include "TMC5160_Register.h"
#include "TMC4361A_Register.h"
#include <stdint.h>
#include "std_defs_and_macros.h"
#include "drives.h"
#include "SPI_TMC.h"
#include "pigpio.h"

void tmc5160_general_conf_set(const unsigned int *hspi) {
    /* GENERAL_CONF - default = */
    uint32_t tmc5160_general_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {


        //0
        // GCONF //Zero crossing recalibration during driver disable (via DRV_ENN or via TOFF setting)
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_RECALIBRATE_MASK, TMC5160_RECALIBRATE_SHIFT, 0);

        //1
        // GCONF // Timeout for step execution until standstill detection
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_FASTSTANDSTILL_MASK, TMC5160_FASTSTANDSTILL_SHIFT, 0);

        //2
        // GCONF //StealthChop voltage PWM mode enabled (depending on velocity thresholds)
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_EN_PWM_MODE_MASK, TMC5160_EN_PWM_MODE_SHIFT, 1);

        //3
        // GCONF //Enable step input filtering for StealthChop optimization with external step source
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_MULTISTEP_FILT_MASK, TMC5160_MULTISTEP_FILT_SHIFT, 1);

        //4
        // GCONF //Inverse motor direction
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_SHAFT_MASK, TMC5160_SHAFT_SHIFT, 0);

        //5
        // GCONF //Enable DIAG0 active on driver errors
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_DIAG0_ERROR_ONLY_WITH_SD_MODE1_MASK, TMC5160_DIAG0_ERROR_ONLY_WITH_SD_MODE1_SHIFT, 0);

        //6
        // GCONF //Enable DIAG0 active on driver over temperature prewarning (otpw)
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_DIAG0_OTPW_ONLY_WITH_SD_MODE1_MASK, TMC5160_DIAG0_OTPW_ONLY_WITH_SD_MODE1_SHIFT, 0);

        //7
        // GCONF //Enable DIAG0 active on motor stall (set TCOOLTHRS before using this feature)
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_DIAG0_STALL_MASK, TMC5160_DIAG0_STALL_SHIFT, 0);

        //8
        // GCONF //Enable DIAG1 active on motor stall (set TCOOLTHRS before using this feature)
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_DIAG1_STALL_MASK, TMC5160_DIAG1_STALL_SHIFT, 0);

        //9
        // GCONF //Enable DIAG1 active on index position (microstep look up table position 0)
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_DIAG1_INDEX_MASK, TMC5160_DIAG1_INDEX_SHIFT, 0);

        //10
        // GCONF //Enable DIAG1 active when chopper is on (for the coil which is in the second half of the fullstep)
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_DIAG1_ONSTATE_MASK, TMC5160_DIAG1_ONSTATE_SHIFT, 0);

        //11
        // GCONF //Enable output toggle when steps are skipped in DcStep mode (increment of LOST_STEPS). Do not enable in conjunction with other DIAG1 options
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_DIAG1_STEPS_SKIPPED_MASK, TMC5160_DIAG1_STEPS_SKIPPED_SHIFT, 0);

        //12
        // GCONF //0: SWN_DIAG0 is open collector output (active low) //1: Enable SWN_DIAG0 push pull output (active high)
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_DIAG0_INT_PUSHPULL_MASK, TMC5160_DIAG0_INT_PUSHPULL_SHIFT, 0);

        //13
        // GCONF //0: SWP_DIAG1 is open collector output (active low) //1: Enable SWP_DIAG1 push pull output (active high)
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_DIAG1_POSCOMP_PUSHPULL_MASK, TMC5160_DIAG1_POSCOMP_PUSHPULL_SHIFT, 0);

        //14
        // GCONF //0: Hysteresis for step frequency comparison is 1/16 //1: Hysteresis for step frequency comparison is 1/32
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_SMALL_HYSTERESIS_MASK, TMC5160_SMALL_HYSTERESIS_SHIFT, 0);

        //15
        // GCONF //0: Normal operation //1: Emergency stop: ENCA_DCIN stops the sequencer when tied high (no steps become executed by the sequencer, motor goes to standstill state).
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_STOP_ENABLE_MASK, TMC5160_STOP_ENABLE_SHIFT, 0);

        //16
        // GCONF //0: Normal operation
        //1: Motor coil currents and polarity directly programmed via serial interface: Register XTARGET (0x2D) specifies signed coil A current (bits 8..0) and coil B current (bits 24..16). In this mode, the current is scaled by IHOLD setting. Velocity based current regulation of StealthChop is not available in this mode. The automatic StealthChop current regulation will work only for low stepper motor velocities.
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_DIRECT_MODE_MASK, TMC5160_DIRECT_MODE_SHIFT, 0);

        //17
        // GCONF //Enable analog test output on pin ENCN_DCO. IHOLD[1..0] selects the function of ENCN_DCO: 0…2: T120, DAC, VDDH
        // min.: 0, max.: 1, default: 0
        tmc5160_general_conf[i] = REGISTER_SET(tmc5160_general_conf[i], TMC5160_TEST_MODE_MASK, TMC5160_TEST_MODE_SHIFT, 0);

    }

    tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_GCONF | 0x80, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_COVER_LOW_WR, tmc5160_general_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    gpioDelay(1000); // COVER_DONE flag: ~90µs -> 1 ms more than enough
}