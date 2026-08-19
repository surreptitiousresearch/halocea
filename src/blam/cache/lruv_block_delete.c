/* lruv_block_delete @ 0x8371C148 — remove a block from an LRU cache: run its delete callback,
 * unlink it from the page-ordered doubly linked list (fixing the cache head/tail), and free its
 * datum. */

#include <stdint.h>
#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"

#include "headers/data_array.h"
extern void datum_delete(data_array *data, int index);

void lruv_block_delete(lruv_cache *cache, int block_index)
{
    lruv_cache_block *blocks = (lruv_cache_block *)cache->blocks->data;
    /* DEVIATION: block_index is a datum HANDLE (salt<<16 | absolute index), so the subscript takes
       the low word exactly as the binary does (clrlwi r8,r4,16 @0x8371C158) and exactly as the two
       unlink subscripts below already spell it. This cast was implicit while the parameter was
       declared uint16_t; widening the parameter is what makes it load-bearing. */
    lruv_cache_block *block = &blocks[(unsigned short)block_index];

    if ( cache->delete_block_proc )
        cache->delete_block_proc(block_index);

    /* unlink from previous */
    if ( block->previous_block_index == -1 )
        cache->first_block_index = block->next_block_index;
    else
        blocks[(unsigned short)block->previous_block_index].next_block_index = block->next_block_index;

    /* unlink from next */
    if ( block->next_block_index == -1 )
        cache->last_block_index = block->previous_block_index;
    else
        blocks[(unsigned short)block->next_block_index].previous_block_index = block->previous_block_index;

    datum_delete(cache->blocks, block_index);
}
