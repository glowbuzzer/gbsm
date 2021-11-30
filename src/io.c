f/**
 ******************************************************************************
 * @file           :  io.c
 * @brief          :  handles the configuration and read and write of PI gpio io
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include <stdbool.h>
#include "io.h"
#include "pigpio.h"
#include "log.h"


/** array of size NUM_PI_GPIO (26+1) that defines the function of the pin and how it maps to the gbc io*/
io_map_t io_map[NUM_PI_GPIO] = {
        [1] = {GPIO_NOT_USED, 0},
        [26] = {GPIO_OUT, 3}
};


/**
 * @brief initialises pi gpio pins based on their defined function
 */
void io_init(void){
    int rc;

    for (unsigned int i = 0; i < NUM_PI_GPIO; i++) {
        if (io_map[i].function == GPIO_IN){
            rc = gpioSetMode(i, PI_INPUT);

        }else if (io_map[i].function == GPIO_OUT){
            rc = gpioSetMode(i, PI_OUTPUT);
        }else{
            //not used
        }

        if(rc == PI_BAD_GPIO || rc== PI_BAD_MODE){
            LL_ERROR(GBSM_GEN_LOG_EN, "GBSEM: Error setting GPIO function for Pi GPIO number [%d]", i);
        }
    }
}

/**
 * @brief sets the pi gpio pin corresponding to the gbc io number
 * @param gbc_dout_num
 * @param value
 * @param rc E_GPIO_FAILURE if write fails, E_SUCCESS, or E_NOT_FOUND
 */
void io_set_dout(uint8_t gbc_dout_num, bool value, gberror_t *rc){

    bool found = false;
    int gpio_rc = 0;
    for (unsigned int i = 0; i < NUM_PI_GPIO; i++) {
        if (io_map[i].function == GPIO_OUT && io_map[i].gbc_gpio_number == gbc_dout_num){
            gpio_rc = gpioWrite(i, (unsigned int) value);
            if(gpio_rc == PI_BAD_GPIO){
                *rc = E_GPIO_FAILURE;
                return;
            }

            found = true;
        }
    }

    if (found){
        *rc = E_SUCCESS;
    }else{
        *rc = E_NOT_FOUND;
    }

}
/**
 * @brief reads a pi gpio pin corresponding to the gbc io number
 * @param gbc_din_num
 * @param rc
 * @return E_GPIO_FAILURE if read fails, E_SUCCESS, or E_NOT_FOUND
 */
bool io_get_din(uint8_t gbc_din_num, gberror_t *rc){
    bool found = false;
    int read_result = false;
    for (unsigned int i = 0; i < NUM_PI_GPIO; i++) {
        if (io_map[i].function == GPIO_IN && io_map[i].gbc_gpio_number == gbc_din_num){
            read_result = gpioRead(i);
            if(read_result == PI_BAD_GPIO){
                *rc = E_GPIO_FAILURE;
                return false;
            }
            found = true;
        }
    }
    if (found){
        *rc = E_SUCCESS;
    }else{
        *rc = E_NOT_FOUND;
    }

    return (bool)read_result;

}
