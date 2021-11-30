
/**
 ******************************************************************************
 * @file           :  tmc4361a_start_conf.c
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

void tmc4361a_start_conf_set(const unsigned int *hspi) {
    /* START_CONF default = 0x0 */

    /* The start conf is:
     * Alteration of XTARGET value requires distinct start signal TMC4361A_START_EN0_SHIFT
     * Alteration of VMAX value requires distinct start signal TMC4361A_START_EN1_SHIFT
     * Start pin is an input TMC4361A_TRIGGER_EVENTS0_SHIFT
     * Start pin is low active
     * Active START input signal is executed immediately TMC4361A_IMMEDIATE_START_IN_SHIFT
    */

    uint32_t tmc4361a_start_conf[SM_NUM_DRIVES] = {0};

    for (int i = 0; i < SM_NUM_DRIVES; i++) {

        // START_CONF // Alteration of XTARGET value requires distinct start signal
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i], TMC4361A_START_EN0_MASK,TMC4361A_START_EN0_SHIFT , 1);

        // START_CONF // Alteration of VMAX value requires distinct start signal
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_START_EN1_MASK ,TMC4361A_START_EN1_SHIFT , 1);

        // START_CONF // Alteration of RAMPMODE value requires distinct start signal
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i], TMC4361A_START_EN2_MASK, TMC4361A_START_EN2_SHIFT, 0);

        // START_CONF // Alteration of GEAR_RATIO value requires distinct start signal
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_START_EN3_MASK ,TMC4361A_START_EN3_SHIFT , 0);

        // START_CONF // Activation of shadow register feature set
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_START_EN4_MASK ,TMC4361A_START_EN4_SHIFT , 0);

        // START_CONF // External start signal is enabled as timer trigger. START pin is assigned as input
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_TRIGGER_EVENTS0_MASK ,TMC4361A_TRIGGER_EVENTS0_SHIFT , 1);

        // START_CONF // TARGET_REACHED event is assigned as start signal trigger
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_TRIGGER_EVENTS1_MASK ,TMC4361A_TRIGGER_EVENTS1_SHIFT , 0);

        // START_CONF // VELOCITY_REACHED event is assigned as start signal trigger
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_TRIGGER_EVENTS2_MASK ,TMC4361A_TRIGGER_EVENTS2_SHIFT , 0);

        // START_CONF // POSCOMP_REACHED event is assigned as start signal trigger
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i], TMC4361A_TRIGGER_EVENTS3_MASK, TMC4361A_TRIGGER_EVENTS3_SHIFT, 0);

        // START_CONF // Active START pin polarity
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i], TMC4361A_POL_START_SIGNAL_MASK,TMC4361A_POL_START_SIGNAL_SHIFT , 0);

        // START_CONF // Immediate external START signal processing
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_IMMEDIATE_START_IN_MASK ,TMC4361A_IMMEDIATE_START_IN_SHIFT , 1);

        // START_CONF // Busy state enable switch
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_BUSY_STATE_EN_MASK , TMC4361A_BUSY_STATE_EN_SHIFT, 0);

        // START_CONF // XTARGET is considered for pipelining
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_PIPELINE_EN0_MASK , TMC4361A_PIPELINE_EN0_SHIFT, 0);

        // START_CONF // POS_COMP is considered for pipelining
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_PIPELINE_EN1_MASK ,TMC4361A_PIPELINE_EN1_SHIFT , 0);

        // START_CONF // GEAR_RATIO is considered for pipelining
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_PIPELINE_EN2_MASK ,TMC4361A_PIPELINE_EN2_SHIFT , 0);

        // START_CONF // GENERAL_CONF is considered for pipelining
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_PIPELINE_EN3_MASK ,TMC4361A_PIPELINE_EN3_SHIFT , 0);

        // START_CONF // Shadow registers options
        // min.: 0, max.: 3, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_SHADOW_OPTION_MASK ,TMC4361A_SHADOW_OPTION_SHIFT , 0);

        // START_CONF // Cyclic shadow register switch
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_CYCLIC_SHADOW_REGS_MASK , TMC4361A_CYCLIC_SHADOW_REGS_SHIFT, 0);

        // START_CONF // Number of unused internal start signals between two consecutive shadow register transfers
        // min.: 0, max.: 15, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i], TMC4361A_SHADOW_MISS_CNT_MASK, TMC4361A_SHADOW_MISS_CNT_SHIFT, 0);

        // START_CONF // X_PIPE0 is selected for rewriting
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i], TMC4361A_XPIPE_REWRITE_REG0_MASK,TMC4361A_XPIPE_REWRITE_REG0_SHIFT , 0);

        // START_CONF // X_PIPE1 is selected for rewriting
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_XPIPE_REWRITE_REG1_MASK , TMC4361A_XPIPE_REWRITE_REG1_SHIFT, 0);

        // START_CONF // X_PIPE2 is selected for rewriting
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_XPIPE_REWRITE_REG2_MASK , TMC4361A_XPIPE_REWRITE_REG2_SHIFT, 0);

        // START_CONF // X_PIPE3 is selected for rewriting
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_XPIPE_REWRITE_REG3_MASK ,TMC4361A_XPIPE_REWRITE_REG3_SHIFT , 0);

        // START_CONF // X_PIPE4 is selected for rewriting
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_XPIPE_REWRITE_REG4_MASK ,TMC4361A_XPIPE_REWRITE_REG4_SHIFT , 0);

        // START_CONF // X_PIPE5 is selected for rewriting
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_XPIPE_REWRITE_REG5_MASK ,TMC4361A_XPIPE_REWRITE_REG5_SHIFT , 0);

        // START_CONF // X_PIPE6 is selected for rewriting
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i], TMC4361A_XPIPE_REWRITE_REG6_MASK,TMC4361A_XPIPE_REWRITE_REG6_SHIFT , 0);

        // START_CONF // X_PIPE7 is selected for rewriting
        // min.: 0, max.: 1, default: 0
        tmc4361a_start_conf[i] = REGISTER_SET(tmc4361a_start_conf[i],TMC4361A_XPIPE_REWRITE_REG7_MASK , TMC4361A_XPIPE_REWRITE_REG7_SHIFT, 0);

    }

    tmc4361A_write_uint32_to_all_different_vals(TMC4361A_START_CONF, tmc4361a_start_conf, SM_NUM_DRIVES, hspi, tmc_status_from_spi);
}