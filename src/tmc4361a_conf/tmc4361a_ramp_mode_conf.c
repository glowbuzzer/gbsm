
/**
 ******************************************************************************
 * @file           :  tmc4361a_ramp_mode_conf.c
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
#include "std_utils.h"

void tmc4361a_ramp_mode_conf_set(const unsigned int *hspi) {
//    /* RAMP_MODE_CONF */
    uint32_t tmc4361a_ramp_mode_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        // RAMPMODE // Operation mode
        // min.: 0, max.: 1, default: 0
        tmc4361a_ramp_mode_conf[i] = REGISTER_SET(tmc4361a_ramp_mode_conf[i],TMC4361A_OPERATION_MODE_MASK ,TMC4361A_OPERATION_MODE_SHIFT , 1);

        // RAMPMODE // Ramp profile
        // min.: 0, max.: 2, default: 0
        tmc4361a_ramp_mode_conf[i] = REGISTER_SET(tmc4361a_ramp_mode_conf[i], TMC4361A_RAMP_PROFILE_MASK, TMC4361A_RAMP_PROFILE_SHIFT, 0);
    }
//    printBits(4, &tmc4361a_ramp_mode_conf[0]);

    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_RAMPMODE, tmc4361a_ramp_mode_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
//    tmc4361A_write_uint32_to_all_same_val(TMC4361A_RAMPMODE, 4, SM_NUM_DRIVES, hspi, tmc_status_from_spi);


}