
/**
 ******************************************************************************
 * @file           :  tmc4361a_scale_values_conf.c
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

void tmc4361a_scale_values_conf_set(const unsigned int *hspi) {
    /* SCALE_VALUES_CONF */
    uint32_t tmc4361a_scale_values_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {


// SCALE_VALUES // Boost current scaling value
// min.: 0, max.: 255, default: 0
        tmc4361a_scale_values_conf[i] = REGISTER_SET(tmc4361a_scale_values_conf[i],TMC4361A_BOOST_SCALE_VAL_MASK ,TMC4361A_BOOST_SCALE_VAL_SHIFT , 0xff);

// SCALE_VALUES // 1st drive current scaling value
// min.: 0, max.: 255, default: 0
        tmc4361a_scale_values_conf[i] = REGISTER_SET(tmc4361a_scale_values_conf[i],TMC4361A_DRV1_SCALE_VAL_MASK ,TMC4361A_DRV1_SCALE_VAL_SHIFT , 0xff);

        // SCALE_VALUES // Start point to increase current scaling value towards CL_IMAX
// min.: 0, max.: 255, default: 0
        tmc4361a_scale_values_conf[i] = REGISTER_SET(tmc4361a_scale_values_conf[i], TMC4361A_CL_START_UP_MASK,TMC4361A_CL_START_UP_SHIFT , 0xff);


// SCALE_VALUES // hold current scaling value
// min.: 0, max.: 255, default: 0
        tmc4361a_scale_values_conf[i] = REGISTER_SET(tmc4361a_scale_values_conf[i], TMC4361A_HOLD_SCALE_VAL_MASK, TMC4361A_HOLD_SCALE_VAL_SHIFT, 0xff);


    }

    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_SCALE_VALUES, tmc4361a_scale_values_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
}