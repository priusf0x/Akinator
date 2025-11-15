#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <ncurses.h>
#include "akinator.h"

enum program_state_e
{
    PROGRAM_STATE_MENU,
    PROGRAM_STATE_ASK,
    PROGRAM_STATE_END_SCREEN,
    PROGRAM_STATE_QUIT,
    PROGRAM_STATE_ERROR
};

program_state_e StartStateMachine(akinator_t akinator);

#endif // STATE_MACHINE_H