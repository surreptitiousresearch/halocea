/* lruv_block_touch @ 0x8371C218 — mark a cache block as used this frame (LRU bookkeeping). */

#include <stdint.h>

#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"

void lruv_block_touch(lruv_cache *cache, int block_index)
{
    /* DEVIATION: block_index is a datum HANDLE (salt<<16 | absolute index); the subscript takes
       only the low word (clrlwi r9,r4,16 @ 0x8371C21C) — same shape as lruv_block_delete. */
    ((lruv_cache_block *)cache->blocks->data)[(unsigned short)block_index].last_used_frame_index = cache->frame_index;
}
