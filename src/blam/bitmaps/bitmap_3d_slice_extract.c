/* bitmap_3d_slice_extract @0x836F5678 — copy one Z-slice of a 3D bitmap (at the given mipmap and
 * slice index) out into a flat destination bitmap whose size determines the copy length. */

#include <stdint.h>
#include <string.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format_tables.h"

extern int bitmap_get_pixel_count(const bitmap_data *bitmap);
extern char *bitmap_3d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t z, int16_t mipmap_index);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);

void bitmap_3d_slice_extract(const bitmap_data *source_bitmap, int16_t source_mipmap_index, int16_t source_slice_index, bitmap_data *slice_bitmap)
{
    size_t size = (bitmap_get_pixel_count(slice_bitmap)
                   * bitmap_format_bits_per_pixel_table[slice_bitmap->format]) / 8;
    const void *source = bitmap_3d_address(source_bitmap, 0, 0, source_slice_index,
            source_mipmap_index);
    void *destination = bitmap_mipmap_address(slice_bitmap, 0);
    memcpy(destination, source, size);
}
