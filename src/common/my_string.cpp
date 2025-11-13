#include "string.h"

#include <stdio.h>

char*
SkipSpaces(char* string)
{
    ASSERT(string != NULL);

    char character = *string;

    while ((character == ' ') && (character != '\0'))
    {
        string++;
        character = *string;
    }

    return string;
}

char*
SkipNotSpaces(char* string)
{
    ASSERT(string != NULL);

    char character = *string;

    while (!isspace(character) && (character != '\0'))
    {
        string++;
        character = *string;
    }

    return string;
}