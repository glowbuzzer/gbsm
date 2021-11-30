
/**
 ******************************************************************************
 * @file           :  tmc4361a_status.c
 * @brief          :  functions to interpret the tmc4261a status event and flags
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */
#include "tmc4361a_status.h"
#include "TMC4361A_Fields.h"
#include <stdint.h>
#include "std_defs_and_macros.h"
#include "log.h"



/**
 * @brief converts a tmc4361a event register into a struct of events
 * @param event_register
 * @param drive
 * @return
 */
status_event_t event_register_to_struct (uint32_t event_register, uint8_t drive){

    status_event_t event = {0};

    //0
    if (REGISTER_GET(event_register,TMC4361A_TARGET_REACHED_MASK, TMC4361A_TARGET_REACHED_SHIFT)==1){
        event.target_reached = 1;
    }

    //1
    if (REGISTER_GET(event_register,TMC4361A_POS_COMP_REACHED_MASK,TMC4361A_POS_COMP_REACHED_SHIFT )==1){
        event.pos_comp_reached = 1;
    }

    //2
    if (REGISTER_GET(event_register,TMC4361A_VEL_REACHED_MASK,TMC4361A_VEL_REACHED_SHIFT )==1){
        event.vactual_eq_vmax = 1;
    }

    //3
    if (REGISTER_GET(event_register,TMC4361A_VEL_STATE_00_MASK, TMC4361A_VEL_STATE_00_SHIFT)==1){
        event.vactual_eq_zero = 1;
    }

    //4
    if (REGISTER_GET(event_register,TMC4361A_VEL_STATE_01_MASK,TMC4361A_VEL_STATE_01_SHIFT )==1){
        event.vactual_less_zero = 1;
    }

    //5
    if (REGISTER_GET(event_register,TMC4361A_VEL_STATE_10_MASK,TMC4361A_VEL_STATE_10_SHIFT )==1){
        event.vactual_less_zero = 1;
    }
    //6
    if (REGISTER_GET(event_register,TMC4361A_RAMP_STATE_00_MASK,TMC4361A_RAMP_STATE_00_SHIFT )==1){
        event.aactual_eq_zero = 1;
    }

    //7
    if (REGISTER_GET(event_register,TMC4361A_RAMP_STATE_01_MASK,TMC4361A_RAMP_STATE_01_SHIFT )==1){
        event.aactual_greater_zero = 1;
    }

    //8
    if (REGISTER_GET(event_register,TMC4361A_RAMP_STATE_10_MASK,TMC4361A_RAMP_STATE_10_SHIFT )==1){
        event.aactual_less_zero = 1;
    }

    //9
    if (REGISTER_GET(event_register,TMC4361A_MAX_PHASE_TRAP_MASK,TMC4361A_MAX_PHASE_TRAP_SHIFT )==1){
        event.max_phase_trap = 1;
    }

    //10
    if (REGISTER_GET(event_register,TMC4361A_FROZEN_MASK,TMC4361A_FROZEN_SHIFT )==1){
        event.frozen = 1;
    }

    //11
    if (REGISTER_GET(event_register,TMC4361A_STOPL_EVENT_MASK, TMC4361A_STOPL_EVENT_SHIFT)==1){
        event.l_stop = 1;
    }

    //12
    if (REGISTER_GET(event_register,TMC4361A_STOPR_EVENT_MASK,TMC4361A_STOPR_EVENT_SHIFT )==1){
        event.r_stop = 1;
    }

    //13
    if (REGISTER_GET(event_register,TMC4361A_VSTOPL_ACTIVE_MASK,TMC4361A_VSTOPL_ACTIVE_SHIFT )==1){
        event.l_vstop = 1;
    }

    //14
    // EVENTS // Right virtual switch was triggered
    // min.: 0, max.: 1, default: 0
    if (REGISTER_GET(event_register,TMC4361A_VSTOPR_ACTIVE_MASK,TMC4361A_VSTOPR_ACTIVE_SHIFT )==1){
        event.r_vstop = 1;
    }

    //15
    if (REGISTER_GET(event_register,TMC4361A_HOME_ERROR_MASK,TMC4361A_HOME_ERROR_SHIFT )==1){
        event.home_error = 1;
    }

    //16
    if (REGISTER_GET(event_register,TMC4361A_XLATCH_DONE_MASK,TMC4361A_XLATCH_DONE_SHIFT )==1){
        event.enc_latch = 1;
    }

    //17
    if (REGISTER_GET(event_register,TMC4361A_FS_ACTIVE_MASK,TMC4361A_FS_ACTIVE_SHIFT )==1){
        event.fs_active = 1;
    }

    //18
    if (REGISTER_GET(event_register,TMC4361A_ENC_FAIL_MASK,TMC4361A_ENC_FAIL_SHIFT )==1){
        event.enc_fail = 1;
    }

    //19
    if (REGISTER_GET(event_register,TMC4361A_N_ACTIVE_MASK,TMC4361A_N_ACTIVE_SHIFT )==1){
        event.n_active = 1;
    }

    //20
    if (REGISTER_GET(event_register,TMC4361A_ENC_DONE_MASK,TMC4361A_ENC_DONE_SHIFT )==1){
        event.enc_latch = 1;
    }

    //21
#define TMC4361A_SER_ENC_DATA_FAIL_MASK                   0x200000U // EVENTS // Serial encoder data fail
#define TMC4361A_SER_ENC_DATA_FAIL_SHIFT                  21U // min.: 0, max.: 1, default: 0
    if (REGISTER_GET(event_register,TMC4361A_SER_ENC_DATA_FAIL_MASK,TMC4361A_SER_ENC_DATA_FAIL_SHIFT )==1){
        event.abs_enc_fail = 1;
    }

    //23
    if (REGISTER_GET(event_register,TMC4361A_SER_DATA_DONE_MASK,TMC4361A_SER_DATA_DONE_SHIFT )==1){
        event.ser_data_done = 1;
    }

    //24
    if (REGISTER_GET(event_register,TMC4361A_SERIAL_ENC_FLAGS_MASK,TMC4361A_SERIAL_ENC_FLAGS_SHIFT )==1){
        event.set_enc_flag_set = 1;
    }

    //25
    if (REGISTER_GET(event_register,TMC4361A_COVER_DONE_MASK,TMC4361A_COVER_DONE_SHIFT )==1){
        event.cover_done = 1;
    }

    //26
    if (REGISTER_GET(event_register,TMC4361A_ENC_VEL0_MASK,TMC4361A_ENC_VEL0_SHIFT )==1){
        event.enc_velo_eq_zero = 1;
    }

    //27
    if (REGISTER_GET(event_register,TMC4361A_CL_MAX_MASK,TMC4361A_CL_MAX_SHIFT )==1){
        event.cl_max = 1;
    }


    //28
    if (REGISTER_GET(event_register,TMC4361A_CL_FIT_MASK,TMC4361A_CL_FIT_SHIFT )==1){
        event.cl_fit = 1;
    }

    //29
    //#define TMC4361A_STOP_ON_STALL_MASK                     0x20000000U // EVENTS // Motor stall detected. Motor was stopped
//#define TMC4361A_STOP_ON_STALL_SHIFT                    29U // min.: 0, max.: 1, default: 0
    if (REGISTER_GET(event_register,TMC4361A_STOP_ON_STALL_MASK,TMC4361A_STOP_ON_STALL_SHIFT )==1){
        event.stop_on_stall = 1;
    }

    //30
    if (REGISTER_GET(event_register,TMC4361A_MOTOR_EV_MASK,TMC4361A_MOTOR_EV_SHIFT )==1){
        event.motor_flag_set = 1;
    }

    //31

    if (REGISTER_GET(event_register,TMC4361A_RST_EV_MASK,TMC4361A_RST_EV_SHIFT )==1){
        event.reset_triggered = 1;
    }

    return event;

}


/**
 * @brief converts a tmc4361a flag register into a struct of flgas
 * @param flag_register
 * @param drive
 * @return
 */
status_flag_t flag_register_to_struct (uint32_t flag_register, uint8_t drive){

    status_flag_t flag = {0};

    //0
    if (REGISTER_GET(flag_register,TMC4361A_TARGET_REACHED_F_MASK, TMC4361A_TARGET_REACHED_F_SHIFT)==1){
        flag.target_reached = 1;
    }

    //1
    if (REGISTER_GET(flag_register,TMC4361A_POS_COMP_REACHED_F_MASK, TMC4361A_POS_COMP_REACHED_F_SHIFT)==1){
        flag.pos_comp_reached = 1;
    }

    //2
    if (REGISTER_GET(flag_register,TMC4361A_VEL_REACHED_F_MASK, TMC4361A_VEL_REACHED_F_SHIFT)==1){
        flag.vactual_eq_vmax = 1;
    }

    if (REGISTER_GET(flag_register,TMC4361A_VEL_STATE_F_MASK, TMC4361A_VEL_STATE_F_SHIFT)==0){
        flag.vactual_eq_zero = 1;
    }
    if (REGISTER_GET(flag_register,TMC4361A_VEL_STATE_F_MASK, TMC4361A_VEL_STATE_F_SHIFT)==1){
        flag.vactual_greater_zero = 1;
    }
    if (REGISTER_GET(flag_register,TMC4361A_VEL_STATE_F_MASK, TMC4361A_VEL_STATE_F_SHIFT)==2){
        flag.vactual_less_zero = 1;
    }

    if (REGISTER_GET(flag_register,TMC4361A_RAMP_STATE_F_MASK, TMC4361A_RAMP_STATE_F_SHIFT)==0){
        flag.aactual_eq_zero = 1;
    }

    if (REGISTER_GET(flag_register,TMC4361A_RAMP_STATE_F_MASK, TMC4361A_RAMP_STATE_F_SHIFT)==1){
        flag.aactual_greater_zero = 1;
    }

    if (REGISTER_GET(flag_register,TMC4361A_RAMP_STATE_F_MASK, TMC4361A_RAMP_STATE_F_SHIFT)==2){
        flag.aactual_less_zero = 1;
    }

    //7
    if (REGISTER_GET(flag_register,TMC4361A_STOPL_ACTIVE_F_MASK, TMC4361A_STOPL_ACTIVE_F_SHIFT)==1){
        flag.l_stop = 1;
//        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [L STOP] on drive [%u]", drive);
    }

    //8
    if (REGISTER_GET(flag_register,TMC4361A_STOPR_ACTIVE_F_MASK, TMC4361A_STOPR_ACTIVE_F_SHIFT)==1){
        flag.r_stop = 1;
//        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [R STOP] on drive [%u]", drive);

    }

    //9
    if (REGISTER_GET(flag_register,TMC4361A_VSTOPL_ACTIVE_F_MASK, TMC4361A_VSTOPL_ACTIVE_F_SHIFT)==1){
        flag.l_vstop = 1;
        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [L VSTOP] on drive [%u]", drive);

    }

    //10
    if (REGISTER_GET(flag_register,TMC4361A_VSTOPR_ACTIVE_F_MASK, TMC4361A_VSTOPR_ACTIVE_F_SHIFT)==1){
        flag.r_vstop = 1;
        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [R VSTOP] on drive [%u]", drive);

    }

    //11
    if (REGISTER_GET(flag_register,TMC4361A_ACTIVE_STALL_F_MASK, TMC4361A_ACTIVE_STALL_F_SHIFT)==1){
        flag.stall = 1;
        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [STALL] on drive [%u]", drive);

    }

    //12
    if (REGISTER_GET(flag_register,TMC4361A_HOME_ERROR_F_MASK, TMC4361A_HOME_ERROR_F_SHIFT)==1){
        flag.home_error = 1;
        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [HOME ERROR] on drive [%u]", drive);

    }

    //13
    if (REGISTER_GET(flag_register,TMC4361A_FS_ACTIVE_F_MASK, TMC4361A_FS_ACTIVE_F_SHIFT)==1){
        flag.fs_active = 1;

    }

    //14
    if (REGISTER_GET(flag_register,TMC4361A_ENC_FAIL_F_MASK, TMC4361A_ENC_FAIL_F_SHIFT)==1){
        flag.enc_fail = 1;
//        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [ENCODER FAIL] on drive [%u]", drive);

    }

    //15
    if (REGISTER_GET(flag_register,TMC4361A_N_ACTIVE_F_MASK, TMC4361A_N_ACTIVE_F_SHIFT)==1){
        flag.n_active = 1;
    }

    //16
    if (REGISTER_GET(flag_register,TMC4361A_ENC_LATCH_F_MASK, TMC4361A_ENC_LATCH_F_SHIFT)==1){
        flag.enc_latch = 1;
    }

    //17
    if (REGISTER_GET(flag_register,TMC4361A_MULTI_CYCLE_FAIL_F__SER_ENC_VAR_F_MASK, TMC4361A_MULTI_CYCLE_FAIL_F__SER_ENC_VAR_F_SHIFT)==1){
        flag.abs_enc_fail = 1;
        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [ABSOLUTE ENCODER FAIL] on drive [%u]", drive);

    }

    //18 - reserved

    //19
    if (REGISTER_GET(flag_register,TMC4361A_CL_FIT_MASK, TMC4361A_CL_FIT_SHIFT)==1){
        flag.pos_dev = 1;
    }

    //20:23 SERIAL_ENC_FLAGS from encoder

    //24
    if (REGISTER_GET(flag_register,TMC4361A_STATUS_SG_MASK, TMC4361A_STATUS_SG_SHIFT)==1){
        flag.stallguard_active = 1;
        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [STALLGUARD] on drive [%u]", drive);

    }

    //25
    if (REGISTER_GET(flag_register,TMC4361A_STATUS_OT_MASK, TMC4361A_STATUS_OT_SHIFT)==1){
        flag.overtemperature_shutdown = 1;
        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [OVER TEMPERATURE SHUTDOWN] on drive [%u]", drive);

    }


    //26
    if (REGISTER_GET(flag_register,TMC4361A_STATUS_OTPW_MASK, TMC4361A_STATUS_OTPW_SHIFT)==1){
        flag.overtemperature_warning= 1;
        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [OVER TEMPERATURE WARNING] on drive [%u]", drive);

    }

    //27
    if (REGISTER_GET(flag_register,TMC4361A_STATUS_S2GA_MASK, TMC4361A_STATUS_S2GA_SHIFT)==1){
        flag.short_to_ground_a= 1;
        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [SHORT TO GROUND A] on drive [%u]", drive);

    }

    //28
    if (REGISTER_GET(flag_register,TMC4361A_STATUS_S2GB_MASK, TMC4361A_STATUS_S2GB_SHIFT)==1){
        flag.short_to_ground_b= 1;
        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [SHORT TO GROUND B] on drive [%u]", drive);

    }

    //29
    if (REGISTER_GET(flag_register,TMC4361A_STATUS_OLA_MASK, TMC4361A_STATUS_OLA_SHIFT)==1){
        flag.open_load_a= 1;
//        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [OPEN LOAD A] on drive [%u]", drive);

    }

    //30
    if (REGISTER_GET(flag_register,TMC4361A_STATUS_OLB_MASK, TMC4361A_STATUS_OLB_SHIFT)==1){
        flag.open_load_b= 1;
//        LL_ERROR(GBSM_DRIVE_STATUS_ERROR_LOG_EN, "GBSM: Driver error (flag) [OPEN LOAD B] on drive [%u]", drive);

    }

    //31
    if (REGISTER_GET(flag_register,TMC4361A_STATUS_STST_MASK, TMC4361A_STATUS_STST_SHIFT)==1){
        flag.standstill= 1;
    }




    return flag;
}

