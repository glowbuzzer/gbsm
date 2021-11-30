
/**
 ******************************************************************************
 * @file           :  tmc4361a_enc_in_data_conf.c
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

void tmc4361a_enc_in_data_conf_set(const unsigned int *hspi) {
    /* ENC_IN_DATA_CONF */
    uint32_t tmc4361a_enc_in_data_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {


// ENC_IN_DATA // Number of angle data bits per revolution - 1
// min.: 0, max.: 31, default: 0
        tmc4361a_enc_in_data_conf[i] = REGISTER_SET(tmc4361a_enc_in_data_conf[i],TMC4361A_SINGLE_TURN_RES_MASK , TMC4361A_SINGLE_TURN_RES_SHIFT, 0);

// ENC_IN_DATA // Number of data bits of revolution cnt - 1
// min.: 0, max.: 31, default: 0
        tmc4361a_enc_in_data_conf[i] = REGISTER_SET(tmc4361a_enc_in_data_conf[i],TMC4361A_MULTI_TURN_RES_MASK ,TMC4361A_MULTI_TURN_RES_SHIFT , 0);

// ENC_IN_DATA // Number of status bits
// min.: 0, max.: 3, default: 0
        tmc4361a_enc_in_data_conf[i] = REGISTER_SET(tmc4361a_enc_in_data_conf[i], TMC4361A_STATUS_BIT_CNT_MASK, TMC4361A_STATUS_BIT_CNT_SHIFT, 0);

// ENC_IN_DATA // Number of address bits within one SPI datagram
// min.: 0, max.: 256, default: 0
        tmc4361a_enc_in_data_conf[i] = REGISTER_SET(tmc4361a_enc_in_data_conf[i], TMC4361A_SERIAL_ADDR_BITS_MASK,TMC4361A_SERIAL_ADDR_BITS_SHIFT , 0);

// ENC_IN_DATA // Number of data bits within one SPI datagram
// min.: 0, max.: 256, default: 0
        tmc4361a_enc_in_data_conf[i] = REGISTER_SET(tmc4361a_enc_in_data_conf[i],TMC4361A_SERIAL_DATA_BITS_MASK , TMC4361A_SERIAL_DATA_BITS_SHIFT, 0);

// ENC_IN_DATA // Number of angle data bits per revolution - 1
// min.: 0, max.: 31, default: 0
        tmc4361a_enc_in_data_conf[i] = REGISTER_SET(tmc4361a_enc_in_data_conf[i], TMC4361A_SINGLE_TURN_RES_MASK, TMC4361A_SINGLE_TURN_RES_SHIFT, 0);

// ENC_IN_DATA // Number of data bits of revolution cnt - 1
// min.: 0, max.: 31, default: 0
        tmc4361a_enc_in_data_conf[i] = REGISTER_SET(tmc4361a_enc_in_data_conf[i],TMC4361A_MULTI_TURN_RES_MASK ,TMC4361A_MULTI_TURN_RES_SHIFT , 0);

// ENC_IN_DATA // Number of status bits
// min.: 0, max.: 3, default: 0
        tmc4361a_enc_in_data_conf[i] = REGISTER_SET(tmc4361a_enc_in_data_conf[i],TMC4361A_STATUS_BIT_CNT_MASK ,TMC4361A_STATUS_BIT_CNT_SHIFT , 0);

// ENC_IN_DATA // Number of angle data bits per revolution - 1
// min.: 0, max.: 31, default: 0
        tmc4361a_enc_in_data_conf[i] = REGISTER_SET(tmc4361a_enc_in_data_conf[i],TMC4361A_SINGLE_TURN_RES_MASK , TMC4361A_SINGLE_TURN_RES_SHIFT, 0);

// ENC_IN_DATA // Number of data bits of revolution cnt - 1
// min.: 0, max.: 31, default: 0
        tmc4361a_enc_in_data_conf[i] = REGISTER_SET(tmc4361a_enc_in_data_conf[i],TMC4361A_MULTI_TURN_RES_MASK ,TMC4361A_MULTI_TURN_RES_SHIFT , 0);

// ENC_IN_DATA // Number of status bits
// min.: 0, max.: 3, default: 0
        tmc4361a_enc_in_data_conf[i] = REGISTER_SET(tmc4361a_enc_in_data_conf[i],TMC4361A_STATUS_BIT_CNT_MASK , TMC4361A_STATUS_BIT_CNT_SHIFT, 0);

// ENC_IN_DATA // Number of address bits within one SPI datagram
// min.: 0, max.: 256, default: 0
        tmc4361a_enc_in_data_conf[i] = REGISTER_SET(tmc4361a_enc_in_data_conf[i], TMC4361A_SERIAL_ADDR_BITS_MASK, TMC4361A_SERIAL_ADDR_BITS_SHIFT, 0);

// ENC_IN_DATA // Number of data bits within one SPI datagram
// min.: 0, max.: 256, default: 0
        tmc4361a_enc_in_data_conf[i] = REGISTER_SET(tmc4361a_enc_in_data_conf[i],TMC4361A_SERIAL_DATA_BITS_MASK ,TMC4361A_SERIAL_DATA_BITS_SHIFT , 0);

    }

    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_ENC_IN_DATA, tmc4361a_enc_in_data_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
}