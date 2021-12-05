/**
 ******************************************************************************
 * @file           :  io.h
 * @brief          :  io handling functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_IO_H
#define GB_MONOREPO_IO_H

#include <stdint.h>
#include "gberror.h"



#define NUM_PI_GPIO 27
typedef enum {GPIO_NOT_USED, GPIO_IN, GPIO_OUT} gpio_function_t;

typedef struct {
    gpio_function_t function;
    uint8_t gbc_gpio_number;
}io_map_t;



#define CTRL_ESTOP_DIN 0
#define CTRL_ESTOP_RESET_DIN 1


void io_init(void);
bool io_get_din(uint8_t gbc_din_num, gberror_t *rc);
void io_set_dout(uint8_t gbc_dout_num, bool value, gberror_t *rc);

#endif //GB_MONOREPO_IO_H
