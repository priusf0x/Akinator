#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <stdlib.h> 

struct fallosate_memory_s
{
    void*  fallosate_pointer;
    size_t fallosated_count;
    size_t fallosated_free; 
};

enum fallosate_return_e
{
    FALLOSATE_RETURN_SUCCESS,
    FALLOSATE_RETURN_ALLOCATE_ERROR
};

typedef fallosate_memory_s* fallosate_memory_t;

// ========================== MEMORY_CONTROLLING ==============================

fallosate_return_e FallosateInit(fallosate_memory_t* memory_block, size_t start_size);
fallosate_return_e FallosateDestroy(fallosate_memory_t* memory_block);

void* fallosator(size_t elements_count, size_t element_size, fallosate_memory_t memory_block);

#endif // MY_ALLOCATOR_H