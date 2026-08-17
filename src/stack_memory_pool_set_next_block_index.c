/* stack_memory_pool_set_next_block_index @0x83803260 */
#include "headers/stack_memory_pool.h"

void stack_memory_pool_set_next_block_index(stack_memory_pool *pool)
{
    int next_block_index = pool->next_block_index;

    if ( next_block_index == -1 )
        return;

    int block_index = next_block_index + 1;
    pool->next_block_index = -1;

    if ( block_index >= pool->number_of_blocks )
        return;

    for ( memory_block **block = &pool->blocks[block_index]; *block; ++block )
    {
        if ( ++block_index >= pool->number_of_blocks )
            return;
    }
    pool->next_block_index = block_index;
}
