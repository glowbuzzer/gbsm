
/**
 ******************************************************************************
 * @file           :  tmc5160_ihold_irun_conf.c
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

void tmc5160_ihold_irun_conf_set(const unsigned int *hspi) {
    /* IHOLD_IRUN_CONF - default = */
    uint32_t tmc5160_ihold_irun_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {

        //0x00080F0A = 1000 0000 1111 0000 1010

// IHOLD_IRUN // Standstill current (0=1/32...31=32/32) In combination with stealthChop mode, setting IHOLD=0 allows to choose freewheeling or coil short circuit (passive braking) for motor stand still.
// min.: 0, max.: 31, default: 0
        tmc5160_ihold_irun_conf[i] = REGISTER_SET(tmc5160_ihold_irun_conf[i], TMC5160_IHOLD_MASK, TMC5160_IHOLD_SHIFT, 5);

// IHOLD_IRUN // Motor run current (0=1/32...31=32/32) Hint: Choose sense resistors in a way, that normal IRUN is 16 to 31 for best microstep performance.
// min.: 0, max.: 31, default: 0
        tmc5160_ihold_irun_conf[i] = REGISTER_SET(tmc5160_ihold_irun_conf[i],TMC5160_IRUN_MASK , TMC5160_IRUN_SHIFT, 10);

// IHOLD_IRUN // Controls the number of clock cycles for motor power down after standstill is detected (stst=1) and TPOWERDOWN has expired. The smooth transition avoids a motor jerk upon power down. 0:  instant power down 1..15:  Delay per current reduction step in multiple of 2^18 clocks
// min.: 0, max.: 15, default: 0

        tmc5160_ihold_irun_conf[i] = REGISTER_SET(tmc5160_ihold_irun_conf[i],TMC5160_IHOLDDELAY_MASK ,TMC5160_IHOLDDELAY_SHIFT , 8);

    }

    tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_IHOLD_IRUN | 0x80, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_COVER_LOW_WR, tmc5160_ihold_irun_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    gpioDelay(1000); // COVER_DONE flag: ~90µs -> 1 ms more than enough
}