/* bitmap_get_pixel_data_size @0x836F5108 — total byte size of all pixel data across every mipmap
 * level: pixel count times bits-per-pixel of the format, in bytes. */

#include "headers/bitmap_data.h"
#include "headers/bitmap_format_tables.h"

extern int bitmap_get_pixel_count(const bitmap_data *bitmap);

int bitmap_get_pixel_data_size(const bitmap_data *bitmap)
{
    return bitmap_get_pixel_count(bitmap) * bitmap_format_bits_per_pixel_table[bitmap->format] / 8;
}
