/* texture_cache_bitmap_delete @0x836F3EA8 — evict a cached bitmap from the streaming texture cache: if
 * it owns a cache block (_bitmap_cached_bit), delete that LRU block and clear the bitmap's cache linkage. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"
#include "headers/pc_texture_cache_globals.h"

#include "headers/lruv_cache.h"
extern void lruv_block_delete(lruv_cache *cache, int block_index);

void texture_cache_bitmap_delete(bitmap_data *bitmap)
{
    if ( (bitmap->flags & (1u << _bitmap_cached_bit)) != 0 )
    {
        int cache_block_index = bitmap->cache_block_index;
        if ( cache_block_index != -1 )
            lruv_block_delete(pc_texture_cache_globals.cache, cache_block_index);
        bitmap->cache_block_index = -1;
        bitmap->base_address = 0;
    }
}
