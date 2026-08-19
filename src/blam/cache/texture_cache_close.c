/* texture_cache_close @0x836F41C0 — flush the LRU texture cache and invalidate the texture data array. */
#include "headers/pc_texture_cache_globals.h"
#include "headers/data_array.h"
#include "headers/lruv_cache.h"
extern void lruv_flush(lruv_cache *cache);
extern void data_make_invalid(data_array *data);
void texture_cache_close(void)
{
    lruv_flush(pc_texture_cache_globals.cache);
    data_make_invalid(pc_texture_cache_globals.textures);
}
