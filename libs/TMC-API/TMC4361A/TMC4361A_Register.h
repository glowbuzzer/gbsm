/*
 * TMC4361A_Register.h
 *
 *  Created on: 18.07.2017
 *      Author: LK
 */

#ifndef TMC4361A_REGISTER_H
#define TMC4361A_REGISTER_H
#include <stdbool.h>

typedef struct{
    int address;
    char name[20];
    bool write;
}tmc4361a_register_t;

#define NUM_TMC4361A_REGISTERS 176
tmc4361a_register_t tmc4361a_register[NUM_TMC4361A_REGISTERS];

#define TMC4361A_GENERAL_CONF          0x00
#define TMC4361A_REFERENCE_CONF        0x01
#define TMC4361A_START_CONF            0x02
#define TMC4361A_INPUT_FILT_CONF       0x03
#define TMC4361A_SPIOUT_CONF           0x04
#define TMC4361A_CURRENT_CONF          0x05
#define TMC4361A_SCALE_VALUES          0x06
#define TMC4361A_ENC_IN_CONF           0x07
#define TMC4361A_ENC_IN_DATA           0x08
#define TMC4361A_ENC_OUT_DATA          0x09
#define TMC4361A_STEP_CONF             0x0A
#define TMC4361A_SPI_STATUS_SELECTION  0x0B
#define TMC4361A_EVENT_CLEAR_CONF      0x0C
#define TMC4361A_INTR_CONF             0x0D
#define TMC4361A_EVENTS                0x0E
#define TMC4361A_STATUS                0x0F

#define TMC4361A_STP_LENGTH_ADD        0x10
#define TMC4361A_DIR_SETUP_TIME        0x10
#define TMC4361A_START_OUT_ADD         0x11
#define TMC4361A_GEAR_RATIO            0x12
#define TMC4361A_START_DELAY           0x13
#define TMC4361A_CLK_GATING_DELAY      0x14
#define TMC4361A_STDBY_DELAY           0x15
#define TMC4361A_FREEWHEEL_DELAY       0x16
#define TMC4361A_VDRV_SCALE_LIMIT      0x17
#define TMC4361A_PWM_VMAX              0x17
#define TMC4361A_UP_SCALE_DELAY        0x18
#define TMC4361A_CL_UPSCALE_DELAY      0x18
#define TMC4361A_HOLD_SCALE_DELAY      0x19
#define TMC4361A_CL_DOWNSCALE_DELAY    0x19
#define TMC4361A_DRV_SCALE_DELAY       0x1A
#define TMC4361A_BOOST_TIME            0x1B
#define TMC4361A_CL_BETA               0x1C
#define TMC4361A_CL_GAMMA              0x1C
#define TMC4361A_DAC_ADDR_A            0x1D
#define TMC4361A_DAC_ADDR_B            0x1D
#define TMC4361A_SPI_SWITCH_VEL        0x1D
#define TMC4361A_HOME_SAFETY_MARGIN    0x1E
#define TMC4361A_PWM_FREQ              0x1F
#define TMC4361A_CHOPSYNC_DIV          0x1F

#define TMC4361A_RAMPMODE              0x20
#define TMC4361A_XACTUAL               0x21
#define TMC4361A_VACTUAL               0x22
#define TMC4361A_AACTUAL               0x23
#define TMC4361A_VMAX                  0x24
#define TMC4361A_VSTART                0x25
#define TMC4361A_VSTOP                 0x26
#define TMC4361A_VBREAK                0x27
#define TMC4361A_AMAX                  0x28
#define TMC4361A_DMAX                  0x29
#define TMC4361A_ASTART                0x2A
#define TMC4361A_SIGN_AACT             0x2A
#define TMC4361A_DFINAL                0x2B
#define TMC4361A_DSTOP                 0x2C
#define TMC4361A_BOW1                  0x2D
#define TMC4361A_BOW2                  0x2E
#define TMC4361A_BOW3                  0x2F
#define TMC4361A_BOW4                  0x30
#define TMC4361A_CLK_FREQ              0x31

#define TMC4361A_POS_COMP              0x32
#define TMC4361A_VIRT_STOP_LEFT        0x33
#define TMC4361A_VIRT_STOP_RIGHT       0x34
#define TMC4361A_X_HOME                0x35
#define TMC4361A_X_LATCH_RD            0x36
#define TMC4361A_REV_CNT_RD            0x36
#define TMC4361A_X_RANGE_WR            0x36
#define TMC4361A_X_TARGET              0x37

#define TMC4361A_X_PIPE0               0x38
#define TMC4361A_X_PIPE1               0x39
#define TMC4361A_X_PIPE2               0x3A
#define TMC4361A_X_PIPE3               0x3B
#define TMC4361A_X_PIPE4               0x3C
#define TMC4361A_X_PIPE5               0x3D
#define TMC4361A_X_PIPE6               0x3E
#define TMC4361A_X_PIPE7               0x3F

#define TMC4361A_SH_REG0               0x40
#define TMC4361A_SH_REG1               0x41
#define TMC4361A_SH_REG2               0x42
#define TMC4361A_SH_REG3               0x43
#define TMC4361A_SH_REG4               0x44
#define TMC4361A_SH_REG5               0x45
#define TMC4361A_SH_REG6               0x46
#define TMC4361A_SH_REG7               0x47
#define TMC4361A_SH_REG8               0x48
#define TMC4361A_SH_REG9               0x49
#define TMC4361A_SH_REG10              0x4A
#define TMC4361A_SH_REG11              0x4B
#define TMC4361A_SH_REG12              0x4C
#define TMC4361A_SH_REG13              0x4D

#define TMC4361A_DFREEZE               0x4E
#define TMC4361A_IFREEZE               0x4E
#define TMC4361A_CLK_GATING_REG        0x4F
#define TMC4361A_RESET_REG             0x4F

#define TMC4361A_ENC_POS               0x50
#define TMC4361A_ENC_LATCH_RD          0x51
#define TMC4361A_ENC_RESET_VAL_WR      0x51
#define TMC4361A_ENC_POS_DEV_RD        0x52
#define TMC4361A_CL_TR_TOLERANCE_WR    0x52
#define TMC4361A_ENC_POS_DEV_TOL_WR    0x53
#define TMC4361A_ENC_IN_RES_WR         0x54
#define TMC4361A_ENC_CONST_RD          0x54
#define TMC4361A_MANUAL_ENC_CONST0     0x54
#define TMC4361A_ENC_OUT_RES           0x55
#define TMC4361A_SER_CLK_IN_HIGH_WR    0x56
#define TMC4361A_SER_CLK_IN_LOW_WR     0x56
#define TMC4361A_SSI_IN_CLK_DELAY_WR   0x57
#define TMC4361A_SSI_IN_WTIME_WR       0x57
#define TMC4361A_SER_PTIME_WR          0x58

#define TMC4361A_CL_OFFSET             0x59
#define TMC4361A_PID_P_WR              0x5A
#define TMC4361A_CL_VMAX_CALC_P_WR     0x5A
#define TMC4361A_PID_VEL_RD            0x5A
#define TMC4361A_PID_I_WR              0x5B
#define TMC4361A_CL_VMAX_CALC_I_WR     0x5B
#define TMC4361A_PID_ISUM_RD           0x5B
#define TMC4361A_PID_D_WR              0x5C
#define TMC4361A_CL_DELTA_P_WR         0x5C
#define TMC4361A_PID_I_CLIP_WR         0x5D
#define TMC4361A_PID_D_CLKDIV_WR       0x5D
#define TMC4361A_PID_E_RD              0x5D
#define TMC4361A_PID_DV_CLIP_WR        0x5E
#define TMC4361A_PID_TOLERANCE_WR      0x5F
#define TMC4361A_CL_TOLERANCE_WR       0x5F

#define TMC4361A_FS_VEL_WR             0x60
#define TMC4361A_DC_VEL_WR             0x60
#define TMC4361A_CL_VMIN_EMF_WR        0x60
#define TMC4361A_DC_TIME_WR            0x61
#define TMC4361A_DC_SG_WR              0x61
#define TMC4361A_DC_BLKTIME_WR         0x61
#define TMC4361A_CL_VADD_EMF           0x61
#define TMC4361A_DC_LSPTM_WR           0x62
#define TMC4361A_ENC_VEL_ZERO_WR       0x62
#define TMC4361A_ENC_VMEAN_WAIT_WR     0x63
#define TMC4361A_ENC_VMEAN_FILTER_WR   0x63
#define TMC4361A_ENC_VMEAN_INT_WR      0x63
#define TMC4361A_SER_ENC_VARIATION_WR  0x63
#define TMC4361A_CL_CYCLE_WR           0x63
#define TMC4361A_SYNCHRO_SET           0x64
#define TMC4361A_V_ENC_RD              0x65
#define TMC4361A_V_ENC_MEAN_RD         0x66
#define TMC4361A_VSTALL_LIMIT_WR       0x67

#define TMC4361A_ADDR_TO_ENC           0x68
#define TMC4361A_DATA_TO_ENC           0x69
#define TMC4361A_ADDR_FROM_ENC         0x6A
#define TMC4361A_DATA_FROM_ENC         0x6B
#define TMC4361A_COVER_LOW_WR          0x6C
#define TMC4361A_POLLING_STATUS_RD     0x6C
#define TMC4361A_COVER_HIGH_WR         0x6D
#define TMC4361A_POLLING_REG_WR        0x6D
#define TMC4361A_COVER_DRV_LOW_RD      0x6E
#define TMC4361A_COVER_DRV_HIGH_RD     0x6F

#define TMC4361A_MSLUT_0_WR            0x70
#define TMC4361A_MSLUT_1_WR            0x71
#define TMC4361A_MSLUT_2_WR            0x72
#define TMC4361A_MSLUT_3_WR            0x73
#define TMC4361A_MSLUT_4_WR            0x74
#define TMC4361A_MSLUT_5_WR            0x75
#define TMC4361A_MSLUT_6_WR            0x76
#define TMC4361A_MSLUT_7_WR            0x77
#define TMC4361A_MSLUTSEL_WR           0x78
#define TMC4361A_MSCNT_RD              0x79
#define TMC4361A_MSOFFSET_WR           0x79
#define TMC4361A_CURRENTA_RD           0x7A
#define TMC4361A_CURRENTB_RD           0x7A
#define TMC4361A_CURRENTA_SPI_RD       0x7B
#define TMC4361A_CURRENTB_SPI_RD       0x7B
#define TMC4361A_TZEROWAIT_WR          0x7B
#define TMC4361A_SCALE_PARAM_RD        0x7C
#define TMC4361A_CIRCULAR_DEC_WR       0x7C
#define TMC4361A_ENC_COMP_XOFFSET      0x7D
#define TMC4361A_ENC_COMP_YOFFSET      0x7D
#define TMC4361A_ENC_COMP_AMPL         0x7D
#define TMC4361A_START_SIN_WR          0x7E
#define TMC4361A_START_SIN90_120_WR    0x7E
#define TMC4361A_DAC_OFFSET_WR         0x7E
#define TMC4361A_VERSION_NO_RD         0x7F

#endif /* TMC4361A_REGISTER_H */
