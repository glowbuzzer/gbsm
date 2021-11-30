/**
 ******************************************************************************
 * @file           :  drives.h
 * @brief          :  drive control functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_DRIVES_H
#define GB_MONOREPO_DRIVES_H

#include "gberror.h"
#include "gbsm_config.h"
#include "stdbool.h"
#include "tmc4361a_status.h"

void drv_send_start_trigger(void);
void drv_set_ctrl_wrd(const uint16_t ctrl_wrd);
void drv_get_stat_wrd(uint16_t * stat_wrd);
void drv_get_actpos(int32_t *actpos);
void drv_set_setpos(const int32_t *setpos);
void drv_update_state(void);
gberror_t drv_init_spi_and_drivers(void);
void drv_set_enabled(void);
gberror_t drv_check_and_enable(void);
void test_motors_oscillating_move(void);
void tmc4361a_terminate(void);


typedef struct {
    int32_t v_stop_l;
    int32_t v_stop_r;
} virt_stop_t;

extern uint8_t tmc_status_from_spi[SM_NUM_DRIVES];

extern status_flag_t g_drv_status_flag[SM_NUM_DRIVES];
#endif //GB_MONOREPO_DRIVES_H
