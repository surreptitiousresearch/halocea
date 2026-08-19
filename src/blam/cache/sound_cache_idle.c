/* sound_cache_idle @ 0x837E7350 — service the sound cache's LRU eviction/streaming. */

#include "headers/pc_sound_cache_globals.h"

extern void lruv_idle(lruv_cache *cache);

void sound_cache_idle(void)
{
    lruv_idle(pc_sound_cache_globals.cache);
}
