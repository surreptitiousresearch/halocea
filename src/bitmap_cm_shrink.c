/* bitmap_cm_shrink @0x8377AB10 — shrink a cube-map bitmap by extracting each of its 6 faces into a scratch
 * 2D bitmap, running bitmap_2d_shrink on it, and inserting the shrunk face into a freshly allocated
 * (width/scale)-sized cube map. `scale` is clamped so it never divides down to less than 1x1 (min divisor
 * is the source width itself). Returns the new cube map, or NULL if either allocation failed. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format.h"

extern bitmap_data * bitmap_cube_map_new(int16_t width, int16_t mipmap_count, int16_t format);
extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern void bitmap_cube_map_face_extract(const bitmap_data *source_bitmap, int16_t source_mipmap_index, int16_t source_face_index, bitmap_data *face_bitmap);
extern bitmap_data * bitmap_2d_shrink(const bitmap_data *source_bitmap, int16_t scale, int16_t alpha_bias, uint8_t ignore_zero_alpha);
extern void bitmap_cube_map_face_insert(const bitmap_data *face_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, int16_t destination_face_index);
extern void bitmap_delete(bitmap_data *bitmap);

bitmap_data * bitmap_cm_shrink(const bitmap_data *source_bitmap, int16_t scale, int16_t alpha_bias, uint8_t ignore_zero_alpha)
{
    int16_t divisor = scale;
    if ( scale > source_bitmap->width )
        divisor = source_bitmap->width;

    bitmap_data *shrunk_cube_map = bitmap_cube_map_new(source_bitmap->width / divisor, 0, _bitmap_format_a8r8g8b8);
    if ( shrunk_cube_map && shrunk_cube_map->base_address )
    {
        bitmap_data *face_scratch = bitmap_2d_new(source_bitmap->width, source_bitmap->height, 0, _bitmap_format_a8r8g8b8);
        if ( face_scratch && face_scratch->base_address )
        {
            for ( int16_t face_index = 0; face_index < 6; face_index++ )
            {
                bitmap_cube_map_face_extract(source_bitmap, 0, face_index, face_scratch);
                bitmap_data *shrunk_face = bitmap_2d_shrink(face_scratch, scale, alpha_bias, ignore_zero_alpha);
                if ( shrunk_face && shrunk_face->base_address )
                    bitmap_cube_map_face_insert(shrunk_face, shrunk_cube_map, 0, face_index);
                bitmap_delete(shrunk_face);
            }
        }
        bitmap_delete(face_scratch);
    }

    return shrunk_cube_map;
}
