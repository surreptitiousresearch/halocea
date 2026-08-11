/* lruv_block_touched @0x8371C260 — true when a block was already touched this frame (its stamped
 * last_used_frame_index equals the cache's current frame_index). */

#include <stdint.h>
#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"

uint8_t lruv_block_touched(lruv_cache *cache, int block_index)
{
    /* DEVIATION: block_index is a datum HANDLE (salt<<16 | absolute index); the subscript takes
       only the low word (clrlwi r9,r4,16 @ 0x8371C264) — same shape as lruv_block_delete. */
    return cache->frame_index == ((lruv_cache_block *)cache->blocks->data)[(unsigned short)block_index].last_used_frame_index;
}
