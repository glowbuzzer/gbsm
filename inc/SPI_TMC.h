/**
 ******************************************************************************
 * @file           :  SPI_TMC.h
 * @brief          :  functions to SPI read/write to the tmc4361a
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GBSTEP_TMC5160_SPI_TMC_H
#define GBSTEP_TMC5160_SPI_TMC_H


//#include "API_Header.h"

#include <stdint.h>

extern unsigned int hspi1;





void initSPI(void);


void tmc4361A_write_int32_to_all_same_val(uint8_t address, int32_t value, uint8_t total_num_in_chain, const unsigned int *hspi, uint8_t *spi_status);

void tmc4361A_write_int32_to_all_different_vals(uint8_t address, const int32_t *values, uint8_t total_num_in_chain, const unsigned int *hspi, uint8_t *spi_status);

void tmc4361A_write_uint32_to_all_same_val(uint8_t address, uint32_t value, uint8_t total_num_in_chain, const unsigned int *hspi, uint8_t *spi_status);

void tmc4361A_write_uint32_to_all_different_vals(uint8_t address, const uint32_t *values, uint8_t total_num_in_chain, const unsigned int *hspi, uint8_t *spi_status);

void tmc4361A_write_int32_to_individual(uint8_t address, int32_t value, uint8_t pos_in_chain, const unsigned int *hspi, uint8_t *spi_status);

void tmc4361A_write_uint32_to_individual(uint8_t address, uint32_t value, uint8_t pos_in_chain, const unsigned int *hspi, uint8_t *spi_status);

void tmc4361A_read_int32_all(uint8_t address, int32_t *values, uint8_t total_num_in_chain, const unsigned int *hspi);

void tmc4361A_read_uint32_all(uint8_t address, uint32_t *values, uint8_t total_num_in_chain, const unsigned int *hspi);

int32_t tmc4361A_read_int32_individual(uint8_t address, uint8_t pos_in_chain, uint8_t total_num_in_chain, const unsigned int *hspi);

uint32_t tmc4361A_read_uint32_individual(uint8_t address, uint8_t pos_in_chain, uint8_t total_num_in_chain, const unsigned int *hspi);



#endif //GBSTEP_TMC5160_SPI_TMC_H
