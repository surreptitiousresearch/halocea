#include "headers/pc_texture_cache_globals.h"

#include "headers/lruv_cache.h"
extern void lruv_flush(lruv_cache *cache);

void texture_cache_flush(void)
{
    lruv_flush(pc_texture_cache_globals.cache);
}
