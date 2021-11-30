
/**
 ******************************************************************************
 * @file           :  stepper_routines.c
 * @brief          :  programs to peform misc. functions e.g. test moves, tuning
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "SPI_TMC.h"
#include "TMC5160_Register.h"
#include "TMC5160_Fields.h"
#include "TMC5160_Constants.h"
#include "TMC4361A_Fields.h"
#include "TMC4361A_Register.h"
#include "TMC4361A_Constants.h"
#include <sys/time.h>
#include <stdio.h>
#include "gbsm_config.h"
#include "pigpio.h"
#include "std_defs_and_macros.h"
#include "user_message.h"

uint64_t current_timestamp(void) {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    uint64_t milliseconds = (uint64_t)(te.tv_sec * 1000LL + te.tv_usec / 1000); // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}


void test_motors_oscillating_move(void) {

    uint8_t spi_status;

    UM_INFO(GBSM_UM_EN, "GBSM: **************************************************************************");
    UM_INFO(GBSM_UM_EN, "GBSM: ***                      GB Test oscillating move                      ***");
    UM_INFO(GBSM_UM_EN, "GBSM: **************************************************************************");

    // DIRECT_ACC_VAL_EN=1, DIRECT_BOW_VAL_EN=1, STDBY_CLK_PIN_ASSIGNMENT=3
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_GENERAL_CONF, 0x00006006, SM_NUM_DRIVES, &hspi1, &spi_status);

    //MOTION_PROFILE = 2 (s-shaped ramp), OPERATION_MODE = 0 (velocity mode)
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_RAMPMODE, 0b110, SM_NUM_DRIVES, &hspi1, &spi_status);

    // Values for speed and acceleration of 1st profile
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_AMAX, 0x00000100, SM_NUM_DRIVES, &hspi1, &spi_status); // AMAX = 4096 pps²
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_DMAX, 0x00000100, SM_NUM_DRIVES, &hspi1, &spi_status); // DMAX = 4096 pps²
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_BOW1, 0x00000020, SM_NUM_DRIVES, &hspi1, &spi_status); // BOW1 = 512 pps³
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_BOW2, 0x00000020, SM_NUM_DRIVES, &hspi1, &spi_status); // BOW2 = 512 pps³
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_BOW3, 0x00000020, SM_NUM_DRIVES, &hspi1, &spi_status); // BOW3 = 512 pps³
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_BOW4, 0x00000020, SM_NUM_DRIVES, &hspi1, &spi_status); // BOW4 = 512 pps³

    tmc4361A_write_uint32_to_all_same_val(TMC4361A_VMAX, 0x08F40000, SM_NUM_DRIVES, &hspi1, &spi_status); // VMAX = 128k pps = 2.5 RPS (1.8° motor)

    tmc4361A_write_uint32_to_all_same_val(TMC4361A_START_CONF, 0b100000000000001000001000000, SM_NUM_DRIVES, &hspi1, &spi_status);


    tmc4361A_write_uint32_to_all_same_val(TMC4361A_X_PIPE0, 0, SM_NUM_DRIVES, &hspi1, &spi_status);
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_X_PIPE1, 256000, SM_NUM_DRIVES, &hspi1, &spi_status);
    tmc4361A_write_int32_to_all_same_val(TMC4361A_X_TARGET, 256000, SM_NUM_DRIVES, &hspi1, &spi_status);


    static uint64_t ms_tick = 0;
    static uint64_t last_ms_tick = 0;
    printf("sg_result == 0 = stall\n");

    uint32_t drvstatus[SM_NUM_DRIVES] = {0};
    uint32_t sg_result[SM_NUM_DRIVES] = {0};
    uint32_t cs_actual[SM_NUM_DRIVES] = {0};

    while (1) {
        ms_tick = current_timestamp();
        bool sg_status[SM_NUM_DRIVES];
        uint32_t lost_steps[SM_NUM_DRIVES] = {0};

        if ((ms_tick - last_ms_tick) > 100) {
            last_ms_tick = ms_tick;

            gpioDelay(1000);
            tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_DRVSTATUS, SM_NUM_DRIVES, &hspi1, &spi_status);
            gpioDelay(1000);
            tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_LOW_WR, 0, SM_NUM_DRIVES, &hspi1, &spi_status);
            gpioDelay(1000);
            tmc4361A_read_uint32_all(TMC4361A_COVER_DRV_LOW_RD, drvstatus, SM_NUM_DRIVES, &hspi1);

//            printBits(4, drvstatus);



            gpioDelay(1000);

            tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_LOST_STEPS, SM_NUM_DRIVES, &hspi1, &spi_status);
            gpioDelay(1000);
            tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_LOW_WR, 0, SM_NUM_DRIVES, &hspi1, &spi_status);
            gpioDelay(1000);
            tmc4361A_read_uint32_all(TMC4361A_COVER_DRV_LOW_RD, lost_steps, SM_NUM_DRIVES, &hspi1);
//            gpioDelay(1000);
//            tmc4361A_read_uint32_all(TMC4361A_COVER_DRV_LOW_RD, lost_steps, SM_NUM_DRIVES, &hspi1);


            for (int i = 0; i < SM_NUM_DRIVES ; i++) {
                sg_result[i] = REGISTER_GET(drvstatus[i], TMC5160_SG_RESULT_MASK, TMC5160_SG_RESULT_SHIFT);
                cs_actual[i] = REGISTER_GET(drvstatus[i], TMC5160_CS_ACTUAL_MASK, TMC5160_CS_ACTUAL_SHIFT);

                sg_status[i] = REGISTER_GET(drvstatus[i], TMC5160_STALLGUARD_MASK, TMC5160_STALLGUARD_SHIFT);

                printf("sg_result [%u], cs_actual [%u], sg_status [%u], lost step [%u]\n", sg_result[i], cs_actual[i], sg_status[i], lost_steps[i]);
            }
//            print_tmc4361a_reg(TMC4361A_XACTUAL);

        }


    }

}


void calibrate_spread_cycle(void) {
    uint8_t spi_status;

    UM_INFO(GBSM_UM_EN, "GBSM: **************************************************************************");
    UM_INFO(GBSM_UM_EN, "GBSM: ***                      GB Calibrate Spread Cycle                     ***");
    UM_INFO(GBSM_UM_EN, "GBSM: **************************************************************************");

//    struct {
//        uint8_t blank_time = 24;        // [16, 24, 36, 54]
//        uint8_t off_time = 3;           // [1..15]
//        uint8_t hysteresis_start = 1;   // [1..8]
//        int8_t hysteresis_end = 12;     // [-3..12]
//    } config;


//    UM_INFO(GBSM_UM_EN, "GBSM: Setting driver blank time to [] cycles");
//    driver.blank_time(config.blank_time);

//    UM_INFO(GBSM_UM_EN, "GBSM: Setting driver off time to []");
//    driver.off_time(config.off_time);

//    UM_INFO(GBSM_UM_EN, "GBSM: Setting hysteresis end value to []");
//    driver.hysteresis_end(config.hysteresis_end);

//    UM_INFO(GBSM_UM_EN, "GBSM: Setting hysteresis start value to []");
//    driver.hysteresis_start(config.hysteresis_start);

    UM_INFO(GBSM_UM_EN, "GBSM: Effective hysteresis = []");
//    Serial.println(config.hysteresis_end + config.hysteresis_start);


//    if (config.hysteresis_end == 12) Serial.println(F("Reached MAX setting already!"));
//    if (config.hysteresis_end == -3) Serial.println(F("Reached MIN setting already!"));

    // DIRECT_ACC_VAL_EN=1, DIRECT_BOW_VAL_EN=1, STDBY_CLK_PIN_ASSIGNMENT=3
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_GENERAL_CONF, 0x00006006, SM_NUM_DRIVES, &hspi1, &spi_status);

    //MOTION_PROFILE = 2 (s-shaped ramp), OPERATION_MODE = 0 (velocity mode)
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_RAMPMODE, 0b110, SM_NUM_DRIVES, &hspi1, &spi_status);

    // Values for speed and acceleration of 1st profile
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_AMAX, 0x00000100, SM_NUM_DRIVES, &hspi1, &spi_status); // AMAX = 4096 pps²
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_DMAX, 0x00000100, SM_NUM_DRIVES, &hspi1, &spi_status); // DMAX = 4096 pps²
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_BOW1, 0x00000020, SM_NUM_DRIVES, &hspi1, &spi_status); // BOW1 = 512 pps³
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_BOW2, 0x00000020, SM_NUM_DRIVES, &hspi1, &spi_status); // BOW2 = 512 pps³
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_BOW3, 0x00000020, SM_NUM_DRIVES, &hspi1, &spi_status); // BOW3 = 512 pps³
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_BOW4, 0x00000020, SM_NUM_DRIVES, &hspi1, &spi_status); // BOW4 = 512 pps³

//    tmc4361A_write_uint32_to_all_same_val(TMC4361A_VMAX, 0x08F40000, SM_NUM_DRIVES, &hspi1, &spi_status); // VMAX = 128k pps = 2.5 RPS (1.8° motor)
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_VMAX, 0x06F40000, SM_NUM_DRIVES, &hspi1, &spi_status); // VMAX = 128k pps = 2.5 RPS (1.8° motor)


    tmc4361A_write_uint32_to_all_same_val(TMC4361A_START_CONF, 0b100000000000001000001000000, SM_NUM_DRIVES, &hspi1, &spi_status);


    tmc4361A_write_uint32_to_all_same_val(TMC4361A_X_PIPE0, 0, SM_NUM_DRIVES, &hspi1, &spi_status);
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_X_PIPE1, 2560000, SM_NUM_DRIVES, &hspi1, &spi_status);
    tmc4361A_write_int32_to_all_same_val(TMC4361A_X_TARGET, 2560000, SM_NUM_DRIVES, &hspi1, &spi_status);


    static uint64_t ms_tick = 0;
    static uint64_t last_ms_tick = 0;

    uint32_t chop_conf[SM_NUM_DRIVES];
    uint8_t hend[SM_NUM_DRIVES];

    while (1) {
        ms_tick = current_timestamp();




        if ((ms_tick - last_ms_tick) > 10000) {
            last_ms_tick = ms_tick;

            tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_CHOPCONF, SM_NUM_DRIVES, &hspi1, &spi_status);
            gpioDelay(1000);
            tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_LOW_WR, 0, SM_NUM_DRIVES, &hspi1, &spi_status);
            gpioDelay(1000);
            tmc4361A_read_uint32_all(TMC4361A_COVER_DRV_LOW_RD, chop_conf, SM_NUM_DRIVES, &hspi1);



            for (int i = 0; i < SM_NUM_DRIVES; i++) {

                hend[i] = REGISTER_GET(chop_conf[i],TMC5160_HEND_MASK, TMC5160_HEND_SHIFT );

                if (hend[i]==12){

                }else{
                    hend[i]++;
                    chop_conf[i] = REGISTER_SET(chop_conf[i],TMC5160_HEND_MASK, TMC5160_HEND_SHIFT, hend[i]);
                }

                    UM_INFO(GBSM_UM_EN, "GBSM: Setting hysteresis end value to [%u] on driver [%d]", hend[i], i);


            }

            tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_HIGH_WR, TMC5160_CHOPCONF | 0x80, SM_NUM_DRIVES, &hspi1, &spi_status);
            tmc4361A_write_uint32_to_all_different_vals(TMC4361A_COVER_LOW_WR, chop_conf, SM_NUM_DRIVES, &hspi1, &spi_status);

            for (int i = 0; i < SM_NUM_DRIVES; i++) {
                hend[i]=0;
            }


        }
    }


}

