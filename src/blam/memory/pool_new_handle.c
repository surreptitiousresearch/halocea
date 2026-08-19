/* pool_new_handle @0x83803A68 — allocate `size` bytes from a stack memory pool as a relocation-stable
 * "handle" (the block header pointer itself, unlike pool_new_pointer.c which returns the payload pointer
 * past the header), updating the pool's running/peak statistics. Returns null if the pool is full. */

#include "headers/stack_memory_pool.h"

extern memory_block *stack_memory_pool_allocate(stack_memory_pool *pool, int size);

void ** pool_new_handle(stack_memory_pool *pool, unsigned int size)
{
    memory_block *block = stack_memory_pool_allocate(pool, size);
    if ( !block )
        return 0;

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
