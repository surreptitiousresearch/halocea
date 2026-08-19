/* bitmap_compress_to_mipmap @0x8377E050 — dispatch a bitmap-type-appropriate mipmap compression: type 1
 * (3D) delegates to bitmap_3d_compress_to_mipmap.c; type 2 (cube maps, "< 3" after the type==0 early-out)
 * builds a per-face scratch pair and processes each of the 6 faces via bitmap_cube_map_face_extract/insert.
 * Type 0 (2D) does nothing here (handled elsewhere).
 *
 * SHIPPED BUG (same class as bitmap_3d_compress_to_mipmap.c / bitmap_cm_compress_to_mipmap.c): the insert
 * reads destination_face, which is never written by any extract call — only source_face is populated per
 * face. Reproduced verbatim, as compiled. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"

extern void bitmap_3d_compress_to_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, const unsigned int *transparent_color);
extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern void bitmap_cube_map_face_extract(const bitmap_data *source_bitmap, int16_t source_mipmap_index, int16_t source_face_index, bitmap_data *face_bitmap);
extern void bitmap_cube_map_face_insert(const bitmap_data *face_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, int16_t destination_face_index);
extern void bitmap_delete(bitmap_data *bitmap);

void bitmap_compress_to_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, const unsigned int *transparent_color)
{
    if ( source_bitmap->type == bitmap_type_2d )
        return;

    if ( source_bitmap->type == bitmap_type_3d )
    {
        bitmap_3d_compress_to_mipmap(source_bitmap, destination_bitmap, destination_mipmap_index, transparent_color);
        return;
    }

    if ( source_bitmap->type >= bitmap_type_white )
        return;

    bitmap_data *source_face = bitmap_2d_new(source_bitmap->width, source_bitmap->height, 0, source_bitmap->format);
    bitmap_data *destination_face = bitmap_2d_new(source_bitmap->width, source_bitmap->height, 0, destination_bitmap->format);

    if ( source_face && source_face->base_address && destination_face && destination_face->base_address )
    {
        for ( int16_t face_index = 0; face_index < 6; face_index++ )
        {
            bitmap_cube_map_face_extract(source_bitmap, 0, face_index, source_face);
            bitmap_cube_map_face_insert(destination_face, destination_bitmap, destination_mipmap_index, face_index);
        }
    }

    bitmap_delete(source_face);
    bitmap_delete(destination_face);
}
