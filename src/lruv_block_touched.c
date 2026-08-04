/* lruv_block_touched @0x8371C260 — true when a block was already touched this frame (its stamped
 * last_used_frame_index equals the cache's current frame_index). */

#include <stdint.h>
#include "headers/lruv_cache.h"
#include "headers/lruv_cache_block.h"

uint8_t lruv_block_touched(lruv_cache *cache, uint16_t block_index)
{
    return cache->frame_index == ((lruv_cache_block *)cache->blocks->data)[block_index].last_used_frame_index;
}
