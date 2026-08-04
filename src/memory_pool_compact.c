/* memory_pool_compact @0x8371BC70 — slide every live block in a memory pool down to remove inter-block gaps,
 * patching each block's back-reference and rebuilding the previous/next chain. After compaction the pool's
 * blocks are contiguous starting at base_address. */

#include <string.h>

#include "headers/memory_pool.h"
#include "headers/memory_pool_block.h"

void memory_pool_compact(memory_pool *pool)
{
    memory_pool_block *block = pool->first_block;
    if ( block )
    {
        memory_pool_block *destination = (memory_pool_block *)pool->base_address;
        memory_pool_block *previous = 0;
        memory_pool_block **p_next_block;
        do
        {
            if ( block > destination )
            {
                memmove(destination, block, block->size);
                block = destination;
                *block->reference = &block[1]; /* re-point the owner's handle at the moved block's data */
            }
            block->previous_block = previous;
            if ( previous )
                previous->next_block = block;
            else
                pool->first_block = block;
            p_next_block = &block->next_block;
            previous = block;
            destination = (memory_pool_block *)((char *)block + block->size);
            block = block->next_block;
        }
        while ( block );
        *p_next_block = 0;
        pool->last_block = previous;
    }
}
