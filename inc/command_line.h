/**
 ******************************************************************************
 * @file           :  command_line.h
 * @brief          :  command line arguments processing
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_COMMAND_LINE_H
#define GB_MONOREPO_COMMAND_LINE_H


typedef enum{GBSM_PROG_NONE, GBSM_PROG_CYCLIC, GBSM_PROG_TEST_MOVE, GBSM_PROG_INTERACTIVE_REG_READ, GBSM_PROG_HYST_TUNE} gbsm_prog_t;

typedef struct{
    gbsm_prog_t program;
} command_line_vars_t;


void process_command_line (int argc, char *argv[], command_line_vars_t *var);


extern command_line_vars_t command_line_vars;


#endif //GB_MONOREPO_COMMAND_LINE_H
