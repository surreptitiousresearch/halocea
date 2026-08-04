#include "headers/lruv_cache.h"

int lruv_has_locked_proc(const lruv_cache *cache)
{
    return cache->locked_block_proc != 0;
}
