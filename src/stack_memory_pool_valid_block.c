/* stack_memory_pool_valid_block @0x838038B8 — sanity-check that `block` is a genuine, currently-tracked
 * block of `pool`: within the pool's address range, not null, not the "free sentinel" size (exactly 0x10),
 * and its slot in pool->blocks[] points back to a block with matching bits/previous_block/next_block. */

#include <stdint.h>
#include "headers/stack_memory_pool.h"

uint8_t stack_memory_pool_valid_block(stack_memory_pool *pool, memory_block *block)
{
    unsigned char *base_address = pool->base_address;

    if ( (unsigned char *)block < base_address || (unsigned char *)block >= base_address + pool->pool_size )
        return 0;

    if ( !block || (block->bits & 0x7FFFFFFF) == 0x10 )
        return 0;

    unsigned int index = block->index;
    if ( index >= (unsigned int)pool->number_of_blocks )
        return 0;

    memory_block *tracked = pool->blocks[index];
    if ( !tracked )
        return 0;

    return tracked->bits == block->bits
        && tracked->previous_block == block->previous_block
        && tracked->next_block == block->next_block;
}
