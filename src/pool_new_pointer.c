/* pool_new_pointer @ 0x83803D00 — allocate `size` bytes from a stack memory pool, mark the
 * block in-use, update the pool's running/peak statistics, and return a pointer to the user
 * payload (just past the block header). Returns null if the pool is full. */

#include "headers/stack_memory_pool.h"

extern memory_block *stack_memory_pool_allocate(stack_memory_pool *pool, int size);

void *pool_new_pointer(stack_memory_pool *pool, unsigned int size)
{
    memory_block *block = stack_memory_pool_allocate(pool, size);
    unsigned int marked_bits;
    int total_bytes;
    unsigned int total_blocks;
    unsigned int block_size;

    if ( !block )
        return 0;

    marked_bits = block->bits | MEMORY_BLOCK_IN_USE_FLAG;
    block->bits = marked_bits;

    total_bytes = MEMORY_BLOCK_SIZE(marked_bits) + pool->statistics.bytes_allocated;
    total_blocks = pool->statistics.blocks_used + 1;
    pool->statistics.bytes_allocated = total_bytes;
    pool->statistics.blocks_used = total_blocks;

    if ( total_bytes > pool->statistics.peak_bytes_allocated )
        pool->statistics.peak_bytes_allocated = total_bytes;
    if ( total_blocks > pool->statistics.peak_blocks_used )
        pool->statistics.peak_blocks_used = total_blocks;

    block_size = MEMORY_BLOCK_SIZE(block->bits);
    if ( block_size > (unsigned int)pool->statistics.largest_block_allocated )
        pool->statistics.largest_block_allocated = block_size;

    return block + 1;
}
