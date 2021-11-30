
/**
 ******************************************************************************
 * @file           :  gbsm_config.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#include "gbsm_config.h"
#include "stdbool.h"


bool gbsm_motor_directions[SM_NUM_DRIVES] = {0};
bool gbsm_enable_phystop_l[SM_NUM_DRIVES] = {0};
bool gbsm_enable_phystop_r[SM_NUM_DRIVES] = {0};


gbsm_encoders_config_t gbsm_encoder_config[SM_NUM_DRIVES] = {
        {.enable=0, .encoder_type=GBSM_ENCODER_NONE}
};

gbsm_vstop_config_t gbsm_vstop_config[SM_NUM_DRIVES] = {
        {.enable_l=0,.enable_r=0, .pos_l=0, .pos_r=0}
};