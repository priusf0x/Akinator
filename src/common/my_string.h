#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdlib.h>

struct string_s
{
    char*  string_source;
    size_t string_size;
};


// ================

char* SkipSpaces(char* string);
char* SkipNotSpaces(char* string);

#endif //MY_STRING_H