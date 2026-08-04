/* stack_memory_pool_find_space_between_blocks @0x83803360 — find a gap of at least `size` bytes: before the
 * first block, or between two adjacent blocks. Returns the gap's start address (and, for the between-blocks
 * case, sets *free_space_in_pool_previous to the block just before the gap), or null if no gap fits. */

#include <stdint.h>
#include "headers/stack_memory_pool.h"

uint8_t * stack_memory_pool_find_space_between_blocks(stack_memory_pool *pool, unsigned int size, memory_block **free_space_in_pool_previous)
{
    memory_block *first_block = pool->first_block;

    if ( !first_block )
        return 0;

    if ( (unsigned int)((char *)first_block - (char *)pool->base_address) < size )
    {
        memory_block *next_block = first_block->next_block;
        unsigned int first_block_size;

        if ( !next_block )
            return 0;

        while ( 1 )
        {
            first_block_size = first_block->bits & 0x7FFFFFFF;

            if ( (unsigned int)((char *)next_block - first_block_size - (char *)first_block) >= size )
                break;

            first_block = next_block;
            next_block = next_block->next_block;
            if ( !next_block )
                return 0;
        }

        *free_space_in_pool_previous = first_block;
        return (unsigned __int8 *)first_block + first_block_size;
    }

    return pool->base_address;
}
