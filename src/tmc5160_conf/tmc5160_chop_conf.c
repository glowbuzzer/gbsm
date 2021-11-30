
/**
 ******************************************************************************
 * @file           :  tmc5160_chop_conf.c
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

void tmc5160_chop_conf_set(const unsigned int *hspi) {
    /* CHOP_CONF - default = */
    uint32_t tmc5160_chop_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {


//    // TOFF=3, HSTRT=4, HEND=1, TBL=2, CHM=0 (spreadCycle™) // 0x000100C3 = 0b1 0000 0000 1100 0011
// CHOPCONF // off time and driver enable

//0:3
// CHOPCONF // off time and driver enable
// min.: 0, max.: 15, default: 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i],TMC5160_TOFF_MASK , TMC5160_TOFF_SHIFT, 3);

//4:6
// CHOPCONF // hysteresis start value added to HEND; Attention: Effective HEND+HSTRT less than or equal to 16. Hint: Hysteresis decrement is done each 16 clocks
// min.: 0, max.: 7, default:
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i],TMC5160_HSTRT_MASK ,TMC5160_HSTRT_SHIFT , 4);

        //7:10
// CHOPCONF // hysteresis low value
// min.: 0, max.: 15, default: 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i],TMC5160_HEND_MASK ,TMC5160_HEND_SHIFT , 1);

        //11
// CHOPCONF // MSB of fast decay time setting
// min.: 0, max.: 1, default: 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i], TMC5160_TFD_3_MASK,TMC5160_TFD_3_SHIFT , 0);

        //12
// CHOPCONF // fast decay mode
// min.: 0, max.: 1, default: 0 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i],TMC5160_DISFDCC_MASK ,TMC5160_DISFDCC_SHIFT , 0);

        //14
// CHOPCONF // chopper mode
// min.: 0, max.: 1, default: 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i],TMC5160_CHM_MASK ,TMC5160_CHM_SHIFT , 0);

        //15:16
// CHOPCONF // blank time select
// min.: 0, max.: 3, default: 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i], TMC5160_TBL_MASK, TMC5160_TBL_SHIFT, 2);

        //18
// CHOPCONF // high velocity fullstep selection
// min.: 0, max.: 1, default: 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i],TMC5160_VHIGHFS_MASK ,TMC5160_VHIGHFS_SHIFT , 0);

        //19
// CHOPCONF // high velocity chopper mode
// min.: 0, max.: 1, default: 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i], TMC5160_VHIGHCHM_MASK,TMC5160_VHIGHCHM_SHIFT , 0);

        //20:23
// CHOPCONF // Passive fast decay time TPFD allows dampening of motor mid-range resonances. Passive fast decay time setting controls duration of the fast decay phase inserted after bridge polarity change N CLK = 12 + 256*TPFD %0000: Disable %0001 ... %1111: 1 ... 15
// min.: 0, max.: 15, default: 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i],TMC5160_TPFD_MASK , TMC5160_TPFD_SHIFT, 0);

        //24:27
// CHOPCONF // Micro step resolution; The resolution gives the number of microstep entries per sine quarter wave. The driver automatically uses microstep positions which result in a symmetrical wave, when choosing a lower microstep resolution.
// min.: 0, max.: 8, default: 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i],TMC5160_MRES_MASK , TMC5160_MRES_SHIFT, 0);

        //28
// CHOPCONF // interpolation to 256 microsteps
// min.: 0, max.: 1, default: 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i],TMC5160_INTPOL_MASK , TMC5160_INTPOL_SHIFT, 0);

        //29
// CHOPCONF // enable double edge step pulses
// min.: 0, max.: 1, default: 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i], TMC5160_DEDGE_MASK,TMC5160_DEDGE_SHIFT , 0);

//30
// CHOPCONF // short to GND protection disable
// min.: 0, max.: 1, default: 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i],TMC5160_DISS2G_MASK ,TMC5160_DISS2G_SHIFT , 0);

        //31
// CHOPCONF // short to supply protection disable
// min.: 0, max.: 1, default: 0
        tmc5160_chop_conf[i] = REGISTER_SET(tmc5160_chop_conf[i],TMC5160_DISS2VS_MASK ,TMC5160_DISS2VS_SHIFT , 0);


    }

    tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_CHOPCONF | 0x80, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_COVER_LOW_WR, tmc5160_chop_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    gpioDelay(1000); // COVER_DONE flag: ~90µs -> 1 ms more than enough
}
