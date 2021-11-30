
/**
 ******************************************************************************
 * @file           :  read_register.c
 * @brief          :  register print functions for TMC5160 or TMC4361A
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "log.h"
#include "user_message.h"
#include "TMC5160_Register.h"
#include "TMC4361A_Register.h"
#include "stdbool.h"
#include "SPI_TMC.h"
#include "pigpio.h"
#include "gbsm_config.h"
#include "std_utils.h"
#include "read_register.h"

typedef enum {IC_NONE, IC_5160, IC_4361A} ic_type_t;

/**
 * @brief interactive register read from TMC5160 or TMC4361A
 */
void interactive_reg_print(void){

    int register_to_read, ic_choice;
    char cont_choice;
    ic_type_t ic_type = IC_NONE;

    bool register_found = false;

    start:

    UM_INFO(GBSM_UM_EN, "GBSM: **************************************************************************");
    UM_INFO(GBSM_UM_EN, "GBSM: ***                GB Stepper Controller Register Read                 ***");
    UM_INFO(GBSM_UM_EN, "GBSM: **************************************************************************");

    UM_INFO(GBSM_UM_EN, "GBSM: Read a register from the Trinamic ICs (with their initialised values)");
    UM_INFO(GBSM_UM_EN, "GBSM: Would you like to read a TMC5160 or TMC4361A register?");
    UM_INFO(GBSM_UM_EN, "GBSM: Enter 1 for TMC5160");
    UM_INFO(GBSM_UM_EN, "GBSM: Enter 2 for TMC4361A");

    if (scanf("%d", &ic_choice) == 1){
        if (ic_choice==1 || ic_choice==2){
            if(ic_choice==1){
                UM_INFO(GBSM_UM_EN, "GBSM: TMC5160 chosen");
                ic_type = IC_5160;
            }else{
                UM_INFO(GBSM_UM_EN, "GBSM: TMC4361A chosen");
                ic_type= IC_4361A;
            }
        }else{
            UM_ERROR(GBSM_UM_EN, "Invalid option");
            goto start;
        }
    }else{
        UM_ERROR(GBSM_UM_EN, "GBSM: Invalid entry");
        goto start;
    }

    enter_reg:


    UM_INFO(GBSM_UM_EN, "GBSM: Enter the value of the register to check, in lower case hex without the 0x:");
    if (scanf("%x", &register_to_read) ==1){

        if (ic_type == IC_5160){

            for (int i = 0; i < NUM_TMC5160_REGISTERS; i++) {
                if (tmc5160_register[i].address == register_to_read){
                    UM_INFO(GBSM_UM_EN, "GBSM: Register found [%s]", tmc5160_register[i].name);
                    register_found = true;
                }
            }
            if (!register_found){
                UM_ERROR(GBSM_UM_EN, "GBSM: We couldn't match the register entered with any valid TMC5160 registers");
                goto enter_reg;
            }

        }else{
            for (int i = 0; i < NUM_TMC4361A_REGISTERS; i++) {
                if (tmc4361a_register[i].address == register_to_read){
                    UM_INFO(GBSM_UM_EN, "GBSM: Register found [%s]", tmc4361a_register[i].name);
                    if (tmc4361a_register[i].write){
                        UM_ERROR(GBSM_UM_EN, "GBSM: Register found [%s] but it is write only", tmc4361a_register[i].name);
                    }else {
                        register_found = true;
                    }
                }
            }
            if (!register_found){
                UM_ERROR(GBSM_UM_EN, "GBSM: We couldn't match the register entered with any valid readable TMC4361A registers");
                goto enter_reg;
            }

        }


    }else{
        UM_ERROR(GBSM_UM_EN, "Invalid option");
        goto enter_reg;

    }

    uint8_t spi_status;
    uint32_t reg_result[SM_NUM_DRIVES];

    if (ic_type == IC_5160) {
        tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_HIGH_WR, (uint32_t) register_to_read, SM_NUM_DRIVES, &hspi1, &spi_status);
        gpioDelay(1000);
        tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_LOW_WR, 0, SM_NUM_DRIVES, &hspi1, &spi_status);
        gpioDelay(1000);
        tmc4361A_read_uint32_all(TMC4361A_COVER_DRV_LOW_RD, reg_result, SM_NUM_DRIVES, &hspi1);
//        gpioDelay(1000);
//        tmc4361A_read_uint32_all(TMC4361A_COVER_DRV_LOW_RD, reg_result, SM_NUM_DRIVES, &hspi1);

    }else{

        tmc4361A_read_uint32_all((uint32_t)register_to_read, reg_result, SM_NUM_DRIVES, &hspi1);

    }

    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        print_register_32(&reg_result[i]);
 }

    UM_INFO(GBSM_UM_EN, "GBSM: Do you want to read another register? Enter y or n");
    if (scanf(" %c", &cont_choice) ==1) {
        if (cont_choice == 'y' || cont_choice == 'Y') {
            goto start;
        } else if (cont_choice == 'n' || cont_choice == 'N') {
            exit(0);
        } else {
            UM_ERROR(GBSM_UM_EN, "Invalid option we will exit");
            exit(0);
        }

    }else{
        UM_ERROR(GBSM_UM_EN, "GBSM: Invalid entry");
        exit(0);
    }



exit(0);


}


/**
 * @brief print a single TMC5160 register contents
 * @param register_to_read
 * @return
 */
gberror_t print_tmc5160_reg(int register_to_read){

    bool register_found = false;
    uint8_t spi_status;

    for (int i = 0; i < NUM_TMC5160_REGISTERS; i++) {
        if (tmc5160_register[i].address == register_to_read) {
            UM_INFO(GBSM_UM_EN, "GBSM: Register found [%s]", tmc5160_register[i].name);
            register_found = true;
        }
    }
    if (!register_found){
        UM_ERROR(GBSM_UM_EN, "GBSM: We couldn't match the register entered with any valid TMC5160 registers");
        return E_NOT_FOUND;
    }

    uint32_t reg_result[SM_NUM_DRIVES];

    tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_HIGH_WR, (uint32_t) register_to_read, SM_NUM_DRIVES, &hspi1, &spi_status);
    gpioDelay(1000);
    tmc4361A_write_uint32_to_all_same_val(TMC4361A_COVER_LOW_WR, 0, SM_NUM_DRIVES, &hspi1, &spi_status);
    gpioDelay(1000);
    tmc4361A_read_uint32_all(TMC4361A_COVER_DRV_LOW_RD, reg_result, SM_NUM_DRIVES, &hspi1);
//    gpioDelay(1000);
//    tmc4361A_read_uint32_all(TMC4361A_COVER_DRV_LOW_RD, reg_result, SM_NUM_DRIVES, &hspi1);

    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        print_register_32(&reg_result[i]);
    }
    return E_SUCCESS;

}

/**
 * @brief print a single TMC4361a register
 * @param register_to_read
 * @return
 */
gberror_t print_tmc4361a_reg(int register_to_read ){

    bool register_found = false;
    uint8_t spi_status;

    for (int i = 0; i < NUM_TMC4361A_REGISTERS; i++) {
        if (tmc4361a_register[i].address == register_to_read){
            UM_INFO(GBSM_UM_EN, "GBSM: Register found [%s]", tmc4361a_register[i].name);
            if (tmc4361a_register[i].write){
                UM_ERROR(GBSM_UM_EN, "GBSM: Register found [%s] but it is write only", tmc4361a_register[i].name);
            }else {
                register_found = true;
            }
        }
    }
    if (!register_found){
        UM_ERROR(GBSM_UM_EN, "GBSM: We couldn't match the register entered with any valid readable TMC4361A registers");
        return E_NOT_FOUND;
    }

    uint32_t reg_result[SM_NUM_DRIVES];


    tmc4361A_read_uint32_all((uint32_t)register_to_read, reg_result, SM_NUM_DRIVES, &hspi1);


    for (int i = 0; i < SM_NUM_DRIVES; i++) {
        print_register_32(&reg_result[i]);
    }
    return E_SUCCESS;


}

