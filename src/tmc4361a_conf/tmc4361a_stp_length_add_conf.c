
/**
 ******************************************************************************
 * @file           :  tmc4361a_stp_length_add_conf.c
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

void tmc4361a_stp_length_add_conf_set(const unsigned int *hspi) {
    /* STP_LENGTH_ADD_CONF */
    uint32_t tmc4361a_stp_length_add_conf[SM_NUM_DRIVES] = {0};

    // STP_LENGTH=4, DIR_SETUP_TIME=6

    for (int i = 0; i < SM_NUM_DRIVES; i++) {

// STP_LENGTH_ADD / DIR_SETUP_TIME // Additional length [# clock cycles] for active step polarity to indicate an active output step at STPOUT
// min.: 0, max.: 65535, default: 0
        tmc4361a_stp_length_add_conf[i] = REGISTER_SET(tmc4361a_stp_length_add_conf[i], TMC4361A_STP_LENGTH_ADD_MASK, TMC4361A_STP_LENGTH_ADD_SHIFT, 4);

// STP_LENGTH_ADD / DIR_SETUP_TIME // Delay [# clock cycles] between DIROUT and STPOUT voltage level changes
// min.: 0, max.: 65535, default: 0
        tmc4361a_stp_length_add_conf[i] = REGISTER_SET(tmc4361a_stp_length_add_conf[i],TMC4361A_DIR_SETUP_TIME_MASK ,TMC4361A_DIR_SETUP_TIME_SHIFT , 6);

    }
    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_STP_LENGTH_ADD, tmc4361a_stp_length_add_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
}