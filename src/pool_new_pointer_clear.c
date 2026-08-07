/* pool_new_pointer_clear @0x83803DD0 — same as pool_new_pointer.c, but zero-fills the returned payload
 * before updating the pool statistics. Kept as its own function (rather than pool_new_pointer + memset)
 * to match the compiled call site exactly, but the statistics bookkeeping is identical. */

#include <string.h>
#include "headers/stack_memory_pool.h"

extern memory_block *stack_memory_pool_allocate(stack_memory_pool *pool, int size);

void *pool_new_pointer_clear(stack_memory_pool *pool, unsigned int size)
{
    memory_block *block = stack_memory_pool_allocate(pool, size);
    if ( !block )
        return 0;

    memset(block + 1, 0, size);

    unsigned int marked_bits = block->bits | MEMORY_BLOCK_IN_USE_FLAG;
    block->bits = marked_bits;

    int total_bytes = MEMORY_BLOCK_SIZE(marked_bits) + pool->statistics.bytes_allocated;
    unsigned int total_blocks = pool->statistics.blocks_used + 1;
    pool->statistics.bytes_allocated = total_bytes;
    pool->statistics.blocks_used = total_blocks;

    if ( total_bytes > pool->statistics.peak_bytes_allocated )
        pool->statistics.peak_bytes_allocated = total_bytes;
    if ( total_blocks > pool->statistics.peak_blocks_used )
        pool->statistics.peak_blocks_used = total_blocks;

    unsigned int block_size = MEMORY_BLOCK_SIZE(block->bits);
    if ( block_size > (unsigned int)pool->statistics.largest_block_allocated )
        pool->statistics.largest_block_allocated = block_size;

    return block + 1;
}
