/* texture_cache_start_loading_bitmap @0x836F4038 — reserve an LRU cache block for a streaming bitmap's
 * pixel data and begin loading it: allocates a pixel buffer (at least 4 bytes), links a new cache-texture
 * datum at the block's page offset (texture_cache_texture, 16-byte stride), and — unless texture
 * streaming is globally disabled — issues the async cache_file_read. Returns 0 only if no cache block
 * could be allocated. */

/* CAVEAT: dead in the shipped image. This function has ZERO code cross-references --
 * its only reference is the ExceptionDir unwind entry at 0x8236AB98. The texture-streaming path is compiled in
 * and never armed, so a reader should not assume this path runs at runtime.
 * (xrefs to 0x836F4038: 1 total, 0 code.) */

#include <stdint.h>
#include "headers/pc_texture_cache_globals.h"
#include "headers/texture_cache_texture.h"
#include "headers/cache_file_read_request_params.h"
#include "headers/bitmap_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int lruv_block_new(lruv_cache *cache, int size);
extern int datum_new_at_index(data_array *data, int index);
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern int16_t cache_file_read(int tag_index, uint32_t offset, int size, void *buffer, const cache_file_read_request_params *params, uint8_t blocking, uint8_t data_file);

int texture_cache_start_loading_bitmap(bitmap_data *bitmap, uint8_t block)
{
    int cache_block_index = lruv_block_new(pc_texture_cache_globals.cache, 4);
    if ( cache_block_index == -1 )
        return 0;

    int pixels_size = bitmap->pixels_size;
    if ( pixels_size < 4 )
        pixels_size = 4;

    void *pixel_buffer = dlMalloc(pixels_size, "D:\\Projects\\code\\HCEX\\sources\\cache\\pc_texture_cache.c", 0x21Eu);

    datum_new_at_index(pc_texture_cache_globals.textures, cache_block_index);
    texture_cache_texture *slot =
        DATA_ARRAY_ELEMENT(pc_texture_cache_globals.textures, texture_cache_texture, cache_block_index);

    cache_file_read_request_params params;
    params.finished_func = 0;
    params.userdata = 0;
    params.finished_flag = &slot->finished;

    bitmap->cache_block_index = cache_block_index;
    bitmap->base_address = pixel_buffer;
    slot->bitmap = bitmap;
    slot->hardware_format = bitmap->hardware_format;

    unsigned char data_file = (bitmap->flags & (1u << _bitmap_data_file_cache_bit)) != 0;

    if ( !hcex_off_tex_streaming )
        slot->read_request_handle = cache_file_read(bitmap->tag_index, bitmap->pixels_offset, bitmap->pixels_size,
                                       pixel_buffer, &params, block, data_file);

    return 1;
}
