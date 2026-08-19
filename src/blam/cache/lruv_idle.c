/* lruv_idle @ 0x8371C138 — advance the cache's frame counter (drives LRU aging). */

#include "headers/lruv_cache.h"

void lruv_idle(lruv_cache *cache)
{
    ++cache->frame_index;
}
