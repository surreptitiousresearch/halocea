/* texture_cache_delete_block_proc @0x836F3F98 — LRU eviction callback for a streaming texture cache
 * block: wait until the block's I/O is no longer in flight, unlink the owning bitmap (clear its
 * cache_block_index), free the bitmap's backing allocation if any, delete the bitmap's rasterizer
 * resources, then free the cache datum. */

#include "headers/pc_texture_cache_globals.h"
#include "headers/texture_cache_texture.h"

extern void Sleep(unsigned int milliseconds);
extern void dlFree(void *ptr);
extern void rasterizer_bitmap_delete(bitmap_data *bitmap);
extern void datum_delete(data_array *data, int index);

void texture_cache_delete_block_proc(int block_index)
{
    texture_cache_texture *slot =
        DATA_ARRAY_ELEMENT(pc_texture_cache_globals.textures, texture_cache_texture, block_index);
    if ( !slot->finished )
    {
        do
            Sleep(0);
        while ( !slot->finished );
    }
    bitmap_data *bitmap = slot->bitmap;
    bitmap->cache_block_index = -1;
    if ( bitmap->base_address )
    {
        dlFree(bitmap->base_address);
        slot->bitmap->base_address = 0;
    }
    rasterizer_bitmap_delete(slot->bitmap);
    datum_delete(pc_texture_cache_globals.textures, block_index);
}
