/**
 ******************************************************************************
 * @file           :  read_register.h
 * @brief          :  register print functions for TMC5160 or TMC4361A
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_READ_REGISTER_H
#define GB_MONOREPO_READ_REGISTER_H

void interactive_reg_print(void);
gberror_t print_tmc4361a_reg(int register_to_read);
gberror_t print_tmc5160_reg(int register_to_read);


#endif //GB_MONOREPO_READ_REGISTER_H
