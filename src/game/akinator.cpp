#include "akinator.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "Assert.h"
#include "tree.h"

static akinator_return_e ReadFileData(akinator_t akinator, const char* file_name);

akinator_return_e
AkinatorInit(akinator_t*  akinator,
             const char* file_name)
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
        free(akinator);
        return AKINATOR_RETURN_TREE_INIT_ERROR;
    }

    const size_t start_string_count = 10;
    (*akinator)->string_array = (string_s*) calloc(start_string_count, sizeof(string_s));

    if ((*akinator)->string_array == NULL)
    {
        TreeDestroy((*akinator)->object_tree);
        free(akinator);

        return AKINATOR_RETURN_ALLOCATION_ERROR;
    }

    akinator_return_e output = ReadFileData((*akinator), file_name);
    if (output != AKINATOR_RETURN_SUCCESS)
    {
        TreeDestroy((*akinator)->object_tree);
        free((*akinator)->string_array);
        free(*akinator);

        return output;
    }

    return AKINATOR_RETURN_SUCCESS;
}

akinator_return_e 
AkinatorDestroy(akinator_t* akinator)
{
    free((*akinator)->string_array);
    free((*akinator)->input_buffer);
    TreeDestroy((*akinator)->object_tree);
    free((*akinator));
    akinator = NULL;

    return AKINATOR_RETURN_SUCCESS;
}

// ================================= INIT_HELP_FUNCTION =======================

static akinator_return_e
RecursiveParser(akinator_t akinator,
                size_t     current_position)
{
    ASSERT(akinator != NULL);

    akinator->input_buffer



    return AKINATOR_RETURN_SUCCESS;
}

static akinator_return_e 
ReadFileData(akinator_t  akinator,
             const char* file_name)
{
    ASSERT(akinator);

    struct stat file_stat = {};

    if (stat(file_name, &file_stat) != 0)
    {
        return AKINATOR_SYSTEM_CALL_ERROR;
    }

    size_t char_number = (size_t) (file_stat.st_size);

    FILE* file_input = fopen(file_name , "r");
    if (file_input == NULL)
    {
        return AKINATOR_FILE_OPEN_ERROR;
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
        return AKINATOR_FILE_CLOSE_ERROR;
    }

    if (read_count == 0)
    {
        free(akinator->input_buffer);
        return AKINATOR_EMPTY_FILE;
    }    

    return AKINATOR_RETURN_SUCCESS;
}