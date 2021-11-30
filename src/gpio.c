
/**
 ******************************************************************************
 * @file           :  gpio.c
 * @brief          :  GPIO config functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */



#include "gpio.h"
#include "pigpio.h"
#include "log.h"
#include "user_message.h"
#include "drives.h"

void gpio_configure_TMC5160_and_TMC4361_pins(void){
    LL_INFO(GBSM_GEN_LOG_EN, "Configuring GPIO pins");
    /***** IO control of TMC4361A & TMC5160 *****/

    /***** TMC5160 *****/
    gpioSetMode(PIN_TMC5160_VCC_IO, PI_OUTPUT);
    gpioWrite(PIN_TMC5160_VCC_IO, 1); // TMC5160: Apply VCC_IO voltage

    gpioSetMode(PIN_TMC5160_CLK, PI_OUTPUT);
    gpioWrite(PIN_TMC5160_CLK, 0); // TMC5160: Use internal clock by tying CLK to GND

    gpioSetMode(PIN_TMC5160_DRV_ENN, PI_OUTPUT);
    gpioWrite(PIN_TMC5160_DRV_ENN, 0); // TMC5160: Enable driver stage

    /***** TMC4361A *****/

    gpioSetMode(PIN_TMC4361A_CLK, PI_ALT0);
//    gpioHardwareClock(PIN_TMC4361A_CLK, 9600000);  // TMC4361A: Set clock to 9.6MHz
    gpioHardwareClock(PIN_TMC4361A_CLK, 20000000);  // TMC4361A: Set clock to 9.6MHz

    gpioSetMode(PIN_TMC4361A_NRST, PI_OUTPUT);
    gpioWrite(PIN_TMC4361A_NRST, 1); // TMC4361A: Low active reset

    gpioSetMode(PIN_TMC4361A_START, PI_OUTPUT);
    gpioWrite(PIN_TMC4361A_START, 1); // TMC4361A: Per default, the voltage level transition from high to low triggers a start signal

    gpioSetMode(PIN_TMC4361A_SPI_CS, PI_OUTPUT);
    gpioWrite(PIN_TMC4361A_SPI_CS, 1); // TMC4361A: SPI CS is low active

}


void gpio_terminate_pigpio(int signum)
{
    UM_INFO(GBSM_UM_EN, "GBSM: signum received [%d]", signum);
    /* only registered for SIGHUP/SIGTERM/SIGINT */
    UM_INFO(GBSM_UM_EN, "GBSM: SIGINT/SIGHUP/SIGTERM received, we will clean up GPIO configuration and then exit");

    tmc4361a_terminate();

    gpioTerminate();


    exit(0);
}