/* bitmap_3d_slice_insert @0x836F5700 — copy a flat slice bitmap into one Z-slice of a 3D bitmap at
 * the given mipmap and slice index. Copy length is the slice bitmap's pixel-data size. */

#include <stdint.h>
#include <string.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format_tables.h"

extern int bitmap_get_pixel_count(const bitmap_data *bitmap);
extern char *bitmap_3d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t z, int16_t mipmap_index);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);

void bitmap_3d_slice_insert(const bitmap_data *slice_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, int16_t destination_slice_index)
{
    size_t size = (bitmap_get_pixel_count(slice_bitmap)
                   * bitmap_format_bits_per_pixel_table[slice_bitmap->format]) / 8;
    const void *source = bitmap_mipmap_address(slice_bitmap, 0);
    void *destination = bitmap_3d_address(destination_bitmap, 0, 0, destination_slice_index,
            destination_mipmap_index);
    memcpy(destination, source, size);
}
