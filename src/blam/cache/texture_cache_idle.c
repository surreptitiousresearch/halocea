/* texture_cache_idle @0x836F3E18 — idles the PC texture LRU cache (per-frame maintenance). */

#include "headers/pc_texture_cache_globals.h"

extern void lruv_idle(lruv_cache *cache);

void texture_cache_idle(void)
{
    lruv_idle(pc_texture_cache_globals.cache);
}
