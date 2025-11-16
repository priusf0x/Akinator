#include "my_allocator.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Assert.h"

fallosate_return_e 
FallocateInit(fallosate_memory_t* memory_block, 
              size_t              start_size)
{
    ASSERT(memory_block != NULL);

    (*memory_block) = (fallosate_memory_t) calloc(start_size, 
                                                  sizeof (fallosate_memory_s));

    if ((*memory_block) == NULL)
    {
        return FALLOSATE_RETURN_ALLOCATE_ERROR;
    } 

    (*memory_block)->fallosate_pointer = calloc(start_size, 1);

    if ((*memory_block)->fallosate_pointer == NULL)
    {
        free(*memory_block);
        return FALLOSATE_RETURN_ALLOCATE_ERROR;
    }

    (*memory_block)->fallosated_count = start_size;

    return FALLOSATE_RETURN_SUCCESS;
}


fallosate_return_e 
FallocateDestroy(fallosate_memory_t* memory_block)
{
    if ((memory_block != NULL) && (*memory_block) != NULL)
    {
        free((*memory_block)->fallosate_pointer);
        free((*memory_block));
        *memory_block = NULL;
    }

    return FALLOSATE_RETURN_SUCCESS;
}


void* 
falloc(size_t elements_count, 
       size_t element_size, 
       fallosate_memory_t memory_block)
{
    ASSERT(memory_block != NULL);
    ASSERT(memory_block->fallosate_pointer != NULL);

    if (elements_count * element_size //NOTE - will be upgraded
        >= memory_block->fallosated_count - memory_block->fallosated_free)
    {
        return NULL;
    }

    void* return_pointer = (uint8_t*) memory_block->fallosate_pointer 
                                + memory_block->fallosated_free;

    memory_block->fallosated_free += element_size * elements_count;
    
    return return_pointer;
}
