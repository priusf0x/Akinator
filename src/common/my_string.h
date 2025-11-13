#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdlib.h>

struct string_s
{
    char*  string_source;
    size_t string_size;
};


// =========================== STRING_HANDLER_HELPERS =========================

size_t SkipSpaces(const char* string, size_t current_position);
size_t SkipNotSpaces(const char* string, size_t current_position);
bool   ReadName(string_s* string_name, char* input_buffer, size_t* current_position);

// ============================================================================

#endif //MY_STRING_H