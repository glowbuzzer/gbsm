
/**
 ******************************************************************************
 * @file           :  TMC5160_Register.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "TMC5160_Register.h"



tmc5160_register_t tmc5160_register[NUM_TMC5160_REGISTERS] = {
        {TMC5160_GCONF, "GCONF"},
        {TMC5160_GSTAT, "GSTAT"},
        {TMC5160_IFCNT, "IFCNT"},
        {TMC5160_SLAVECONF, "SLAVECONF"},
        {TMC5160_INP_OUT, "INP_OUT"},
        {TMC5160_X_COMPARE, "X_COMPARE"},
        {TMC5160_OTP_PROG, "OTP_PROG"},
        {TMC5160_OTP_READ, "OTP_READ"},
        {TMC5160_FACTORY_CONF, "FACTORY_CONF"},
        {TMC5160_SHORT_CONF, "SHORT_CONF"},
        {TMC5160_DRV_CONF, "DRV_CONF"},
        {TMC5160_GLOBAL_SCALER, "GLOBAL_SCALER"},
        {TMC5160_OFFSET_READ, "OFFSET_READ"},
        {TMC5160_IHOLD_IRUN, "IHOLD_IRUN"},
        {TMC5160_TPOWERDOWN, "TPOWERDOWN"},
        {TMC5160_TSTEP, "TSTEP"},
        {TMC5160_TPWMTHRS, "TPWMTHRS"},
        {TMC5160_TCOOLTHRS, "TCOOLTHRS"},
        {TMC5160_THIGH, "THIGH"},

        {TMC5160_RAMPMODE, "RAMPMODE"},
        {TMC5160_XACTUAL, "XACTUAL"},
        {TMC5160_VACTUAL, "VACTUAL"},
        {TMC5160_VSTART, "VSTART"},
        {TMC5160_A1, "A1"},
        {TMC5160_V1, "V1"},
        {TMC5160_AMAX, "AMAZ"},
        {TMC5160_VMAX, "VMAX"},
        {TMC5160_DMAX, "DMAX"},
        {TMC5160_D1, "D1"},
        {TMC5160_VSTOP, "VSTOP"},
        {TMC5160_TZEROWAIT, "TZEROWAIT"},
        {TMC5160_XTARGET, "XTARGET"},

        {TMC5160_VDCMIN, "VDCMIN"},
        {TMC5160_SWMODE, "SWMODE"},
        {TMC5160_RAMPSTAT, "RAMPSTAT"},
        {TMC5160_XLATCH, "XLATCH"},
        {TMC5160_ENCMODE, "ENCMODE"},
        {TMC5160_XENC, "XENC"},
        {TMC5160_ENC_CONST, "ENC_CONST"},
        {TMC5160_ENC_STATUS, "ENC_STATUS"},
        {TMC5160_ENC_LATCH, "ENC_LATCH"},
        {TMC5160_ENC_DEVIATION, "ENC_DEVIATION"},

        {TMC5160_MSLUT0, "MSLUT0"},
        {TMC5160_MSLUT1, "MSLUT1"},
        {TMC5160_MSLUT2, "MSLUT2"},
        {TMC5160_MSLUT3, "MSLUT3"},
        {TMC5160_MSLUT4, "MSLUT4"},
        {TMC5160_MSLUT5, "MSLUT5"},
        {TMC5160_MSLUT6, "MSLUT6"},
        {TMC5160_MSLUT7, "MSLUT7"},
        {TMC5160_MSLUTSEL, "MSLUTSEL"},
        {TMC5160_MSLUTSTART, "MSLUTSTART"},
        {TMC5160_MSCNT, "MSCNT"},
        {TMC5160_MSCURACT, "MSCURACT"},
        {TMC5160_CHOPCONF, "CHOPCONF"},
        {TMC5160_COOLCONF, "COOLCONF"},
        {TMC5160_DCCTRL, "DCCTRL"},
        {TMC5160_DRVSTATUS, "DRVSTATUS"},
        {TMC5160_PWMCONF, "PWMCONF"},
        {TMC5160_PWMSCALE, "PWMSCALE"},
        {TMC5160_PWM_AUTO, "PWM_AUTO"},
        {TMC5160_LOST_STEPS, "LOST_STEPS"},
};
