#ifndef AKINATOR_H
#define AKINATOR_H                                                                                                  

#include "tree.h"
#include "my_string.h"
#include "Assert.h"
#include "my_allocator.h"
#include "stack.h"

enum akinator_return_e
{
    AKINATOR_RETURN_SUCCESS,
    AKINATOR_RETURN_TREE_INIT_ERROR,
    AKINATOR_RETURN_ALLOCATION_ERROR,
    AKINATOR_RETURN_SYSTEM_CALL_ERROR,
    AKINATOR_RETURN_FILE_OPEN_ERROR,
    AKINATOR_RETURN_FILE_CLOSE_ERROR,
    AKINATOR_RETURN_EMPTY_FILE,
    AKINATOR_RETURN_READ_ERROR,
    AKINATOR_RETURN_ADD_OBJECT_ERROR,
    AKINATOR_RETURN_EMPTY_BASE,
    AKINATOR_RETURN_STACK_ERROR,
    AKINATOR_RETURN_VALUE_ERROR,
    AKINATOR_RETURN_UNDEFINED_OBJECT,
    AKINATOR_RETURN_PATH_ERROR
};

struct akinator_s
{
    tree_t             object_tree;
    char*              input_buffer;
    fallosate_memory_t memory;
    akinator_return_e  akinator_error;
};

typedef akinator_s* akinator_t;

// =========================== MEMORY_CONTROLLING =============================   

akinator_return_e AkinatorInit(akinator_t* akinator, const char* file_name);
akinator_return_e AkinatorDestroy(akinator_t* akinator);

// ================================== GAME ====================================

akinator_return_e StartAkinatorGame(akinator_t akinator);

// ============================== WRITE_DATA_IN_FILES =========================

akinator_return_e WriteAllData(akinator_t akinator, const char* base_file_name);

// =============================== TREE_ALGORITHMS ============================

ssize_t SearchObject(const akinator_t akinator, const char* object_name);
akinator_return_e PutPathIntoStack(const akinator_t akinator, ssize_t current_node,
                                   swag_t path_stack);

// =============================== ASSERT_AKINATOR ============================

#define ASSERT_AKINATOR(__X___) do {ASSERT((__X___)); ASSERT((__X___)->object_tree != NULL);\
                                            ASSERT((__X___)->input_buffer != NULL);\
                                            ASSERT((__X___)->memory != NULL);} while(0);

#endif // AKINATOR_H