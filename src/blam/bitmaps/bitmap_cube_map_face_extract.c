/* bitmap_cube_map_face_extract @0x836F5780 — copy one cube-map face (at the given mipmap and face
 * index) out into a flat destination bitmap whose size determines the copy length. */

#include <stdint.h>
#include <string.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format_tables.h"

extern int bitmap_get_pixel_count(const bitmap_data *bitmap);
extern void * bitmap_cube_map_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t face_index, int16_t mipmap_index);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);

void bitmap_cube_map_face_extract(const bitmap_data *source_bitmap, int16_t source_mipmap_index, int16_t source_face_index, bitmap_data *face_bitmap)
{
    size_t size = (bitmap_get_pixel_count(face_bitmap)
                   * bitmap_format_bits_per_pixel_table[face_bitmap->format]) / 8;
    const void *source = bitmap_cube_map_address(source_bitmap, 0, 0, source_face_index,
            source_mipmap_index);
    void *destination = bitmap_mipmap_address(face_bitmap, 0);
    memcpy(destination, source, size);
}
