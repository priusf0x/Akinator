#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <ncurses.h>
#include "akinator.h"

enum program_state_e
{
    PROGRAM_STATE_MENU,
    PROGRAM_STATE_GUESS_ASK,
    PROGRAM_STATE_DEFINITION,
    PROGRAM_STATE_COMPARE,
    PROGRAM_STATE_ADD,
    PROGRAM_STATE_GUESSED,
    PROGRAM_STATE_QUIT,
    PROGRAM_STATE_ERROR
};

akinator_return_e StartStateMachine(akinator_t akinator);

#endif // STATE_MACHINE_H