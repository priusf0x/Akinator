#include "state_machine.h"

#include <ncurses.h>
   
#include "Assert.h"
#include "akinator.h"
#include "visuals.h"

program_state_e
StartStateMachine(akinator_t akinator)
{
    ASSERT(akinator != NULL);

    visualisation_context screen = {};
    ScreenContextInit(&screen);

    program_state_e current_state = PROGRAM_STATE_MENU;
    size_t current_node = akinator->object_tree->nodes_array[0].left_index;

    switch(current_state)
    {
        case PROGRAM_STATE_MENU:
            current_state = PROGRAM_STATE_ASK; // time 
            break;     

        case PROGRAM_STATE_ASK:
            // printf("%d", current_node);
            break;   

        case PROGRAM_STATE_QUIT:
            break;

        default: break;
    }

    ScreenContextDestroy(&screen);

    return PROGRAM_STATE_QUIT;
}

static program_state_e
AskUser(visualisation_context* screen,
        akinator_t*            akinator,
        size_t*                current_node)
{
    ASSERT(screen != NULL);
    ASSERT(akinator != NULL);
    ASSERT(current_node != NULL)

    return PROGRAM_STATE_ERROR;
} 

