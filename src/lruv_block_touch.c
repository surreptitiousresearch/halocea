/* lruv_block_touch @ 0x8371C218 — mark a cache block as used this frame (LRU bookkeeping). */

#include <stdint.h>

#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"

void lruv_block_touch(lruv_cache *cache, uint16_t block_index)
{
    ((lruv_cache_block *)cache->blocks->data)[block_index].last_used_frame_index = cache->frame_index;
}
