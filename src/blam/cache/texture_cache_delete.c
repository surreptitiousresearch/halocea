/* texture_cache_delete @ 0x836F3DC8
   Dispose the texture data array and delete the LRU cache. */
#include "headers/pc_texture_cache_globals.h"

extern void data_dispose(data_array *data);
extern void lruv_delete(lruv_cache *cache);

void texture_cache_delete(void)
{
    data_dispose(pc_texture_cache_globals.textures);
    lruv_delete(pc_texture_cache_globals.cache);
}
