#include "state_machine_functions.h"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
   
#include "Assert.h"
#include "akinator.h"
#include "visuals.h"
#include "my_string.h"

// =========================== PROGRAM_STATE_ASK ==============================

program_state_e
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
   
    program_state_e next_state = PROGRAM_STATE_ERROR;

    switch (user_option)
    {
        case USER_OPTION_YES:
            *current_node = (size_t) positive_answer_node; 
            next_state = PROGRAM_STATE_ASK;
            break;

        case USER_OPTION_NO:
            *current_node = (size_t) negative_answer_node;
            next_state = PROGRAM_STATE_ASK;
            break;

        case USER_OPTION_EXIT:
            next_state = PROGRAM_STATE_QUIT;
            break;

        default: break;
    }

    DestroyWindow(&screen->question_window);
    DestroyWindow(&screen->img_window);
    
    return next_state;
} 

// ============================ PROGRAM_STATE_GUESSED =========================

program_state_e 
AddNewElement(visualisation_context* screen,
              akinator_t             akinator,
              size_t                 previous_node)
{
    ASSERT_AKINATOR(akinator);
    ASSERT(screen != NULL);

    const size_t max_buffer_count = 100;

    char* object = (char*) falloc(max_buffer_count, sizeof(char),
                                  akinator->memory);

    char* question = (char*) falloc(max_buffer_count, sizeof(char),
                                    akinator->memory);
    
    if ((object == NULL) || (question == NULL))
    {
        return PROGRAM_STATE_ERROR;
    }
   
    ImageWindowInit(screen);
    QuestionWindowInit(screen);

    ShowAkinator(screen);

// ==================== Ask if user want to add his element ===================

    const char* new_object_question = "you would like to add new object." ;
    ShowMessage(screen, new_object_question, strlen(new_object_question));

    user_option_e user_option = GiveChoice();

    if (user_option != USER_OPTION_YES)
    {
        DestroyWindow(&screen->question_window);
        DestroyWindow(&screen->img_window);

        return PROGRAM_STATE_QUIT;
    }

// ========================= Ask new element name =============================

    const char* new_object_comment = "you could enter new name.";
    ShowMessage(screen, new_object_comment, strlen(new_object_comment));

    ScanWindowInit(screen);
    ScanUserInput(screen, object, max_buffer_count);
    string_s object_string = {.string_source = object, .string_size = strlen(object)};

// =========================== Ask new question ===============================

    const char* question_comment_template = "you could name the difference between %s and %.*s.\n"
                                            "(Example: It (new_object) studies in MIPT"; 

    const size_t commment_buffer_size = 300;
    char question_comment[commment_buffer_size] = {};

    string_s previous_object = akinator->object_tree->nodes_array[previous_node].node_value;

    snprintf(question_comment, commment_buffer_size, question_comment_template, 
             object, previous_object.string_size, previous_object.string_source);

    ShowMessage(screen, question_comment, strlen(question_comment));

    ScanUserInput(screen, question, max_buffer_count);

    string_s question_string = {.string_source = question, .string_size = strlen(question)};

// ============================== Adding nodes ================================

}
