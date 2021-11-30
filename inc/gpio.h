/**
 ******************************************************************************
 * @file           :  gpio.h
 * @brief          :  pin definitions for Pi and GPIO config functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_GPIO_H
#define GB_MONOREPO_GPIO_H

//Start pin - shared by TMC4351 daisy chain
#define PIN_TMC4361A_START      17
//Clock - shared by TMC4351 daisy chain
#define PIN_TMC4361A_CLK        4
//NRST - shared by TMC4351 daisy chain
#define PIN_TMC4361A_NRST       15

#define PIN_TMC4361A_SPI_CS     8
#define PIN_TMC4361A_SPI_CLK    11
#define PIN_TMC4361A_SPI_MOSI   10
#define PIN_TMC4361A_SPI_MISO   9




//pull clk to ground to enable internal clock
#define PIN_TMC5160_CLK         14
#define PIN_TMC5160_DRV_ENN     3

//VCC_IO takes very little current
#define PIN_TMC5160_VCC_IO      2


void gpio_configure_TMC5160_and_TMC4361_pins(void);
void gpio_terminate_pigpio(int signum);


#endif //GB_MONOREPO_GPIO_H
