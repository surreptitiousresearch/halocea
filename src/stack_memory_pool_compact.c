/* stack_memory_pool_compact @0x838032C0 — slide every free (not-in-use) block in the pool's block chain down
 * to close the gap left by the block before it, coalescing free space toward the end of the pool. No-op if
 * the pool is empty or locked. */

#include <string.h>
#include "headers/stack_memory_pool.h"

void stack_memory_pool_compact(stack_memory_pool *pool)
{
    memory_block *block = pool->first_block;
    if ( !block || pool->locked )
        return;

    unsigned char *destination = pool->base_address;
    int gap = 0;

    do
    {
        if ( !MEMORY_BLOCK_IS_IN_USE(block->bits) && (char *)block - gap - (char *)destination > 0 )
        {
            memory_block *moved_block = (memory_block *)&destination[gap];
            memmove(&destination[gap], block, MEMORY_BLOCK_SIZE(block->bits));

            if ( moved_block->previous_block )
                moved_block->previous_block->next_block = moved_block;

            block = moved_block;
        }

        destination = (unsigned char *)block;
        gap = MEMORY_BLOCK_SIZE(block->bits);
        block = block->next_block;
    }
    while ( block );
}
