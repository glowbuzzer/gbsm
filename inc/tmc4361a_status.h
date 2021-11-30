/**
 ******************************************************************************
 * @file           :  tmc4361a_status.h
 * @brief          :  functions to interpret the tmc4261a status event and flags
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_TMC4361A_STATUS_H
#define GB_MONOREPO_TMC4361A_STATUS_H

#include <stdint.h>

typedef struct{
    uint32_t target_reached:1;
    uint32_t pos_comp_reached:1;
    uint32_t vactual_eq_vmax:1;
    uint32_t vactual_eq_zero:1;
    uint32_t vactual_greater_zero:1;
    uint32_t vactual_less_zero:1;
    uint32_t aactual_eq_zero:1;
    uint32_t aactual_greater_zero:1;
    uint32_t aactual_less_zero:1;
    uint32_t l_stop:1;
    uint32_t r_stop:1;
    uint32_t l_vstop:1;
    uint32_t r_vstop:1;
    uint32_t stall:1;
    uint32_t home_error:1;
    uint32_t fs_active:1;
    uint32_t enc_fail:1;
    uint32_t n_active:1;
    uint32_t enc_latch:1;
    uint32_t abs_enc_fail:1;
    uint32_t pos_dev:1;
    uint32_t stallguard_active:1;
    uint32_t undervoltage:1;
    uint32_t overtemperature_shutdown:1;
    uint32_t overtemperature_warning:1;
    uint32_t short_to_ground_a:1;
    uint32_t over_current_a:1;
    uint32_t short_to_ground_b:1;
    uint32_t over_current_b:1;
    uint32_t open_load_a:1;
    uint32_t open_load_b:1;
    uint32_t standstill:1;

}status_flag_t;

typedef struct{
    uint32_t target_reached:1;
    uint32_t pos_comp_reached:1;
    uint32_t vactual_eq_vmax:1;
    uint32_t vactual_eq_zero:1;
    uint32_t vactual_greater_zero:1;
    uint32_t vactual_less_zero:1;
    uint32_t aactual_eq_zero:1;
    uint32_t aactual_greater_zero:1;
    uint32_t aactual_less_zero:1;
    uint32_t max_phase_trap:1;
    uint32_t frozen:1;
    uint32_t l_stop:1;
    uint32_t r_stop:1;
    uint32_t l_vstop:1;
    uint32_t r_vstop:1;
    uint32_t home_error:1;
    uint32_t latch_done:1;
    uint32_t fs_active:1;
    uint32_t enc_fail:1;
    uint32_t n_active:1;
    uint32_t enc_latch:1;
    uint32_t abs_enc_fail:1;
    uint32_t ser_data_done:1;
    uint32_t set_enc_flag_set:1;
    uint32_t cover_done:1;
    uint32_t enc_velo_eq_zero:1;
    uint32_t overtemperature_warning:1;
    uint32_t cl_max:1;
    uint32_t cl_fit:1;
    uint32_t stop_on_stall:1;
    uint32_t motor_flag_set:1;
    uint32_t over_current_b:1;
    uint32_t reset_triggered:1;
}status_event_t;

status_flag_t flag_register_to_struct (uint32_t flag_register, uint8_t drive);
status_event_t event_register_to_struct (uint32_t event_register, uint8_t drive);


#endif //GB_MONOREPO_TMC4361A_STATUS_H
