/* stack_memory_pool_find_first_unused_memory_block @0x83803218 */
#include "headers/stack_memory_pool.h"

int stack_memory_pool_find_first_unused_memory_block(stack_memory_pool *pool)
{
    unsigned int number_of_blocks = pool->number_of_blocks;

    if (!number_of_blocks)
        return -1;

    int index = 0;
    for (memory_block **block = pool->blocks; *block; ++block)
    {
        ++index;
        if ((unsigned int)index >= number_of_blocks)
            return -1;
    }
    return index;
}
