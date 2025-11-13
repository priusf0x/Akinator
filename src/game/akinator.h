#ifndef AKINATOR_H
#define AKINATOR_H                                                                                                  

#include "tree.h"
#include "my_string.h"

enum akinator_return_e
{
    AKINATOR_RETURN_SUCCESS,
    AKINATOR_RETURN_TREE_INIT_ERROR,
    AKINATOR_RETURN_ALLOCATION_ERROR,
    AKINATOR_RETURN_SYSTEM_CALL_ERROR,
    AKINATOR_RETURN_FILE_OPEN_ERROR,
    AKINATOR_RETURN_FILE_CLOSE_ERROR,
    AKINATOR_RETURN_EMPTY_FILE,
};

struct akinator_s
{
    tree_t    object_tree;
    char*     input_buffer;
    char*     add_buffer;
};

typedef akinator_s* akinator_t;

// =========================== MEMORY_CONTROLLING =============================   

akinator_return_e AkinatorInit(akinator_t* akinator, const char* file_name);
akinator_return_e AkinatorDestroy(akinator_t* akinator);

// ================================== GAME ====================================

akinator_return_e StartAkinatorGame(akinator_t akinator);


#endif // AKINATOR_H