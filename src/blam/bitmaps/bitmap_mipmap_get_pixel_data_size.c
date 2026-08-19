/* bitmap_mipmap_get_pixel_data_size @0x836F4A70 — byte size of a single mipmap level's pixel data:
 * that level's pixel count times the format's bits-per-pixel, in bytes. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format_tables.h"

extern int bitmap_mipmap_get_pixel_count(const bitmap_data *bitmap, int16_t mipmap_index);

int bitmap_mipmap_get_pixel_data_size(const bitmap_data *bitmap, int16_t mipmap_index)
{
    return bitmap_mipmap_get_pixel_count(bitmap, mipmap_index)
         * bitmap_format_bits_per_pixel_table[bitmap->format] / 8;
}
