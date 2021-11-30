
/**
 ******************************************************************************
 * @file           :  tmc4361a_freeze_conf.c
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

void tmc4361a_freeze_conf_set(const unsigned int *hspi) {
    /* FREEZE_CONF */
    uint32_t tmc4361a_freeze_conf[SM_NUM_DRIVES] = {0};


    for (int i = 0; i < SM_NUM_DRIVES; i++) {


// Freeze Registers // 0=hardstop; [delta_v per clock cycle]: DFREEZE[pps²]=DFREEZE/2^37*f_clk²
// min.: 0, max.: 16777215, default: 0
        tmc4361a_freeze_conf[i] = REGISTER_SET(tmc4361a_freeze_conf[i], TMC4361A_DFREEZE_MASK, TMC4361A_DFREEZE_SHIFT, 0);

// Freeze Registers // 0=last scaling value valid
// min.: 0, max.: 255, default: 0
        tmc4361a_freeze_conf[i] = REGISTER_SET(tmc4361a_freeze_conf[i], TMC4361A_IFREEZE_MASK,TMC4361A_IFREEZE_SHIFT , 0);

    }
    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_DFREEZE, tmc4361a_freeze_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
}