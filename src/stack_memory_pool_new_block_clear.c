#include "headers/stack_memory_pool.h"

extern memory_block *stack_memory_pool_allocate(stack_memory_pool *pool, int size);
extern void *memset(void *, int, unsigned int);

memory_block *stack_memory_pool_new_block_clear(stack_memory_pool *pool, unsigned int size)
{
    memory_block *block = stack_memory_pool_allocate(pool, size);
    if (block)
        memset(block + 1, 0, size);
    return block;
}
