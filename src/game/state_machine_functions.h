#ifndef STATE_MACHINE_FUNCTIONS_H
#define STATE_MACHINE_FUNCTIONS_H

#include <stdlib.h>

#include "visuals.h"
#include "akinator.h"
#include "state_machine.h"

program_state_e AskUserMode();
program_state_e AskUser(visualisation_context* screen, akinator_t akinator, size_t* current_node);
program_state_e AddNewElement(visualisation_context* screen, akinator_t akinator, size_t previous_node);
program_state_e AskIfGuessed(visualisation_context* screen, akinator_t akinator, size_t previous_node);

#endif // STATE_MACHINE_FUNCTIONS_H