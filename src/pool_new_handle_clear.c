/* pool_new_handle_clear @0x83803B10 — pool_new_handle.c, but zero-fills the allocated payload first. */

#include <string.h>
#include "headers/stack_memory_pool.h"

extern memory_block *stack_memory_pool_allocate(stack_memory_pool *pool, int size);

void **pool_new_handle_clear(stack_memory_pool *pool, unsigned int size)
{
    memory_block *block = stack_memory_pool_allocate(pool, size);
    if ( !block )
        return 0;

    memset(block + 1, 0, size);

    int total_bytes = (block->bits & 0x7FFFFFFF) + pool->statistics.bytes_allocated;
    unsigned int total_blocks = pool->statistics.blocks_used + 1;
    pool->statistics.bytes_allocated = total_bytes;
    pool->statistics.blocks_used = total_blocks;

    if ( total_bytes > pool->statistics.peak_bytes_allocated )
        pool->statistics.peak_bytes_allocated = total_bytes;
    if ( total_blocks > pool->statistics.peak_blocks_used )
        pool->statistics.peak_blocks_used = total_blocks;

    unsigned int block_size = block->bits & 0x7FFFFFFF;
    if ( block_size > (unsigned int)pool->statistics.largest_block_allocated )
        pool->statistics.largest_block_allocated = block_size;

    return (void **)block;
}
