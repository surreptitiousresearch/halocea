/* memory_pool_block_free @0x8371BC18 — unlink a block from its memory_pool's doubly-linked block list
 * and return its bytes to the pool's free count. `reference` points at the payload; the 24-byte header
 * precedes it. */

#include "headers/memory_pool.h"

void memory_pool_block_free(memory_pool *pool, void **reference)
{
    memory_pool_block *block = (memory_pool_block *)((char *)*reference - 24);

    pool->free_size += block->size;

    if ( block->previous_block )
        block->previous_block->next_block = block->next_block;
    else
        pool->first_block = block->next_block;

    if ( block->next_block )
        block->next_block->previous_block = block->previous_block;
    else
        pool->last_block = block->previous_block;
}
