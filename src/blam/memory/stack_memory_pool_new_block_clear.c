/* stack_memory_pool_new_block_clear @0x83803970 */
#include <string.h>
#include "headers/stack_memory_pool.h"

extern memory_block *stack_memory_pool_allocate(stack_memory_pool *pool, int size);

memory_block *stack_memory_pool_new_block_clear(stack_memory_pool *pool, unsigned int size)
{
    memory_block *block = stack_memory_pool_allocate(pool, size);
    if (block)
        memset(block + 1, 0, size);
    return block;
}
