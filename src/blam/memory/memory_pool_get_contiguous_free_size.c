/* memory_pool_get_contiguous_free_size @0x8371BD38 — bytes of contiguous free space remaining at the
 * tail of a memory pool: total size minus the span already consumed up through the last block. */

#include "headers/memory_pool.h"

int memory_pool_get_contiguous_free_size(memory_pool *pool)
{
    if ( pool->last_block )
        /* pointer difference spelled with char* (was (unsigned int) pointer truncation — x64-unsafe) */
        return pool->size
             - (int)(((char *)pool->last_block + pool->last_block->size) - (char *)pool->base_address);
    return pool->size;
}
