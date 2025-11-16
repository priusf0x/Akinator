#include "akinator.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "Assert.h"
#include "tree.h"
#include "my_allocator.h"

static akinator_return_e ReadFileData(akinator_t akinator, const char* file_name);


static recursion_return_e RecursiveParser(akinator_t akinator, 
                                         size_t*    current_position, 
                                         size_t     root_position);

akinator_return_e
AkinatorInit(akinator_t*  akinator,
             const char*  file_name)
{   
    ASSERT(akinator != NULL);
    ASSERT(file_name != NULL);

    (*akinator) = (akinator_t) calloc(1, sizeof(akinator_s));
    if (akinator == NULL)
    {
        return AKINATOR_RETURN_ALLOCATION_ERROR;
    }

    const size_t start_tree_size = 10;
    if (TreeInit(&((*akinator)->object_tree), start_tree_size) != 0)
    {
        free(*akinator);
        return AKINATOR_RETURN_TREE_INIT_ERROR;
    }

    akinator_return_e output = ReadFileData((*akinator), file_name);
    if (output != AKINATOR_RETURN_SUCCESS)
    {
        TreeDestroy((*akinator)->object_tree);
        free(*akinator);

        return output;
    }

// test_version_will_be_deleted test_version_will_be_deleted test_version_will_be_deleted 

    const size_t max_buffer_count = 1024;
    if (FallocateInit(&(*akinator)->memory, max_buffer_count) != 0)
    {
        return AKINATOR_RETURN_ALLOCATION_ERROR;
    }

// test_version_will_be_deleted test_version_will_be_deleted test_version_will_be_deleted

    size_t current_position = 0;
    if (RecursiveParser(*akinator, &current_position, 0) != 0)
    {
        return AKINATOR_RETURN_READ_ERROR;
    }

    return AKINATOR_RETURN_SUCCESS;
}

akinator_return_e 
AkinatorDestroy(akinator_t* akinator)
{
    TreeDestroy((*akinator)->object_tree);
    free((*akinator)->input_buffer);
    FallocateDestroy(&(*akinator)->memory);
    free((*akinator));
    akinator = NULL;

    return AKINATOR_RETURN_SUCCESS;
}

// ========================= INIT_HELP_FUNCTION ===============================

static akinator_return_e 
ReadFileData(akinator_t  akinator,
             const char* file_name)
{
    ASSERT(akinator != NULL);
    ASSERT(file_name != NULL);

    struct stat file_stat = {};

    if (stat(file_name, &file_stat) != 0)
    {
        return AKINATOR_RETURN_SYSTEM_CALL_ERROR;
    }

    size_t char_number = (size_t) (file_stat.st_size);
    
    FILE* file_input = fopen(file_name , "r");
    if (file_input == NULL)
    {
        return AKINATOR_RETURN_FILE_OPEN_ERROR;
    }

    akinator->input_buffer = (char*) calloc(char_number + 1, sizeof(char));
    if (akinator->input_buffer == NULL)
    {
        fclose(file_input);
        return AKINATOR_RETURN_ALLOCATION_ERROR;
    }

    akinator->input_buffer[char_number] = '\0';
    
    size_t read_count = fread(akinator->input_buffer , sizeof(char), char_number, file_input);
    
    if (fclose(file_input) != 0)
    {
        free(akinator->input_buffer);
        return AKINATOR_RETURN_FILE_CLOSE_ERROR;
    }

    if (read_count == 0)
    {
        free(akinator->input_buffer);
        return AKINATOR_RETURN_EMPTY_FILE;
    }    

    return AKINATOR_RETURN_SUCCESS;
}

// =========================== RECURSION_ALGORITHM ============================


static recursion_return_e 
ReadNode(akinator_t akinator,
         size_t*    curren_position,
         size_t     root_position,
         edge_dir_e node_position);

static recursion_return_e
RecursiveParser(akinator_t akinator,
                size_t*    current_position,
                size_t     root_position)
{
    ASSERT(akinator != NULL);
    ASSERT(current_position);

    if (!CheckIfSymbol(akinator->input_buffer, current_position, '('))
    {
        return RECURSION_RETURN_MISSING_SYMBOL_ERROR;
    }

    recursion_return_e output = RECURSION_RETURN_SUCCESS;

    output = ReadNode(akinator, current_position, root_position, EDGE_DIR_LEFT);

    if (output != RECURSION_RETURN_SUCCESS)
    {
        return output;
    }
    
    output = ReadNode(akinator, current_position, root_position, EDGE_DIR_RIGHT);

    if (output != RECURSION_RETURN_SUCCESS)
    {
        return output;
    }

    if (!CheckIfSymbol(akinator->input_buffer, current_position, ')'))
    {
        return RECURSION_RETURN_MISSING_SYMBOL_ERROR;
    }

    return RECURSION_RETURN_SUCCESS;
}

static recursion_return_e 
ReadNode(akinator_t akinator,
         size_t*    current_position,
         size_t     root_position,
         edge_dir_e node_position)
{
    ASSERT(akinator != NULL);
    ASSERT(current_position != NULL);

    node_s node = {.parent_index = (ssize_t) root_position, 
                   .parent_connection = node_position, .right_index = -1,
                   .left_index = -1};

    recursion_return_e output = ReadName(&(node.node_value), 
                                         akinator->input_buffer, 
                                         current_position);
    
    if (output == RECURSION_RETURN_SUCCESS)
    {
        if (TreeAddNode(akinator->object_tree, &node) != 0)
        {
            return RECURSION_RETURN_TREE_ERROR;
        }

        output = RecursiveParser(akinator, current_position,
                                 node.index_in_tree); 

        if (output != RECURSION_RETURN_SUCCESS)
        {
            return output;
        }
    }
    else if (output != RECURSION_RETURN_NIL_SYMBOL)
    {
        return output;
    }
    
    return RECURSION_RETURN_SUCCESS;
}
