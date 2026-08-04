/* dispose_pointer @ 0x838034B8 — free a block previously handed out by a stack memory pool.
 * The block header sits one memory_block before the user pointer and holds the size; after
 * releasing it, the pool's allocated-bytes and block-count bookkeeping are decremented.
 *
 * The decompiler lost the base register holding `pool` for the final bookkeeping writes
 * (shown as an uninitialized v3); reconstructed here as `pool`, which owns those fields. */

#include "headers/stack_memory_pool.h"

extern stack_memory_pool *stack_memory_pool_dispose_block(stack_memory_pool *pool, const memory_block *reference);

void dispose_pointer(stack_memory_pool *pool, void *p)
{
    /* generic deallocator: the block header sits one memory_block before the user pointer.
       param is void* (called with every allocated type); reinterpret localized here. */
    memory_block *block = (memory_block *)p - 1;
    unsigned int block_size = block->bits & 0x7FFFFFFF;

    stack_memory_pool_dispose_block(pool, block);
    pool->statistics.bytes_allocated -= block_size;
    --pool->statistics.blocks_used;
}
