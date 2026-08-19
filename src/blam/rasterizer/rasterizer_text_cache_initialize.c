/* rasterizer_text_cache_initialize @ 0x83768A88
   Allocates the 512x512 hardware character-cache bitmap, registers it with the
   rasterizer, and marks the cache initialized. */

#include <stdint.h>
#include <string.h>
#include "headers/hardware_character_cache.h"
#include "headers/bitmap_format.h"

extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern uint8_t rasterizer_bitmap_new(bitmap_data *bitmap);
extern void rasterizer_bitmap_changed(bitmap_data *bitmap);

uint8_t rasterizer_text_cache_initialize(void)
{
    memset(&hardware_character_cache, 0, sizeof(hardware_character_cache));

    bitmap_data *bitmap = bitmap_2d_new(512, 512, 0, _bitmap_format_a8r8g8b8);
    hardware_character_cache.bitmap = bitmap;
    if ( !bitmap || !rasterizer_bitmap_new(bitmap) )
        return 0;

    rasterizer_bitmap_changed(hardware_character_cache.bitmap);
    hardware_character_cache.initialized = 1;
    return 1;
}
