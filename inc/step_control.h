/**
 ******************************************************************************
 * @file           :  step_control.h
 * @brief          :  main control functions - runs drive state machine
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include "stateMachine.h"

#include "cia402.h"
#include "gberror.h"

/** sm event data (passed in with each call to execute the state machine ) */
typedef struct {
    bool follow_error;
    bool machine_request_error; //bit 16 of machine controlword
    bool machine_move_not_op_enabled_fault_req; //bit 17 of machine controlword
    bool gbc_internal_fault;
    bool estop;
    bool heartbeat_lost;
    bool any_drive_has_alarm;
    bool internal_limit;
    bool remote_ok;
    bool slave_reported_error;
    bool plc_signalled_error;
    bool homing_failed;
    uint32_t fault_cause;
} step_event_data_t;



struct stateMachine* step_ctrl_statemachine_alloc(void);
void step_ctrl_statemachinefree (struct stateMachine *smachine);
void step_ctrl_init_statemachine(struct stateMachine *smachine);
void step_ctrl_main(struct stateMachine *m, bool first_run);


/* sm helper functions */
void step_ctrl_change_all_drives_states(uint16_t controlword);
bool cia_is_fault_condition(struct event *event);
bool step_ctrl_check_all_drives_state(cia_state_t state);
bool step_ctrl_check_any_drives_state(cia_state_t state);
bool step_ctrl_check_all_drives_commands_sim(cia_commands_t command);

extern step_event_data_t event_data;
extern uint32_t gbem_heartbeat ;
extern bool estop;
extern cia_state_t current_state;
extern uint32_t ctrl_state_change_cycle_count;
extern uint32_t ctrl_state_change_timeout;


void step_ctrl_process_iomap_in (void);

void step_ctrl_process_iomap_out (const bool zero);

bool step_ctrl_check_for_internal_limit(gberror_t *grc);
bool step_ctrl_check_for_follow_error(gberror_t *grc);
bool step_ctrl_check_remote(void);
#endif /* INC_CONTROL_H_ */
