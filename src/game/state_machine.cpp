#include "state_machine.h"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
   
#include "Assert.h"
#include "akinator.h"
#include "visuals.h"
#include "my_string.h"
 
program_state_e
StartStateMachine(akinator_t akinator)
{
    ASSERT(akinator != NULL);

    visualisation_context screen = {};
    ScreenContextInit(&screen);

    program_state_e current_state = PROGRAM_STATE_MENU;

    size_t current_node = (size_t) akinator->object_tree->nodes_array[0].left_index;

    do
    {
        switch(current_state)
        {
            case PROGRAM_STATE_MENU:
                current_state = PROGRAM_STATE_ASK; 
                // test_mode test_mode test_mode 
                break;     

            case PROGRAM_STATE_ASK:
                current_state = AskUser(&screen, akinator, &current_node);
                break;   

            case PROGRAM_STATE_QUIT:
                TreeBaseDump(akinator->object_tree, stdout);
                break;

            case PROGRAM_STATE_GUESSED:
                current_state = PROGRAM_STATE_QUIT;
                AddNewElement(&screen, akinator, current_node);
                break;

            case PROGRAM_STATE_ADD:
                // test_mode test_mode test_mode
                printf("PROGRAM STATE ADD");
                current_state = PROGRAM_STATE_QUIT;
                break;

            case PROGRAM_STATE_ERROR:
                // test_mode test_mode test_mode 
                break;

            default: 
                ScreenContextDestroy(&screen);
                break;
        }
    } while (current_state != PROGRAM_STATE_QUIT);

    ScreenContextDestroy(&screen);

    return PROGRAM_STATE_QUIT;
}














