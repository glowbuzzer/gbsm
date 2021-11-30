
/**
 ******************************************************************************
 * @file           :  tmc4361a_input_filt_conf.c
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

void tmc4361a_input_filt_conf_set(const unsigned int *hspi) {
    /* INPUT_FILT_CONF */
    uint32_t tmc4361a_input_filt_conf[SM_NUM_DRIVES] = {0};

    // SR_ENC_IN=2, FILT_L_ENC_IN=2, SR_REF=0, FILT_L_REF=0, SR_S=4, FILT_L_S=5, SR_ENC_OUT=0, FILT_L_ENC_OUT=0

    for (int i = 0; i < SM_NUM_DRIVES; i++) {

        // INPUT_FILT_CONF // SR of Encoder input group: A, ANEG, B, BNEG, N, NNEG
        // min.: 0, max.: 7, default: 0
        tmc4361a_input_filt_conf[i] = REGISTER_SET(tmc4361a_input_filt_conf[i], TMC4361A_SR_ENC_IN_MASK, TMC4361A_SR_ENC_IN_SHIFT, 0);

        // INPUT_FILT_CONF // FILT_L of Encoder input group
        // min.: 0, max.: 7, default: 0
        tmc4361a_input_filt_conf[i] = REGISTER_SET(tmc4361a_input_filt_conf[i],TMC4361A_FILT_L_ENC_IN_MASK ,TMC4361A_FILT_L_ENC_IN_SHIFT , 0);

        // INPUT_FILT_CONF // Choose encoder input filter group for SD input pins
        // min.: 0, max.: 1, default: 0
        tmc4361a_input_filt_conf[i] = REGISTER_SET(tmc4361a_input_filt_conf[i], TMC4361A_SD_FILT0_MASK, TMC4361A_SD_FILT0_SHIFT, 0);

        // INPUT_FILT_CONF // SR of reference input group: STOPL, HOME_REF, STOPL
        // min.: 0, max.: 7, default: 0
        tmc4361a_input_filt_conf[i] = REGISTER_SET(tmc4361a_input_filt_conf[i],TMC4361A_SR_REF_MASK , TMC4361A_SR_REF_SHIFT, 0);

        // INPUT_FILT_CONF // FILT_L of reference input group
        // min.: 0, max.: 7, default: 0
        tmc4361a_input_filt_conf[i] = REGISTER_SET(tmc4361a_input_filt_conf[i], TMC4361A_FILT_L_REF_MASK, TMC4361A_FILT_L_REF_SHIFT, 0);

        // INPUT_FILT_CONF // Choose reference filter group for SD input pins
        // min.: 0, max.: 1, default: 0
        tmc4361a_input_filt_conf[i] = REGISTER_SET(tmc4361a_input_filt_conf[i], TMC4361A_SD_FILT1_MASK,TMC4361A_SD_FILT1_SHIFT , 0);

        // INPUT_FILT_CONF // SR of START input pin
        // min.: 0, max.: 7, default: 0
        tmc4361a_input_filt_conf[i] = REGISTER_SET(tmc4361a_input_filt_conf[i],TMC4361A_SR_S_MASK ,TMC4361A_SR_S_SHIFT , 0);

        // INPUT_FILT_CONF // FILT_L of START input pin
        // min.: 0, max.: 7, default: 0
        tmc4361a_input_filt_conf[i] = REGISTER_SET(tmc4361a_input_filt_conf[i],TMC4361A_FILT_L_S_MASK ,TMC4361A_FILT_L_S_SHIFT , 0);

        // INPUT_FILT_CONF // Choose START input pin filter for SD input pins
        // min.: 0, max.: 1, default: 0
        tmc4361a_input_filt_conf[i] = REGISTER_SET(tmc4361a_input_filt_conf[i],TMC4361A_SD_FILT2_MASK ,TMC4361A_SD_FILT2_SHIFT , 0);

        // INPUT_FILT_CONF // SR of Encoder output group: SDODRV_SCLK, SDIDRVNSCLK
        // min.: 0, max.: 7, default: 0
        tmc4361a_input_filt_conf[i] = REGISTER_SET(tmc4361a_input_filt_conf[i],TMC4361A_SR_ENC_OUT_MASK ,TMC4361A_SR_ENC_OUT_SHIFT , 0);

        // INPUT_FILT_CONF // FILT_L of Encoder output group
        // min.: 0, max.: 7, default: 0
        tmc4361a_input_filt_conf[i] = REGISTER_SET(tmc4361a_input_filt_conf[i],TMC4361A_FILT_L_ENC_OUT_MASK ,TMC4361A_FILT_L_ENC_OUT_SHIFT , 0);

        // INPUT_FILT_CONF // Choose Encoder output filter group for SD input pins
        // min.: 0, max.: 1, default: 0
        tmc4361a_input_filt_conf[i] = REGISTER_SET(tmc4361a_input_filt_conf[i],TMC4361A_SD_FILT3_MASK ,TMC4361A_SD_FILT3_SHIFT , 0);

    }

    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_INPUT_FILT_CONF, tmc4361a_input_filt_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
}