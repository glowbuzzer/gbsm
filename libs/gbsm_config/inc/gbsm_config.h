/**
 ******************************************************************************
 * @file           :  gbsm_config.h
 * @brief          :  source for GBSM config hash defines
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#ifndef GB_MONOREPO_GBSM_CONFIG_H
#define GB_MONOREPO_GBSM_CONFIG_H

#include "gbsm_config_autogen.h"
#include "stdbool.h"

#define SM_NUM_DRIVES                                   1
#define USE_ESTOP_RESET                                 0
#define DISABLE_ESTOP_CHECKING                          1
#define BUS_CYCLE_TIME                                  4

#define GBSM_REMINDER_MESSAGE_INTERVAL_MS               5000
#define GBSM_GBC_CONNECTION_CHECK_INTERVAL_MS           50000
#define GBSM_I_AM_ALIVE_MESSAGE_INTERVAL_MS             10000
#define GBSM_DELAY_TO_START_MESSAGES_SEC                10


/*** GBC CONFIGURATION ***/

/** defines the size of the PDO buffers IN/OUT (overlaid with the dpm structs) */
#define SIZE_OF_GBC_PDO                                 200


/*** *** CTRL CONFIGURATION *** ***/

/** Number of cycles in which the heartbeat received from GBC must be within the one generated on GBSM */
#define CTRL_HEARTBEAT_TOLERANCE                        50

/**Bit number in machine controlword that triggers a reset of drives and statemachine */
#define CTRL_MACHINE_CTRL_WRD_REQUEST_RESET_BIT_NUM     ???

/** Threshold of cycles after requesting drives change state that the transition must be made */
//this has to be quite large as it needs to be > maximum time for the drives to stop
#define CTRL_DRIVE_CHANGE_STATE_TIMEOUT                 100

/**CRITICAL! if this is defined then hardware estop is disabled! */
#define DISABLE_ESTOP_CHECKING                          1
/**CRITICAL! if this is defined then heartbeat is disabled! */
#define DISABLE_HEARTBEAT_CHECKING                      0

/** This defines the size of the stack in bytes allocated to the main ecrxtx thread */
#define STACK64K                                        (64 * 1024)

#define GBSM_MICROSTEPS                                 256
#define GBSM_FULL_STEP_PER_REV                          200


/*** *** SIZES & LENGTHS CONFIGURATION *** ***/

/* Defines for length of strings, buffers etc. */

/** Max length of string for gbc process name */
#define GBC_PROCESS_NAME_MAX_LENGTH                     100

#define GBSM_ENABLE_VSTOPS                              0

#define V_STOP_L_1                                      (-99)
#define V_STOP_R_1                                      (99)
#define V_STOP_L_2                                      (-99)
#define V_STOP_R_2                                      (99)
#define V_STOP_L_3                                      (-99)
#define V_STOP_R_3                                      (99)


#define GBSM_ENABLE_PHYSTOPS                            0


#define CYCLIC_EXEC_TIME_ERROR_PERCENTAGE               80
#define CYCLIC_EXEC_TIME_WARNING_PERCENTAGE             50

typedef struct {
    bool enable_l;
    bool enable_r;
    int pos_l;
    int pos_r;
}gbsm_vstop_config_t;


typedef enum {GBSM_ENCODER_NONE, GBSM_ENCODER_ABN, GBSM_ENCODER_ABS_SSI, GBSM_ENCODER_ABS_SPI} gbsm_encoder_type_t;


typedef struct{
    bool enable;
gbsm_encoder_type_t encoder_type;
}gbsm_encoders_config_t;

bool gbsm_enable_phystop_l[SM_NUM_DRIVES];
bool gbsm_enable_phystop_r[SM_NUM_DRIVES];
bool gbsm_motor_directions[SM_NUM_DRIVES];
gbsm_encoders_config_t gbsm_encoder_config[SM_NUM_DRIVES];
gbsm_vstop_config_t gbsm_vstop_config[SM_NUM_DRIVES];

#endif //GB_MONOREPO_GBSM_CONFIG_H
