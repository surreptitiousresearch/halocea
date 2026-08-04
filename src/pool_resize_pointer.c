/* pool_resize_pointer @0x83803E70 — resizes a stack_memory_pool allocation in place (p may be null for a
 * fresh allocation), marks the returned block in-use, and updates the pool's allocation statistics
 * (bytes/blocks used, peaks, largest block). Returns the user pointer (one past the block header), or
 * null if the underlying resize failed. */

#include "headers/stack_memory_pool.h"

extern memory_block *stack_memory_pool_resize_block(stack_memory_pool *pool, memory_block *reference, unsigned int new_size);

/* Returns void*: callers store the result as an opaque user buffer (stw into fields, used as a string base),
 * never as a memory_block*. It is the user pointer one past the block header. */
void *pool_resize_pointer(stack_memory_pool *pool, void *p, unsigned int new_size)
{
    memory_block *block = p ? (memory_block *)((char *)p - 16) : nullptr;
    unsigned int old_size = block ? MEMORY_BLOCK_SIZE(block->bits) : 0;

    memory_block *new_block = stack_memory_pool_resize_block(pool, block, new_size);
    if (!new_block)
        return nullptr;

    if (!MEMORY_BLOCK_IS_IN_USE(new_block->bits))
        new_block->bits |= MEMORY_BLOCK_IN_USE_FLAG;

    int bytes_allocated = pool->statistics.bytes_allocated;
    int peak_bytes_allocated = pool->statistics.peak_bytes_allocated;
    int size_delta = (int)(MEMORY_BLOCK_SIZE(new_block->bits) - old_size);
    /* old_size == 0 means p was null (a fresh block), so this counts one more block in use. */
    unsigned int blocks_used = pool->statistics.blocks_used + (old_size == 0 ? 1u : 0u);

    pool->statistics.bytes_allocated = size_delta + bytes_allocated;
    pool->statistics.blocks_used = blocks_used;
    if (size_delta + bytes_allocated > peak_bytes_allocated)
        pool->statistics.peak_bytes_allocated = size_delta + bytes_allocated;
    if (blocks_used > pool->statistics.peak_blocks_used)
        pool->statistics.peak_blocks_used = blocks_used;

    unsigned int new_block_size = MEMORY_BLOCK_SIZE(new_block->bits);
    if (new_block_size > pool->statistics.largest_block_allocated)
        pool->statistics.largest_block_allocated = new_block_size;

    return new_block + 1;
}
