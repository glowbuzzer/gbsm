
/**
 ******************************************************************************
 * @file           :  command_line.c
 * @brief          :  command line arguments processing
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#include "command_line.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "user_message.h"

#include <string.h>
#include <signal.h>

command_line_vars_t command_line_vars = {0};

#define NUM_GBSM_PROG 5

char gbsm_prog_names[NUM_GBSM_PROG][200] = {{"GBSM program None"}, {"GBSM cyclic"},{"GBSM program test move"}, {"GBSM interactive register read"}, {"GBSM hysteresis tuning"}};

/**
 * @brief prints usage information for gbem command line
 */
static void getopt_usage(void) {
    printf("Usage of GBSM:\n");

    printf("\t-h | -pPROGRAM_NAME  \n\n");
    printf("\t-h | --help: GBSM usage information\n");
    printf("\nExample #1: GBSM -pREG_READ = run GBSM with program REG_READ\n");
    printf("\nAvailable programs:\n");
    printf("\tCYCLIC - normal cyclic execution\n");
    printf("\tREG_READ - read a register on the TMC5160 or TMC4361A to check configuration\n");
    printf("\tTEST_MOVE - read a register on the TMC5160 or TMC4361A to check configuration\n");
    printf("\tHYST_TUNE - tune hysteresis settings\n");
}


void process_command_line(int argc, char *argv[], command_line_vars_t *vars) {
    char ch;
    int index = 0;
    bool program_specified = false;
    int help = 0;
    struct option options[] = {
            {"help",            no_argument,       NULL, 'h'},
            {"program",         required_argument, NULL, 'p'},
             {0, 0, 0,                                    0}
    };

    while (((ch = getopt_long(argc, argv, "hp:", options, &index)) != -1) && (ch != 255)) {
        switch (ch) {
            case 'h':
                getopt_usage();
                help = 1;
                break;
            case 'p':
                if (optarg != NULL) {
                    if(strcmp(optarg,"CYCLIC")==0)
                    {
                        vars->program = GBSM_PROG_CYCLIC;
                        program_specified = true;
                    }
                    else if(strcmp(optarg,"REG_READ")==0)//compare both the strings
                    {
                        vars->program = GBSM_PROG_INTERACTIVE_REG_READ;
                        program_specified = true;
                    }
                    else if (strcmp(optarg,"TEST_MOVE")==0)
                    {
                        vars->program = GBSM_PROG_TEST_MOVE;
                        program_specified = true;

                    }
                    else if (strcmp(optarg,"HYST_TUNE")==0)
                    {
                        vars->program = GBSM_PROG_HYST_TUNE;
                        program_specified = true;

                    }
                    else{
                        UM_INFO(GBSM_UM_EN, "GBSM Specified program name did not match those available");
                    }

                }
                break;
            case '?':
                getopt_usage();
                exit(EXIT_SUCCESS);
            default:
                getopt_usage();
                exit(EXIT_SUCCESS);
        }
    }



    if (help) {
        exit(EXIT_SUCCESS);
    }

    if (!program_specified){
        vars->program = GBSM_PROG_CYCLIC;
        UM_WARN(GBSM_UM_EN, "GBSM: No valid program was specified on the command line. Will use the GBSM_PROG_CYLIC");
    }

    UM_INFO(GBSM_UM_EN,"GBSM: Running with program [%s]", gbsm_prog_names[vars->program]);

}



