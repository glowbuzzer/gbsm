
/**
 ******************************************************************************
 * @file           :  tmc4361a_current_conf.c
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

void tmc4361a_current_conf_set(const unsigned int *hspi) {
    /* CURRENT_CONF */
    uint32_t tmc4361a_current_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {


// CURRENT_CONF // Enabling hold current scaling during standstill
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i],TMC4361A_HOLD_CURRENT_SCALE_EN_MASK , TMC4361A_HOLD_CURRENT_SCALE_EN_SHIFT, 0);

// CURRENT_CONF // Enabling drive current scaling during motion
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i], TMC4361A_DRIVE_CURRENT_SCALE_EN_MASK,TMC4361A_DRIVE_CURRENT_SCALE_EN_SHIFT , 0);

// CURRENT_CONF // Enabling boost current during acceleration slopes
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i],TMC4361A_BOOST_CURRENT_ON_ACC_EN_MASK ,TMC4361A_BOOST_CURRENT_ON_ACC_EN_SHIFT , 0);

// CURRENT_CONF // Enabling boost current during deceleration slopes
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i], TMC4361A_BOOST_CURRENT_ON_DEC_EN_MASK,TMC4361A_BOOST_CURRENT_ON_DEC_EN_SHIFT , 0);

// CURRENT_CONF // Enabling boost current after ramp start
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i],TMC4361A_BOOST_CURRENT_AFTER_START_EN_MASK ,TMC4361A_BOOST_CURRENT_AFTER_START_EN_SHIFT , 0);

// CURRENT_CONF // Enabling 2nd drive current scaling during motion
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i],TMC4361A_SEC_DRIVE_CURRENT_SCALE_EN_MASK , TMC4361A_SEC_DRIVE_CURRENT_SCALE_EN_SHIFT, 0);

// CURRENT_CONF // Enabling freewheeling phase during standstill
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i],TMC4361A_FREEWHEELING_EN_MASK ,TMC4361A_FREEWHEELING_EN_SHIFT , 0);

// CURRENT_CONF // Enabling hold current scaling during standstill
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i], TMC4361A_HOLD_CURRENT_SCALE_EN_MASK, TMC4361A_HOLD_CURRENT_SCALE_EN_SHIFT, 0);

// CURRENT_CONF // Enabling drive current scaling during motion
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i], TMC4361A_DRIVE_CURRENT_SCALE_EN_MASK, TMC4361A_DRIVE_CURRENT_SCALE_EN_SHIFT, 0);

// CURRENT_CONF // Enabling boost current during acceleration slopes
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i],TMC4361A_BOOST_CURRENT_ON_ACC_EN_MASK ,TMC4361A_BOOST_CURRENT_ON_ACC_EN_SHIFT , 0);

// CURRENT_CONF // Enabling boost current during deceleration slopes
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i],TMC4361A_BOOST_CURRENT_ON_DEC_EN_MASK , TMC4361A_BOOST_CURRENT_ON_DEC_EN_SHIFT, 0);

// CURRENT_CONF // Enabling boost current after ramp start
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i],TMC4361A_BOOST_CURRENT_AFTER_START_EN_MASK ,TMC4361A_BOOST_CURRENT_AFTER_START_EN_SHIFT , 0);

// CURRENT_CONF // Enabling 2nd drive current scaling during motion
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i], TMC4361A_SEC_DRIVE_CURRENT_SCALE_EN_MASK,TMC4361A_SEC_DRIVE_CURRENT_SCALE_EN_SHIFT , 0);

// CURRENT_CONF // Enabling freewheeling phase during standstill
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i],TMC4361A_FREEWHEELING_EN_MASK ,TMC4361A_FREEWHEELING_EN_SHIFT , 0);

// CURRENT_CONF // Enabling closed loop current scaling, turn Off during calibration
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i],TMC4361A_CLOSED_LOOP_SCALE_EN_MASK ,TMC4361A_CLOSED_LOOP_SCALE_EN_SHIFT , 0);

// CURRENT_CONF // Enabling hold current scaling during standstill
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i],TMC4361A_HOLD_CURRENT_SCALE_EN_MASK , TMC4361A_HOLD_CURRENT_SCALE_EN_SHIFT, 0);

// CURRENT_CONF // Enabling PWM scaling
// min.: 0, max.: 1, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i], TMC4361A_PWM_SCALE_EN_MASK,TMC4361A_PWM_SCALE_EN_SHIFT , 0);

// CURRENT_CONF // PWM amplitude at VACTUAL=0
// min.: 0, max.: 65535, default: 0
        tmc4361a_current_conf[i] = REGISTER_SET(tmc4361a_current_conf[i],TMC4361A_PWM_AMPL_MASK ,TMC4361A_PWM_AMPL_SHIFT , 0);
    }

    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_CURRENT_CONF, tmc4361a_current_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
}