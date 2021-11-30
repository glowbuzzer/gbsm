
/**
 ******************************************************************************
 * @file           :  tmc5160_cool_conf.c
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

void tmc5160_cool_conf_set(const unsigned int *hspi) {
    /* Cool_CONF - default = */
    uint32_t tmc5160_cool_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {

        //0:3
        // COOLCONF // minimum stallGuard2 value - If the stallGuard2 result falls below SEMIN*32, the motor current becomes increased to reduce motor load angle. 0: smart current control coolStep off
        // min.: 0, max.: 15, default: 0
        tmc5160_cool_conf[i] = REGISTER_SET(tmc5160_cool_conf[i],TMC5160_SEMIN_MASK , TMC5160_SEMIN_SHIFT, 5);


        //5:7
        // COOLCONF // Current increment steps per measured stallGuard2 value
        // min.: 0, max.: 3, default: 0
        tmc5160_cool_conf[i] = REGISTER_SET(tmc5160_cool_conf[i],TMC5160_SEUP_MASK ,TMC5160_SEUP_SHIFT , 0);

        //8:11
        // COOLCONF // stallGuard2 hysteresis value - If the stallGuard2 result is equal to or above (SEMIN+SEMAX+1)*32, the motor current becomes decreased to save energy.
        // min.: 0, max.: 15, default: 0
        tmc5160_cool_conf[i] = REGISTER_SET(tmc5160_cool_conf[i],TMC5160_SEMAX_MASK ,TMC5160_SEMAX_SHIFT , 2);

        //13:14
        // COOLCONF // Current down step speed; For each n stallGuard2 values decrease by one
        // min.: 0, max.: 3, default: 0
        tmc5160_cool_conf[i] = REGISTER_SET(tmc5160_cool_conf[i],TMC5160_SEDN_MASK , TMC5160_SEDN_SHIFT, 1);

        //15
        // COOLCONF // minimum current for smart current control
        // min.: 0, max.: 1, default: 0
        tmc5160_cool_conf[i] = REGISTER_SET(tmc5160_cool_conf[i],TMC5160_SEIMIN_MASK ,TMC5160_SEIMIN_SHIFT , 0);

        //16:22
        // COOLCONF // stallGuard2 threshold value This signed value controls stallGuard2 level for stall output and sets the optimum measurement range for readout. A lower value gives a higher sensitivity. Zero is the starting value working with most motors. A higher value makes stallGuard2 less sensitive and requires more torque to indicate a stall.
        // min.: -64, max.: 63, default: 0
        tmc5160_cool_conf[i] = REGISTER_SET(tmc5160_cool_conf[i],TMC5160_SGT_MASK ,TMC5160_SGT_SHIFT , 12);

        //24
        // COOLCONF // stallGuard2 filter enable
        // min.: 0, max.: 1, default: 0

        tmc5160_cool_conf[i] = REGISTER_SET(tmc5160_cool_conf[i],TMC5160_SFILT_MASK , TMC5160_SFILT_SHIFT, 0);




    }

    tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_COOLCONF | 0x80, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_COVER_LOW_WR, tmc5160_cool_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
    gpioDelay(1000); // COVER_DONE flag: ~90µs -> 1 ms more than enough
}
