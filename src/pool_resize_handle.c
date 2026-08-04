/* pool_resize_handle @0x83803BB8 — resize an allocation held via a handle (*h) in a stack_memory_pool,
 * validating first that the handle still points at a genuine, currently-allocated block belonging to
 * this pool (in range, not a free block per its bits' in-use flag, and its slot in the pool's block
 * table still round-trips back to an identical block) before resizing. Updates the pool's
 * bytes/peak/largest-block statistics on success. Returns 0 if the handle didn't validate or the resize
 * failed, 1 on success. */

#include <stdint.h>
#include "headers/stack_memory_pool.h"

extern memory_block *stack_memory_pool_resize_block(stack_memory_pool *pool, memory_block *reference, unsigned int new_size);

uint8_t pool_resize_handle(stack_memory_pool *pool, void ***h, unsigned int new_size)
{
    memory_block *block = (memory_block *)*h;
    unsigned char *base_address = pool->base_address;

    unsigned __int8 valid = 0;
    if ( block >= (memory_block *)base_address && block < (memory_block *)&base_address[pool->pool_size] )
    {
        unsigned __int8 in_use = block && (block->bits & 0x7FFFFFFF) != 0x10;

        if ( in_use )
        {
            unsigned int index = block->index;
            if ( index < (unsigned int)pool->number_of_blocks )
            {
                memory_block *table_block = pool->blocks[index];
                if ( table_block
                  && table_block->bits == block->bits
                  && table_block->previous_block == block->previous_block
                  && table_block->next_block == block->next_block )
                    valid = 1;
            }
        }
    }

    if ( !valid )
        return valid;

    unsigned int old_size = block->bits & 0x7FFFFFFF;
    memory_block *resized = stack_memory_pool_resize_block(pool, block, new_size);
    if ( !resized )
        return 0;

    *h = (void **)resized;

    int peak_bytes_allocated = pool->statistics.peak_bytes_allocated;
    int bytes_allocated = (resized->bits & 0x7FFFFFFF) - old_size + pool->statistics.bytes_allocated;
    pool->statistics.bytes_allocated = bytes_allocated;

    if ( bytes_allocated > peak_bytes_allocated )
        pool->statistics.peak_bytes_allocated = bytes_allocated;

    unsigned int new_block_size = resized->bits & 0x7FFFFFFF;
    if ( new_block_size > (unsigned int)pool->statistics.largest_block_allocated )
        pool->statistics.largest_block_allocated = new_block_size;

    return valid;
}
