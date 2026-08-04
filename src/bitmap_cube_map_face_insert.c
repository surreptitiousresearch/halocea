/* bitmap_cube_map_face_insert @0x836F5808 — copy a flat face bitmap into one cube-map face of a
 * destination bitmap at the given mipmap and face index. Copy length is the face bitmap's size. */

#include <stdint.h>
#include <string.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format_tables.h"

extern int bitmap_get_pixel_count(const bitmap_data *bitmap);
extern void * bitmap_cube_map_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t face_index, int16_t mipmap_index);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);

void bitmap_cube_map_face_insert(const bitmap_data *face_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, int16_t destination_face_index)
{
    size_t size = (bitmap_get_pixel_count(face_bitmap)
                   * bitmap_format_bits_per_pixel_table[face_bitmap->format]) / 8;
    const void *source = bitmap_mipmap_address(face_bitmap, 0);
    void *destination = bitmap_cube_map_address(destination_bitmap, 0, 0, destination_face_index,
            destination_mipmap_index);
    memcpy(destination, source, size);
}
