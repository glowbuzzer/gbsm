
/**
 ******************************************************************************
 * @file           :  TMC4351A_Register.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "TMC4361A_Register.h"

tmc4361a_register_t tmc4361a_register[NUM_TMC4361A_REGISTERS] = {

        {TMC4361A_GENERAL_CONF, "GENERAL_CONF", false},
        {TMC4361A_REFERENCE_CONF, "REFERENCE", false},
        {TMC4361A_START_CONF, "START_CONF", false},
        {TMC4361A_INPUT_FILT_CONF, "INPUT_FILT_CONF", false},
        {TMC4361A_SPIOUT_CONF, "SPIOUT_CONF", false},
        {TMC4361A_CURRENT_CONF, "CURRENT_CONF", false},
        {TMC4361A_SCALE_VALUES, "SCALE_VALUES", false},
        {TMC4361A_ENC_IN_CONF, "ENC_IN_CONF", false},
        {TMC4361A_ENC_IN_DATA, "ENC_IN_DATA", false},
        {TMC4361A_ENC_OUT_DATA, "ENC_OUT_DATA", false},
        {TMC4361A_STEP_CONF, "STEP_CONF", false},
        {TMC4361A_SPI_STATUS_SELECTION, "SPI_STATUS_SELECTION", false},
        {TMC4361A_EVENT_CLEAR_CONF, "EVENT_CLEAR_CONF", false},
        {TMC4361A_INTR_CONF, "INTR_CONF", false},
        {TMC4361A_EVENTS, "EVENTS", false},
        {TMC4361A_STATUS, "STATUS", false},

        {TMC4361A_STP_LENGTH_ADD, "STP_LENGTH_ADD", false},
        {TMC4361A_DIR_SETUP_TIME, "DIR_SETUP_TIME", false},
        {TMC4361A_START_OUT_ADD, "START_OUT_ADD", false},
        {TMC4361A_GEAR_RATIO, "GEAR_RATIO", false},
        {TMC4361A_START_DELAY, "START_DELAY", false},
        {TMC4361A_CLK_GATING_DELAY, "CLK_GATING_DELAY", false},
        {TMC4361A_STDBY_DELAY, "STDBY_DELAY", false},
        {TMC4361A_FREEWHEEL_DELAY, "FREEWHEEL_DELAY", false},
        {TMC4361A_VDRV_SCALE_LIMIT, "VDRV_SCALE_LIMIT", false},
        {TMC4361A_PWM_VMAX, "PWM_VMAX", false},
        {TMC4361A_UP_SCALE_DELAY, "UP_SCALE_DELAY", false},
        {TMC4361A_CL_UPSCALE_DELAY, "CL_UPSCALE_DELAY", false},
        {TMC4361A_HOLD_SCALE_DELAY, "HOLD_SCALE_DELAY", false},
        {TMC4361A_CL_DOWNSCALE_DELAY, "CL_DOWNSCALE_DELAY", false},
        {TMC4361A_DRV_SCALE_DELAY, "DRV_SCALE_DELAY", false},
        {TMC4361A_BOOST_TIME, "BOOST_TIME", false},
        {TMC4361A_CL_BETA, "CL_BETA", false},
        {TMC4361A_CL_GAMMA, "CL_GAMMA", false},
        {TMC4361A_DAC_ADDR_A, "AC_ADDR_A", false},
        {TMC4361A_DAC_ADDR_B, "DAC_ADDR_B", false},
        {TMC4361A_SPI_SWITCH_VEL, "SPI_SWITCH_VEL", false},
        {TMC4361A_HOME_SAFETY_MARGIN, "HOME_SAFETY_MARGIN", false},
        {TMC4361A_PWM_FREQ, "PWM_FREQ", false},
        {TMC4361A_CHOPSYNC_DIV, "CHOPSYNC_DIV", false},

        {TMC4361A_RAMPMODE, "RAMPMODE", false},
        {TMC4361A_XACTUAL, "XACTUAL", false},
        {TMC4361A_VACTUAL, "VACTUAL", false},
        {TMC4361A_AACTUAL, "AACTUAL", false},
        {TMC4361A_VMAX, "VMAX", false},
        {TMC4361A_VSTART, "VSTART", false},
        {TMC4361A_VSTOP, "VSTOP", false},
        {TMC4361A_VBREAK, "VBREAK", false},
        {TMC4361A_AMAX, "AMAX", false},
        {TMC4361A_DMAX, "DMAX", false},
        {TMC4361A_ASTART, "ASTART", false},
        {TMC4361A_SIGN_AACT, "SIGN_AACT", false},
        {TMC4361A_DFINAL, "DFINAL", false},
        {TMC4361A_DSTOP, "DSTOP", false},
        {TMC4361A_BOW1, "BOW1", false},
        {TMC4361A_BOW2, "BOW2", false},
        {TMC4361A_BOW3, "BOW3", false},
        {TMC4361A_BOW4, "BOW4", false},
        {TMC4361A_CLK_FREQ, "CLK_FREQ", false},

        {TMC4361A_POS_COMP, "POS_COMP", false},
        {TMC4361A_VIRT_STOP_LEFT, "VIRT_STOP_LEFT", false},
        {TMC4361A_VIRT_STOP_RIGHT, "VIRT_STOP_RIGHT", false},
        {TMC4361A_X_HOME, "X_HOME", false},
        {TMC4361A_X_LATCH_RD, "X_LATCH_RD", false},
        {TMC4361A_REV_CNT_RD, "REV_CNT_RD", false},
        {TMC4361A_X_RANGE_WR, "X_RANGE_WR", true},
        {TMC4361A_X_TARGET, "X_TARGET", false},

        {TMC4361A_X_PIPE0, "X_PIPE0", false},
        {TMC4361A_X_PIPE1, "X_PIPE1", false},
        {TMC4361A_X_PIPE2, "X_PIPE2", false},
        {TMC4361A_X_PIPE3, "X_PIPE3", false},
        {TMC4361A_X_PIPE4, "X_PIPE4", false},
        {TMC4361A_X_PIPE5, "X_PIPE5", false},
        {TMC4361A_X_PIPE6, "X_PIPE6", false},
        {TMC4361A_X_PIPE7, "X_PIPE7", false},

        {TMC4361A_SH_REG0, "SH_REG0", false},
        {TMC4361A_SH_REG1, "SH_REG1", false},
        {TMC4361A_SH_REG2, "SH_REG2", false},
        {TMC4361A_SH_REG3, "SH_REG3", false},
        {TMC4361A_SH_REG4, "SH_REG4", false},
        {TMC4361A_SH_REG5, "SH_REG5", false},
        {TMC4361A_SH_REG6, "SH_REG6", false},
        {TMC4361A_SH_REG7, "SH_REG7", false},
        {TMC4361A_SH_REG8, "SH_REG8", false},
        {TMC4361A_SH_REG9, "SH_REG9", false},
        {TMC4361A_SH_REG10, "SH_REG10", false},
        {TMC4361A_SH_REG11, "SH_REG11", false},
        {TMC4361A_SH_REG12, "SH_REG12", false},
        {TMC4361A_SH_REG13, "SH_REG12", false},

        {TMC4361A_DFREEZE, "DFREEZE", false},
        {TMC4361A_IFREEZE, "IFREEZE", false},
        {TMC4361A_CLK_GATING_REG, "CLK_GATING_REG", false},
        {TMC4361A_RESET_REG, "RESET_REG", false},

        {TMC4361A_ENC_POS, "ENC_POS", false},
        {TMC4361A_ENC_LATCH_RD, "ENC_LATCH_RD", false},
        {TMC4361A_ENC_RESET_VAL_WR, "ENC_RESET_VAL_WR", true},
        {TMC4361A_ENC_POS_DEV_RD, "ENC_POS_DEV_RD", false},
        {TMC4361A_CL_TR_TOLERANCE_WR, "CL_TR_TOLERANCE_WR", true},
        {TMC4361A_ENC_POS_DEV_TOL_WR, "ENC_POS_DEV_TOL_WR", true},
        {TMC4361A_ENC_IN_RES_WR, "ENC_IN_RES_WR", true},
        {TMC4361A_ENC_CONST_RD, "ENC_CONST_RD", false},
        {TMC4361A_MANUAL_ENC_CONST0, "MANUAL_ENC_CONST0", false},
        {TMC4361A_ENC_OUT_RES, "ENC_OUT_RES", false},
        {TMC4361A_SER_CLK_IN_HIGH_WR, "SER_CLK_IN_HIGH_WR", true},
        {TMC4361A_SER_CLK_IN_LOW_WR, "SER_CLK_IN_LOW_WR", true},
        {TMC4361A_SSI_IN_CLK_DELAY_WR, "SSI_IN_CLK_DELAY_WR", true},
        {TMC4361A_SSI_IN_WTIME_WR, "SSI_IN_WTIME_WR", true},
        {TMC4361A_SER_PTIME_WR, "SER_PTIME_WR", true},

        {TMC4361A_CL_OFFSET, "CL_OFFSET", false},
        {TMC4361A_PID_P_WR, "PID_P_WR", true},
        {TMC4361A_CL_VMAX_CALC_P_WR, "CL_VMAX_CALC_P_WR", true},
        {TMC4361A_PID_VEL_RD, "PID_VEL_RD", false},
        {TMC4361A_PID_I_WR, "PID_I_WR", true},
        {TMC4361A_CL_VMAX_CALC_I_WR, "CL_VMAX_CALC_I_WR", true},
        {TMC4361A_PID_ISUM_RD, "PID_ISUM_RD", false},
        {TMC4361A_PID_D_WR, "PID_D_WR", true},
        {TMC4361A_CL_DELTA_P_WR, "CL_DELTA_P_WR", true},
        {TMC4361A_PID_I_CLIP_WR, "PID_I_CLIP_WR", true},
        {TMC4361A_PID_D_CLKDIV_WR, "PID_D_CLKDIV_WR", true},
        {TMC4361A_PID_E_RD, "PID_E_RD", false},
        {TMC4361A_PID_DV_CLIP_WR, "PID_DV_CLIP_WR", true},
        {TMC4361A_PID_TOLERANCE_WR, "PID_TOLERANCE_WR", true},
        {TMC4361A_CL_TOLERANCE_WR, "CL_TOLERANCE_WR", true},

        {TMC4361A_FS_VEL_WR, "FS_VEL_WR", true},
        {TMC4361A_DC_VEL_WR, "DC_VEL_WR", true},
        {TMC4361A_CL_VMIN_EMF_WR, "CL_VMIN_EMF_WR", true},
        {TMC4361A_DC_TIME_WR, "DC_TIME_WR", true},
        {TMC4361A_DC_SG_WR, "DC_SG_WR", true},
        {TMC4361A_DC_BLKTIME_WR, "DC_BLKTIME_WR", true},
        {TMC4361A_CL_VADD_EMF, "CL_VADD_EMF", false},
        {TMC4361A_DC_LSPTM_WR, "DC_LSPTM_WR", true},
        {TMC4361A_ENC_VEL_ZERO_WR, "ENC_VEL_ZERO_WR", true},
        {TMC4361A_ENC_VMEAN_WAIT_WR, "ENC_VMEAN_WAIT_WR", true},
        {TMC4361A_ENC_VMEAN_FILTER_WR, "ENC_VMEAN_FILTER_WR", true},
        {TMC4361A_ENC_VMEAN_INT_WR, "ENC_VMEAN_INT_WR", true},
        {TMC4361A_SER_ENC_VARIATION_WR, "SER_ENC_VARIATION_WR", true},
        {TMC4361A_CL_CYCLE_WR, "CL_CYCLE_WR", true},
        {TMC4361A_SYNCHRO_SET, "SYNCHRO_SET,", false},
        {TMC4361A_V_ENC_RD, "", false},
        {TMC4361A_V_ENC_MEAN_RD, "V_ENC_MEAN_RD", false},
        {TMC4361A_VSTALL_LIMIT_WR, "VSTALL_LIMIT_WR", true},

        {TMC4361A_ADDR_TO_ENC, "ADDR_TO_ENC", false},
        {TMC4361A_DATA_TO_ENC, "DATA_TO_ENC", false},
        {TMC4361A_ADDR_FROM_ENC, "ADDR_FROM_ENC", false},
        {TMC4361A_DATA_FROM_ENC, "DATA_FROM_ENC", false},
        { TMC4361A_COVER_LOW_WR, "COVER_LOW_WR", true },
        { TMC4361A_POLLING_STATUS_RD, "POLLING_STATUS_RD" , false},
        { TMC4361A_COVER_HIGH_WR, "" ,true},
        { TMC4361A_POLLING_REG_WR, "POLLING_REG_WR" ,true},
        { TMC4361A_COVER_DRV_LOW_RD, "COVER_DRV_LOW_RD", false },
        { TMC4361A_COVER_DRV_HIGH_RD, "COVER_DRV_HIGH_RD" , false},

        { TMC4361A_MSLUT_0_WR, "MSLUT_0_WR" ,true},
        { TMC4361A_MSLUT_1_WR, "MSLUT_1_WR" ,true},
        { TMC4361A_MSLUT_2_WR, "MSLUT_2_WR",true },
        { TMC4361A_MSLUT_3_WR, "MSLUT_3_WR" ,true},
        { TMC4361A_MSLUT_4_WR, "MSLUT_4_WR" ,true},
        { TMC4361A_MSLUT_5_WR, "MSLUT_5_WR" ,true},
        { TMC4361A_MSLUT_6_WR, "MSLUT_6_WR" ,true},
        { TMC4361A_MSLUT_7_WR, "MSLUT_7_WR" },
        { TMC4361A_MSLUTSEL_WR, "MSLUTSEL_WR" },
        { TMC4361A_MSCNT_RD, "MSCNT_RD", false },
        { TMC4361A_MSOFFSET_WR, "MSOFFSET_WR" ,true},
        { TMC4361A_CURRENTA_RD, "CURRENTA_RD", false },
        { TMC4361A_CURRENTB_RD, "CURRENTB_RD", false },
        { TMC4361A_CURRENTA_SPI_RD, "CURRENTA_SPI_RD", false },
        { TMC4361A_CURRENTB_SPI_RD, "CURRENTB_SPI_RD", false },
        { TMC4361A_TZEROWAIT_WR, "TZEROWAIT_WR" ,true},
        { TMC4361A_SCALE_PARAM_RD, "SCALE_PARAM_RD", false },
        { TMC4361A_CIRCULAR_DEC_WR, "CIRCULAR_DEC_WR",true },
        { TMC4361A_ENC_COMP_XOFFSET, "ENC_COMP_XOFFSET", false },
        { TMC4361A_ENC_COMP_YOFFSET, "ENC_COMP_YOFFSET", false },
        { TMC4361A_ENC_COMP_AMPL, "ENC_COMP_AMPL" , false},
        { TMC4361A_START_SIN_WR, "START_SIN_WR",true },
        { TMC4361A_START_SIN90_120_WR, "START_SIN90_120_WR",true },
        { TMC4361A_DAC_OFFSET_WR, "DAC_OFFSET_WR" ,true},
        { TMC4361A_VERSION_NO_RD, "VERSION_NO_RD", false },
};
