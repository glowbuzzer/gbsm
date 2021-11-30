
/**
 ******************************************************************************
 * @file           :  tmc4361a_enc_in_conf.c
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

void tmc4361a_enc_in_conf_set(const unsigned int *hspi) {
    /* ENC_IN_CONF - default = 0x00000400 = 0b100 0000 0000 */
    uint32_t tmc4361a_enc_in_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {

// ENC_IN_CONF // Encoder constant is binary/decimal
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i], TMC4361A_ENC_SEL_DECIMAL_MASK,TMC4361A_ENC_SEL_DECIMAL_SHIFT , 0);

// ENC_IN_CONF // ENC_POS is reset to ENC_RESET_VAL at every/next N event
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_CLEAR_ON_N_MASK ,TMC4361A_CLEAR_ON_N_SHIFT , 0);

// ENC_IN_CONF // Every active N input signal generates an active N event
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_CLR_LATCH_CONT_ON_N_MASK ,TMC4361A_CLR_LATCH_CONT_ON_N_SHIFT , 0);

// ENC_IN_CONF // Only next active N input signal generates an active N event. This bit is autmatically cleared
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_CLR_LATCH_ONCE_ON_N_MASK ,TMC4361A_CLR_LATCH_ONCE_ON_N_SHIFT , 0);

// ENC_IN_CONF // Active polarity of N input signal
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i], TMC4361A_POL_N_MASK,TMC4361A_POL_N_SHIFT , 0);

// ENC_IN_CONF // N event trigger
// min.: 0, max.: 3, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_N_CHAN_SENSITIVITY_MASK ,TMC4361A_N_CHAN_SENSITIVITY_SHIFT , 0);

// ENC_IN_CONF // Active polarity of A for valid N event
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_POL_A_FOR_N_MASK ,TMC4361A_POL_A_FOR_N_SHIFT , 0);

// ENC_IN_CONF // Active polarity of B for valid N event
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_POL_B_FOR_N_MASK ,TMC4361A_POL_B_FOR_N_SHIFT , 0);

// ENC_IN_CONF // No valid AB polarity required for valid N event
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_IGNORE_AB_MASK , TMC4361A_IGNORE_AB_SHIFT, 0);

// ENC_IN_CONF // ENC_POS is latched to ENC_LATCH at every/next N event
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_LATCH_ENC_ON_N_MASK , TMC4361A_LATCH_ENC_ON_N_SHIFT, 1);

// ENC_IN_CONF // XACTUAL is latched to X_LATCH at every/next N event
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_LATCH_X_ON_N_MASK ,TMC4361A_LATCH_X_ON_N_SHIFT , 0);

// ENC_IN_CONF // Enabling multiturn encoder
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_MULTI_TURN_IN_EN_MASK ,TMC4361A_MULTI_TURN_IN_EN_SHIFT , 0);

// ENC_IN_CONF // Multiturn data is signed
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_MULTI_TURN_IN_SIGNED_MASK ,TMC4361A_MULTI_TURN_IN_SIGNED_SHIFT , 0);

// ENC_IN_CONF // Serial encoder output transmits multiturn data
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_MULTI_TURN_OUT_EN_MASK ,TMC4361A_MULTI_TURN_OUT_EN_SHIFT, 0);

// ENC_IN_CONF // USTEPS_PER_REV limits cirular motion
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_USE_USTEPS_INSTEAD_OF_XRANGE_MASK ,TMC4361A_USE_USTEPS_INSTEAD_OF_XRANGE_SHIFT , 0);

// ENC_IN_CONF // Internal calculation of multiturn data
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i], TMC4361A_CALC_MULTI_TURN_BEHAV_MASK, TMC4361A_CALC_MULTI_TURN_BEHAV_SHIFT, 0);

// ENC_IN_CONF // Multicycle SSI requests
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_SSI_MULTI_CYCLE_DATA_MASK , TMC4361A_SSI_MULTI_CYCLE_DATA_SHIFT, 0);

// ENC_IN_CONF // SSI data is gray-coded
 // min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_SSI_GRAY_CODE_EN_MASK , TMC4361A_SSI_GRAY_CODE_EN_SHIFT, 0);

// ENC_IN_CONF // Serial input data is left aligned (first data, then flags)
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i], TMC4361A_LEFT_ALIGNED_DATA_MASK, TMC4361A_LEFT_ALIGNED_DATA_SHIFT, 0);

// ENC_IN_CONF // SPI Data is provided immediately when NSCLK (=NCS) switches low
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i], TMC4361A_SPI_DATA_ON_CS_MASK, TMC4361A_SPI_DATA_ON_CS_SHIFT, 0);

// ENC_IN_CONF // SCLK switches low before NSCLK (=NCS)
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i], TMC4361A_SPI_LOW_BEFORE_CS_MASK,TMC4361A_SPI_LOW_BEFORE_CS_SHIFT , 0);

// ENC_IN_CONF // Closed-loop or PID
// min.: 0, max.: 3, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i], TMC4361A_REGULATION_MODUS_MASK, TMC4361A_REGULATION_MODUS_SHIFT, 0);

// ENC_IN_CONF // Enabling closed-loop calibration
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_CL_CALIBRATION_EN_MASK ,TMC4361A_CL_CALIBRATION_EN_SHIFT , 0);

// ENC_IN_CONF // Enabling closed-loop back-emf compensation
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_CL_EMF_EN_MASK ,TMC4361A_CL_EMF_EN_SHIFT , 0);

// ENC_IN_CONF // XACTUAL = ENC_POS in case |ENC_POS_DEV| > ENC_POS_DEV_TOL
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_CL_CLR_XACT_MASK ,TMC4361A_CL_CLR_XACT_SHIFT , 0);

// ENC_IN_CONF // Enabling limit for closed-loop catch-up velocity
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_CL_VLIMIT_EN_MASK ,TMC4361A_CL_VLIMIT_EN_SHIFT , 0);

// ENC_IN_CONF // Enabling closed-loop velocity mode
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_CL_VELOCITY_MODE_EN_MASK ,TMC4361A_CL_VELOCITY_MODE_EN_SHIFT , 0);

 // ENC_IN_CONF // Internal encoder direction is inverted
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_INVERT_ENC_DIR_MASK ,TMC4361A_INVERT_ENC_DIR_SHIFT , 0);

// ENC_IN_CONF // SSI output data is gray-coded
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_ENC_OUT_GRAY_MASK ,TMC4361A_ENC_OUT_GRAY_SHIFT , 0);

// ENC_IN_CONF // Disabling AB preprocessing for encoder velocity calculation
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i], TMC4361A_NO_ENC_VEL_PREPROC_MASK,TMC4361A_NO_ENC_VEL_PREPROC_SHIFT , 0);

// ENC_IN_CONF // Limiting the deviation between two consecutive encoder values for valid input data
// min.: 0, max.: 1, default: 0
        tmc4361a_enc_in_conf[i] = REGISTER_SET(tmc4361a_enc_in_conf[i],TMC4361A_SERIAL_ENC_VARIATION_LIMIT_MASK , TMC4361A_SERIAL_ENC_VARIATION_LIMIT_SHIFT, 0);

    }

    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_ENC_IN_CONF, tmc4361a_enc_in_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
}