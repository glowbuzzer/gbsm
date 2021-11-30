
/**
 ******************************************************************************
 * @file           :  tmc4361a_step_conf.c
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

void tmc4361a_step_conf_set(const unsigned int *hspi) {
    /* STEP_CONF */
    uint32_t tmc4361a_step_conf[SM_NUM_DRIVES] = {0};

//    0x00FB0C80 = 1111 1011 0000 1100 1000 0000

    for (int i = 0; i < SM_NUM_DRIVES; i++) {




// STEP_CONF // microstep per revolution
// min.: 0, max.: 8, default: 0

#if GBSM_MICROSTEPS == 256
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_MSTEP_PER_FS_MASK , TMC4361A_MSTEP_PER_FS_SHIFT, 0);
#endif
#if GBSM_MICROSTEPS == 128
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_MSTEP_PER_FS_MASK , TMC4361A_MSTEP_PER_FS_SHIFT, 1);
#endif
#if GBSM_MICROSTEPS == 64
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_MSTEP_PER_FS_MASK , TMC4361A_MSTEP_PER_FS_SHIFT, 2);
#endif
#if GBSM_MICROSTEPS == 32
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_MSTEP_PER_FS_MASK , TMC4361A_MSTEP_PER_FS_SHIFT, 3);
#endif
#if GBSM_MICROSTEPS == 16
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_MSTEP_PER_FS_MASK , TMC4361A_MSTEP_PER_FS_SHIFT, 4);
#endif
#if GBSM_MICROSTEPS == 8
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_MSTEP_PER_FS_MASK , TMC4361A_MSTEP_PER_FS_SHIFT, 5);
#endif
#if GBSM_MICROSTEPS == 4
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_MSTEP_PER_FS_MASK , TMC4361A_MSTEP_PER_FS_SHIFT, 6);
#endif
#if GBSM_MICROSTEPS == 2
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_MSTEP_PER_FS_MASK , TMC4361A_MSTEP_PER_FS_SHIFT, 7);
#endif
#if GBSM_MICROSTEPS == 1
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_MSTEP_PER_FS_MASK , TMC4361A_MSTEP_PER_FS_SHIFT, 8);

#endif



// STEP_CONF // Fullsteps per motor axis revolution
// min.: 0, max.: 4095, default: 0
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_FS_PER_REV_MASK ,TMC4361A_FS_PER_REV_SHIFT , 200);


// STEP_CONF // EVENTS(30) considers Stallguard2 status - 16
// min.: 0, max.: 1, default: 0
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i], TMC4361A_SG_MASK,TMC4361A_SG_SHIFT , 1);

// STEP_CONF // EVENTS(30) considers Overtemperature shutdown - 17
// min.: 0, max.: 1, default: 0
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i], TMC4361A_OT_MASK, TMC4361A_OT_SHIFT, 1);

// STEP_CONF // EVENTS(30) considers Overtemperature warning - 18
// min.: 0, max.: 1, default: 0
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_OTPW_MASK ,TMC4361A_OTPW_SHIFT , 0);

// STEP_CONF // EVENTS(30) considers Short2Gnd coilA - 19
// min.: 0, max.: 1, default: 0
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_S2GA_MASK ,TMC4361A_S2GA_SHIFT , 1);

// STEP_CONF // EVENTS(30) considers Short2Gnd coilB - 20
// min.: 0, max.: 1, default: 0
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_S2GB_MASK ,TMC4361A_S2GB_SHIFT , 1);

// STEP_CONF // EVENTS(30) considers Open load coilA - 21
// min.: 0, max.: 1, default: 0
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_OLA_MASK ,TMC4361A_OLA_SHIFT , 1);

// STEP_CONF // EVENTS(30) considers Open load coilB - 22
// min.: 0, max.: 1, default: 0
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_OLB_MASK ,TMC4361A_OLB_SHIFT , 1);

// STEP_CONF // EVENTS(30) considers Standstill - 23
// min.: 0, max.: 1, default: 0
        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i], TMC4361A_STST_MASK,TMC4361A_STST_SHIFT , 1);

//// STEP_CONF // EVENTS(30) considers calculated Stallguard status
//// min.: 0, max.: 1, default: 0
//        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_SG_MASK , TMC4361A_SG_SHIFT, 0);
//
//// STEP_CONF // EVENTS(30) considers Undervoltage status
//// min.: 0, max.: 1, default: 0
//        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i], TMC4361A_UV_SF_MASK, TMC4361A_UV_SF_SHIFT, 0);
//
//// STEP_CONF // EVENTS(30) considers Undervoltage status
//// min.: 0, max.: 1, default: 0
//        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_UV_SF_MASK ,TMC4361A_UV_SF_SHIFT , 0);
//
//// STEP_CONF // EVENTS(30) considers Overtemperature shutdown
//// min.: 0, max.: 1, default: 0
//        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i], TMC4361A_OT_MASK, TMC4361A_OT_SHIFT, 0);
//
//// STEP_CONF // EVENTS(30) considers Overtemperature warning
//// min.: 0, max.: 1, default: 0
//        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i], TMC4361A_OTPW_MASK, TMC4361A_OTPW_SHIFT, 0);
//
//// STEP_CONF // EVENTS(30) considers Overcurrent bridge A
//// min.: 0, max.: 1, default: 0
//        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_OCA_MASK , TMC4361A_OCA_SHIFT, 0);
//
//// STEP_CONF // EVENTS(30) considers Overcurrent bridge B
//// min.: 0, max.: 1, default: 0
//        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i], TMC4361A_OCB_MASK, TMC4361A_OCB_SHIFT, 0);
//
//// STEP_CONF // EVENTS(30) considers Open load coilA
//// min.: 0, max.: 1, default: 0
//        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_OLA_MASK , TMC4361A_OLA_SHIFT, 0);
//
//// STEP_CONF // EVENTS(30) considers Open load coilB
//// min.: 0, max.: 1, default: 0
//        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i], TMC4361A_OLB_MASK, TMC4361A_OLB_SHIFT, 0);
////todo-crit
//// STEP_CONF // EVENTS(30) considers Overcurrent highSide
//// min.: 0, max.: 1, default: 0
//        tmc4361a_step_conf[i] = REGISTER_SET(tmc4361a_step_conf[i],TMC4361A_OCHS_MASK , TMC4361A_OCHS_SHIFT, 0);
    }

    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_STEP_CONF, tmc4361a_step_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
}