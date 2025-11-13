#include "my_string.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "Assert.h"
#include "tree.h"

size_t
SkipSpaces(const char* string,  
           size_t      current_position)
{
    ASSERT(string != NULL);

    char character = *(string + current_position);

    while ((isspace(character)) && (character != '\0'))
    {
        current_position++;
        character = *(string + current_position);
    }

    return current_position;
}

size_t
SkipNotSpaces(const char* string,
              size_t      current_position)
{
    ASSERT(string != NULL);

    char character = *(string + current_position);

    while (!isspace(character) && (character != '\0'))
    {
        current_position++;
        character = *(string + current_position);
    }

    return current_position;
}

static bool 
CheckIfNil(const char* string,  // add current position 
           size_t*     current_position)
{
    const char* stop_word = "nil";

    if (strncmp(string + *current_position, stop_word, strlen(stop_word)) == 0) 
    {
        *current_position += sizeof(stop_word);
        return true;
    }

    return false;
}

bool // returns true if error was occupied;
ReadName(string_s*   string_name,
         char*       input_buffer,
         size_t*     current_position)
{
    ASSERT(string_name);
    ASSERT(input_buffer);
    ASSERT(current_position);

    *current_position = SkipSpaces(input_buffer, *current_position);

    char* current_pointer = input_buffer + *current_position;

    if ((*current_pointer == '(') || (*current_pointer == ')'))
    {
        return true;
    }

    if CheckIfNil

    if ((*current_pointer = '\"') ||
        !CheckIfNil(input_buffer, current_position))
    {
        return true;
    }

    (*current_position)++;
    (*current_pointer)++;

    char* close_position = strchr(input_buffer + *current_pointer, '\"');

    if (close_position == NULL)
    {
        return true ;
    }

    string_name->string_size = (size_t) (close_position - current_pointer);
    string_name->string_source = current_pointer;
    *current_position += string_name->string_size + 1;
    *current_position = SkipSpaces(input_buffer, *current_position);

    if (*current_position != ')')
    {
        return true;
    }

    *current_position = SkipSpaces(input_buffer, current_position + 1);

    return false;
}