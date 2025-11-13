#ifndef AKINATOR_H
#define AKINATOR_H

#include "tree.h"
#include "my_string.h"

enum akinator_return_e
{
    AKINATOR_RETURN_SUCCESS,
    AKINATOR_RETURN_TREE_INIT_ERROR,
    AKINATOR_RETURN_ALLOCATION_ERROR
};

struct akinator_s
{
    tree_t    object_tree;
    string_s* string_array;
    char*     input_buffer;
    char*     add_buffer;
};

typedef akinator_s* akinator_t;

// =========================== MEMORY_CONTROLLING ============================= 

akinator_return_e AkinatorInit(akinator_t* akinator);
akinator_return_e AkinatorDestroy(akinator_t* akinator);

// ================================== GAME ====================================

akinator_return_e StartAkinatorGame(akinator_t akinator);


#endif // AKINATOR_H