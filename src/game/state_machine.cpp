#include "state_machine.h"

#include <ncurses.h>
   
#include "Assert.h"
#include "akinator.h"
#include "visuals.h"

static program_state_e
AskUser(visualisation_context* screen,
        akinator_t             akinator,
        size_t*                current_node);

program_state_e
StartStateMachine(akinator_t akinator)
{
    ASSERT(akinator != NULL);

    visualisation_context screen = {};
    ScreenContextInit(&screen);

    program_state_e current_state = PROGRAM_STATE_MENU;

    size_t current_node = (size_t) akinator->object_tree->nodes_array[0].left_index;

    while (current_state != PROGRAM_STATE_QUIT)
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
                ScreenContextDestroy(&screen);
                break;

            case PROGRAM_STATE_GUESSED:
                return PROGRAM_STATE_QUIT;
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
    }   

    ScreenContextDestroy(&screen);

    return PROGRAM_STATE_QUIT;
}

// ============================== STATE_MACHINE_CASES =========================

static program_state_e
AskUser(visualisation_context* screen,
        akinator_t             akinator,
        size_t*                current_node)
{
    ASSERT(screen != NULL);
    ASSERT_AKINATOR(akinator);
    ASSERT(current_node != NULL);

    string_s current_node_value = akinator->object_tree
                                    ->nodes_array[*current_node].node_value;

    ssize_t positive_answer_node = akinator->object_tree
                                        ->nodes_array[*current_node].right_index;
    ssize_t negative_answer_node = akinator->object_tree
                                        ->nodes_array[*current_node].left_index;

    if ((positive_answer_node == NO_LINK) || (negative_answer_node == NO_LINK))
    {
        return PROGRAM_STATE_GUESSED;
    }

    ImageWindowInit(screen);
    QuestionWindowInit(screen);

    ShowAkinator(screen);
    ShowMessage(screen, current_node_value.string_source,
                current_node_value.string_size);

    user_option_e user_option = GiveChoice();
    
    switch (user_option)
    {
        case USER_OPTION_YES:
            *current_node = (size_t) positive_answer_node;
            return PROGRAM_STATE_ASK;

        case USER_OPTION_NO:
            *current_node = (size_t) negative_answer_node;
            return PROGRAM_STATE_ASK;

        case USER_OPTION_EXIT:
            return PROGRAM_STATE_QUIT;

        default: break;
    }

    DestroyWindow(screen->question_window);
    DestroyWindow(screen->img_window);
    
    return PROGRAM_STATE_ERROR;
} 

static void 
AddNewElement(akinator_t akinator)
{
    ASSERT_AKINATOR(akinator);


    // read new element 

    // read the difference between the elements
    
    // add the difference 

    // add new element
}