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
            next_state = PROGRAM_STATE_GUESS_ASK;
            break;

        case USER_OPTION_NO:
            *current_node = (size_t) negative_answer_node;
            next_state = PROGRAM_STATE_GUESS_ASK;
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
AskIfGuessed(visualisation_context* screen,
             akinator_t             akinator,
             size_t                 previous_node)
{
    ASSERT(screen != NULL);
    ASSERT_AKINATOR(akinator);

    ImageWindowInit(screen);
    QuestionWindowInit(screen);

    ShowAkinator(screen);

    const char* suggestion_template = "%.*s is your object.";
    const size_t comment_buffer_size = 300;
    char add_object_suggestion[comment_buffer_size] = {};

    string_s previous_object = akinator->object_tree->nodes_array[previous_node].node_value;

    snprintf(add_object_suggestion, comment_buffer_size, suggestion_template, 
             previous_object.string_size, previous_object.string_source);

    ShowMessage(screen, add_object_suggestion, strlen(add_object_suggestion));

    program_state_e next_state = PROGRAM_STATE_ERROR;
    user_option_e user_option = GiveChoice();

    switch(user_option)
    {
        case USER_OPTION_YES:
            next_state = PROGRAM_STATE_QUIT;
            break;
        
        case USER_OPTION_NO:
            next_state = PROGRAM_STATE_ADD;
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

// ============================ PROGRAM_STATE_ADD =============================


static akinator_return_e
AddObjectInGame(akinator_t       akinator,
                size_t           current_node,
                const string_s*  question_string,
                const string_s*  object_string);

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
        akinator->akinator_error = AKINATOR_RETURN_ALLOCATION_ERROR;

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
    DestroyWindow(&screen->scan_window);

// =========================== Ask new question ===============================

    const char* question_comment_template = "you could name the difference between %s and %.*s."
                                            "(Example: It (new_object) studies in MIPT)"; 

    const size_t comment_buffer_size = 300;
    char question_comment[comment_buffer_size] = {};

    string_s previous_object = akinator->object_tree->nodes_array[previous_node].node_value;

    snprintf(question_comment, comment_buffer_size, question_comment_template, 
             object, previous_object.string_size, previous_object.string_source);

    ShowMessage(screen, question_comment, strlen(question_comment));

    ScanWindowInit(screen);
    ScanUserInput(screen, question, max_buffer_count);
    DestroyWindow(&screen->scan_window);

    string_s question_string = {.string_source = question, 
                                .string_size = strlen(question)};

// ============================== Adding nodes ================================
    
    if (AddObjectInGame(akinator, previous_node, 
                        &question_string, &object_string) != 0)
    {
        akinator->akinator_error = AKINATOR_RETURN_ADD_OBJECT_ERROR;

        return PROGRAM_STATE_ERROR;
    } 

    return PROGRAM_STATE_QUIT;
}

static akinator_return_e
AddObjectInGame(akinator_t       akinator,
                size_t           previous_node,
                const string_s*  question_string,
                const string_s*  object_string)
{
    ASSERT_AKINATOR(akinator);
    ASSERT(question_string != NULL);
    ASSERT(object_string != NULL);


    node_s question_node = {.parent_index = akinator->object_tree
                                ->nodes_array[previous_node].parent_index,
                            .parent_connection = akinator->object_tree
                                ->nodes_array[previous_node].parent_connection,
                            .right_index = NO_LINK,
                            .left_index = (ssize_t) previous_node,
                            .node_value = *question_string};

    if (TreeAddNode(akinator->object_tree, &question_node) != 0)
    {
        return AKINATOR_RETURN_ADD_OBJECT_ERROR;
    }

    node_s object_node = {.parent_index = (ssize_t) question_node.index_in_tree,
                          .parent_connection = EDGE_DIR_RIGHT,
                          .right_index = NO_LINK,
                          .left_index = NO_LINK,
                          .node_value = *object_string};
                    
    if (TreeAddNode(akinator->object_tree, &object_node) != 0)
    {
        return AKINATOR_RETURN_ADD_OBJECT_ERROR;
    }

    return AKINATOR_RETURN_SUCCESS;
}

// ============================= PROGRAM_STATE_MENU ===========================

program_state_e 
AskUserMode()
{
    mode_menu_e user_option = ShowMenu();
    program_state_e next_state = PROGRAM_STATE_ERROR; 

    switch(user_option)
    {
        case MODE_MENU_GUESS:
            next_state = PROGRAM_STATE_GUESS_ASK;
            break;
    
        case MODE_MENU_DEFINITION:
            next_state = PROGRAM_STATE_DEFINITION;
            break;

        case MODE_MENU_COMPARE:
            next_state = PROGRAM_STATE_COMPARE;
            break;

        case MODE_MENU_QUIT:
            next_state = PROGRAM_STATE_QUIT;
            break;

        default: break;
    }

    return next_state;
}
         
// ========================== PROGRAM_STATE_DEFINITION ========================

static akinator_return_e
ShowObjectDefinition(akinator_t             akinator, 
                     visualisation_context* screen,
                     swag_t                 path_stack);

program_state_e
GiveObjectDefinition(akinator_t             akinator,
                     visualisation_context* screen)
{
    ASSERT_AKINATOR(akinator)

    ScanWindowInit(screen);

    const size_t buffer_capacity = 100;
    char object_name[buffer_capacity] = {};

    ScanUserInput(screen, object_name, buffer_capacity);

    DestroyWindow(&screen->scan_window);

    ssize_t defined_object_index = SearchObject(akinator, object_name);

    if (defined_object_index == NO_LINK)
    {
        akinator->akinator_error = AKINATOR_RETURN_UNDEFINED_OBJECT;

        return PROGRAM_STATE_ERROR;
    }

    swag_t path_stack = NULL;
    const size_t start_stack_size = 10;

    if (StackInit(&path_stack, start_stack_size, "Path stack") != 0)
    {
        akinator->akinator_error = AKINATOR_RETURN_STACK_ERROR;

        return PROGRAM_STATE_ERROR;
    }

    if (PutPathIntoStack(akinator, defined_object_index,
                         path_stack) != 0)
    {
        StackDestroy(path_stack);
        akinator->akinator_error = AKINATOR_RETURN_PATH_ERROR;

        return PROGRAM_STATE_ERROR;
    }

    akinator_return_e return_value = AKINATOR_RETURN_SUCCESS;

    if ((return_value = ShowObjectDefinition(akinator, screen, path_stack)) != 0)
    {
        StackDestroy(path_stack);
        akinator->akinator_error = return_value;

        return PROGRAM_STATE_ERROR; 
    }

    StackDestroy(path_stack);

    return PROGRAM_STATE_MENU;
}

static akinator_return_e
ShowObjectDefinition(akinator_t             akinator,
                     visualisation_context* screen,
                     swag_t                 path_stack)
{
    ASSERT_AKINATOR(akinator);
    ASSERT(path_stack != NULL);
    ASSERT(screen != NULL);

    SubWindow1Init(screen);
    SubWindow2Init(screen);
    SubWindow3Init(screen);

    size_t previous_node = 0;
    size_t current_node = 0;

    node_s* nodes_array = akinator->object_tree->nodes_array;

    if (StackPop(path_stack, &current_node) != 0)
    {
        return AKINATOR_RETURN_STACK_ERROR;
    }

    do 
    {
        previous_node = current_node;
        
        if (StackPop(path_stack, &current_node) != 0)
        {
            return AKINATOR_RETURN_STACK_ERROR;
        }

        if (nodes_array[previous_node].right_index == (ssize_t) current_node)
        {
            AddTextToSubwindow(&screen->subwindow_2, 
                               nodes_array[previous_node].node_value.string_source,
                               nodes_array[previous_node].node_value.string_size);
        }
        else if (nodes_array[previous_node].left_index == (ssize_t) current_node)
        {
            AddTextToSubwindow(&screen->subwindow_3, 
                               nodes_array[previous_node].node_value.string_source,
                               nodes_array[previous_node].node_value.string_size);

        }
    } while (GetStackSize(path_stack) != 0);

    AddTextToSubwindow(&screen->subwindow_1, 
                       nodes_array[current_node].node_value.string_source,
                       nodes_array[current_node].node_value.string_size);

    while (getch() != 'q');

    DestroySubwindow(&screen->subwindow_1);
    DestroySubwindow(&screen->subwindow_2);
    DestroySubwindow(&screen->subwindow_3);

    return AKINATOR_RETURN_SUCCESS;
}

// ========================== PROGRAM_STATE_COMPARE ===========================


static akinator_return_e 
ShowComparison(akinator_t             akinator,
               visualisation_context* screen,
               swag_t                 path_stack_1,
               swag_t                 path_stack_2);

program_state_e
CompareTwoObjects(akinator_t             akinator,
                  visualisation_context* screen)
{
    ASSERT_AKINATOR(akinator);
    ASSERT(screen != NULL);

    const size_t start_stack_size = 10;
    
// ======================= FIRST_OBJECT_DEFINITION ============================

    ScanWindowInit(screen);
    
    const size_t buffer_capacity = 100;
    char object_name_1[buffer_capacity] = {};
    ScanUserInput(screen, object_name_1, buffer_capacity);

    DestroyWindow(&screen->scan_window);

    ssize_t defined_object_index_1 = SearchObject(akinator, object_name_1);

    if (defined_object_index_1 == NO_LINK)
    {
        akinator->akinator_error = AKINATOR_RETURN_UNDEFINED_OBJECT;

        return PROGRAM_STATE_ERROR;
    }

    swag_t path_stack_1 = NULL;

    if (StackInit(&path_stack_1, start_stack_size, "Path stack") != 0)
    {
        akinator->akinator_error = AKINATOR_RETURN_STACK_ERROR;

        return PROGRAM_STATE_ERROR;
    }

    if (PutPathIntoStack(akinator, defined_object_index_1,
                         path_stack_1) != 0)
    {
        StackDestroy(path_stack_1);
        akinator->akinator_error = AKINATOR_RETURN_PATH_ERROR;

        return PROGRAM_STATE_ERROR;
    }

// ====================== SECOND_OBJECT_DEFINITION ============================

    ScanWindowInit(screen);

    char object_name_2[buffer_capacity] = {};
    ScanUserInput(screen, object_name_2, buffer_capacity);

    DestroyWindow(&screen->scan_window);

    ssize_t defined_object_index_2 = SearchObject(akinator, object_name_2);

    if (defined_object_index_2 == NO_LINK)
    {
        StackDestroy(path_stack_1);

        akinator->akinator_error = AKINATOR_RETURN_UNDEFINED_OBJECT;

        return PROGRAM_STATE_ERROR;
    }

    swag_t path_stack_2 = NULL;

    if (StackInit(&path_stack_2, start_stack_size, "Path stack") != 0)
    {
        StackDestroy(path_stack_1);

        akinator->akinator_error = AKINATOR_RETURN_STACK_ERROR;

        return PROGRAM_STATE_ERROR;
    }

    if (PutPathIntoStack(akinator, defined_object_index_2,
                         path_stack_2) != 0)
    {
        StackDestroy(path_stack_1);
        StackDestroy(path_stack_2);
        akinator->akinator_error = AKINATOR_RETURN_PATH_ERROR;

        return PROGRAM_STATE_ERROR;
    }

// ========================== COMPARATOR ======================================
    
    SubWindow2Init(screen);    

    akinator_return_e return_value = AKINATOR_RETURN_SUCCESS;
    if (ShowComparison(akinator, screen, path_stack_1, path_stack_2) != 0)
    {
        StackDestroy(path_stack_1);
        StackDestroy(path_stack_2);

        akinator->akinator_error = return_value;

        return PROGRAM_STATE_ERROR;
    }

    while (getch() != 'q');

    DestroySubwindow(&screen->subwindow_2);
    StackDestroy(path_stack_1);
    StackDestroy(path_stack_2);

    return PROGRAM_STATE_MENU;
}

static akinator_return_e 
ShowComparison(akinator_t             akinator,
               visualisation_context* screen,
               swag_t                 path_stack_1,
               swag_t                 path_stack_2)
{
    ASSERT_AKINATOR(akinator);
    ASSERT(screen != NULL);
    ASSERT(screen->subwindow_2.window != NULL);
    ASSERT(path_stack_1 != NULL);
    ASSERT(path_stack_2 != NULL);
    
    size_t previous_path_node = 0;

    size_t path_node_1 = 0;
    size_t path_node_2 = 0;

    if ((StackPop(path_stack_1, &path_node_1) != 0) 
        || (StackPop(path_stack_2, &path_node_2) != 0))
    {
        return AKINATOR_RETURN_STACK_ERROR;
    }

    previous_path_node = path_node_1;

    if ((StackPop(path_stack_1, &path_node_1) != 0) 
        || (StackPop(path_stack_2, &path_node_2) != 0))
    {
        return AKINATOR_RETURN_STACK_ERROR;
    }

    node_s* nodes_array = akinator->object_tree->nodes_array;

    while (path_node_1 == path_node_2)
    {
        AddTextToSubwindow(&screen->subwindow_2, 
                           nodes_array[previous_path_node].node_value.string_source,
                           nodes_array[previous_path_node].node_value.string_size);
            
        previous_path_node = path_node_1;

        if ((StackPop(path_stack_1, &path_node_1) != 0) 
            || (StackPop(path_stack_2, &path_node_2) != 0))
        {
            return AKINATOR_RETURN_STACK_ERROR;
        }
    }

    return AKINATOR_RETURN_SUCCESS;
}