/* memory_pool_block_allocate @0x8371BE60 — bump-allocate a new block at the tail of a memory_pool.
 * Rounds the requested size (plus the 24-byte header) up to a multiple of 4, places the block right
 * after the current last block (or at the pool base if empty), links it into the block list, stamps
 * the header/trailer signatures, and hands the caller the payload pointer through *reference. Returns
 * 0 (without allocating) if the pool would overflow. */

#include <stdint.h>

#include "headers/memory_pool.h"

/* return attested uint8_t: all 3 callers byte-normalize r3 (clrlwi ...,r3,24 @0x836ECDFC,
 * 0x836F1790, 0x8371BFBC) */
uint8_t memory_pool_block_allocate(memory_pool *pool, void **reference, int size)
{
    int block_size = size + 24;
    memory_pool_block *last_block;
    memory_pool_block *block;

    if ( (block_size & 3) != 0 )
        block_size = (block_size | 3) + 1;   /* round up to 4 */

    last_block = pool->last_block;
    if ( last_block )
        block = (memory_pool_block *)((char *)last_block + last_block->size);
    else
        block = (memory_pool_block *)pool->base_address;

    if ( (char *)block + block_size > (char *)pool->base_address + pool->size || !block )
        return 0;

    block->size = block_size;
    block->reference = reference;
    block->next_block = nullptr;
    block->header_signature = 0x68656164u;   /* 'head' */
    block->previous_block = pool->last_block;
    block->trailer_signature = 0x7461696Cu;   /* 'tail' */

    if ( !pool->first_block )
        pool->first_block = block;
    if ( pool->last_block )
        pool->last_block->next_block = block;

    pool->last_block = block;
    pool->free_size -= block->size;
    *reference = &block[1];           /* payload follows the header */
    return 1;
}
