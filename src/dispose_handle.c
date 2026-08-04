/* dispose_handle @0x838033D8 — free a block previously handed out by a stack memory pool, addressed by
 * its handle. Same shape as dispose_pointer, except the block header precedes the handle itself.
 *
 * DEVIATION: the decompiler lost the base register holding `pool` for the final bookkeeping writes
 * (shown as an uninitialized v3 read at v3+20/v3+28); reconstructed here as `pool`, which owns those
 * fields (same fix as dispose_pointer). */

#include "headers/stack_memory_pool.h"

extern stack_memory_pool *stack_memory_pool_dispose_block(stack_memory_pool *pool, const memory_block *reference);

void dispose_handle(stack_memory_pool *pool, void **h)
{
    memory_block *block = (memory_block *)h - 1;
    unsigned int block_size = block->bits & 0x7FFFFFFF;

    stack_memory_pool_dispose_block(pool, block);
    pool->statistics.bytes_allocated -= block_size;
    --pool->statistics.blocks_used;
}
