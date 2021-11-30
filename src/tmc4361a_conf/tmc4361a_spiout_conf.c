
/**
 ******************************************************************************
 * @file           :  tmc4361a_spiout_conf.c
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

void tmc4361a_spiout_conf_set(const unsigned int *hspi) {
    /* SPIOUT_CONF */
    uint32_t tmc4361a_spiout_conf[SM_NUM_DRIVES] = {0};

    /* SPI_OUTPUT_FORMAT=12, DISABLE_POLLING=1, COVER_DATA_LENGTH=0, SPI_OUT_LOW_TIME=4, SPI_OUT_HIGH_TIME=4, SPI_OUT_BLOCK_TIME=8 */

//todo-crit
//WARING POLLING ENABLED!!!!
//    tmc4361A_write_uint32_to_all_same_val(TMC4361A_SPIOUT_CONF, 0x8440104C, SM_NUM_DRIVES, hspi, tmc_status_from_spi);

//this works 09.10
//    tmc4361A_write_uint32_to_all_same_val(TMC4361A_SPIOUT_CONF, 0b10000100010000000001000000001100, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
//                                                                      10000100010000000001000001001100
//get reads working with ^

    //              31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    //0x8440004C =  1  0  0  0  0  1  0  0  0  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  1  0  0  1  1  0  0
//0b10000100010000000000000001001100
//0b10000100010000000000000100001100
    //bits 0:3 = 4 == actual unsigned scaling factor sent to spi

    //bits 19:13 = cover datagram length

    //bit 6 =  0 = Permanent transfer of polling datagrams to check driver status. 1 = No transfer of polling datagrams.
//todo
// poll for reset of 5160
//It is a common approach that the microcontroller continuously rewrites register values for TMC21x0 to respond to possible voltage drops at the VS pin of TMC21x0, which – if they occur – prompt an internal register reset, by design.
//TMC4361A provides an option to continuously rewrite five configuration registers of TMC21x0, which take off workload from the microcontroller.
//These registers are: GCONF 0x00, IHOLD_IRUN 0x10, CHOPCONF 0x6C, COOLCONF 0x6D, and DCCTRL 0x6E.
//In order to activate automatic continuous streaming of TMC21x0 cover datagrams, do as follows:
//Action:
//Set autorepeat_cover_en = 1 (bit7 of SPI_OUT_CONF register 0x04).


    for (int i = 0; i < SM_NUM_DRIVES; i++) {


        // SPI_OUT_CONF // Selection of connected driver/DAC - 0
        // min.: 0, max.: 0, default: 0
//        SPI output interface is connected with a TMC2130 / TMC2160 stepper motor driver. Only configuration data is transferred to the stepper motor driver. S/D output interface provides steps.
        tmc4361a_spiout_conf[i] = REGISTER_SET(tmc4361a_spiout_conf[i], TMC4361A_SPI_OUTPUT_FORMAT_MASK,TMC4361A_SPI_OUTPUT_FORMAT_SHIFT , 12);


        // SPI_OUT_CONF // Transfer of scaling values in scaling values alter - 5
        // min.: 0, max.: 1, default: 0
        tmc4361a_spiout_conf[i] = REGISTER_SET(tmc4361a_spiout_conf[i], TMC4361A_SCALE_VALE_TRANSFER_EN_MASK, TMC4361A_SCALE_VALE_TRANSFER_EN_SHIFT, 0);

        // SPI_OUT_CONF // No transfer of polling datagrams - 6
        // min.: 0, max.: 1, default: 0
        tmc4361a_spiout_conf[i] = REGISTER_SET(tmc4361a_spiout_conf[i],TMC4361A_DISABLE_POLLING_MASK ,TMC4361A_DISABLE_POLLING_SHIFT , 0);

        // SPI_OUT_CONF // Continous streaming of once transferred cover datagrams - 7
        // min.: 0, max.: 1, default: 0
        tmc4361a_spiout_conf[i] = REGISTER_SET(tmc4361a_spiout_conf[i],TMC4361A_AUTOREPEAT_COVER_EN_MASK ,TMC4361A_AUTOREPEAT_COVER_EN_SHIFT , 0);

        // SPI_OUT_CONF // Exponent for idle state time between two polling datagrams - 8
        // min.: 0, max.: 15, default: 0
        tmc4361a_spiout_conf[i] = REGISTER_SET(tmc4361a_spiout_conf[i], TMC4361A_POLL_BLOCK_EXP_MASK, TMC4361A_POLL_BLOCK_EXP_SHIFT, 0);

        // SPI_OUT_CONF // COVER_DONE only set in case cover datagram is sent - 12
        // min.: 0, max.: 1, default: 0
        tmc4361a_spiout_conf[i] = REGISTER_SET(tmc4361a_spiout_conf[i], TMC4361A_COVER_DONE_ONLY_FOR_COVER_MASK,TMC4361A_COVER_DONE_ONLY_FOR_COVER_SHIFT , 1);

        // SPI_OUT_CONF // Datagram length. Set to 0 for TMCx drivers - 13
        // min.: 0, max.: 64, default: 0
        tmc4361a_spiout_conf[i] = REGISTER_SET(tmc4361a_spiout_conf[i], TMC4361A_COVER_DATA_LENGTH_MASK, TMC4361A_COVER_DATA_LENGTH_SHIFT, 0);

        // SPI_OUT_CONF // SPI output clock low level duration - 20:23 (4)
        // min.: 0, max.: 15, default: 0
        tmc4361a_spiout_conf[i] = REGISTER_SET(tmc4361a_spiout_conf[i],TMC4361A_SPI_OUT_LOW_TIME_MASK ,TMC4361A_SPI_OUT_LOW_TIME_SHIFT , 4);

        // SPI_OUT_CONF // SPI output clock high level duration - 24:27 (4)
        // min.: 0, max.: 15, default: 0
        tmc4361a_spiout_conf[i] = REGISTER_SET(tmc4361a_spiout_conf[i],TMC4361A_SPI_OUT_HIGH_TIME_MASK , TMC4361A_SPI_OUT_HIGH_TIME_SHIFT, 4);

        // SPI_OUT_CONF // SPI output transfer idle state duration - 28:31 (4)
        // min.: 0, max.: 15, default: 0
        tmc4361a_spiout_conf[i] = REGISTER_SET(tmc4361a_spiout_conf[i],TMC4361A_SPI_OUT_IDLE_TIME_MASK , TMC4361A_SPI_OUT_IDLE_TIME_SHIFT, 8U);

//           31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
//        0b  1  0  0  0  0  1  0  0  0  1  0  0  0  0  0  0  0  0  0  1  0  0 0 0 0 0 0 0 1 1 0 0

//LOW_TIME = 0b100 = 4
//HIGH_TIME = 0b100 = 4
//IDLE_TIME = 0b1000 = 8

    }

    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_SPIOUT_CONF, tmc4361a_spiout_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
}