/* bitmap_cm_uncompress_from_mipmap @0x8377CFF0 — uncompress one mipmap level of a cube map into a
 * (full-size) destination cube map: per face, extract the source face into a source-format scratch,
 * uncompress it into a destination-format scratch, and insert that into the destination's level 0.
 * (Unlike bitmap_cm_compress_to_mipmap, this direction does perform the conversion.) */

#include <stdint.h>
#include "headers/bitmap_data.h"

extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern void bitmap_cube_map_face_extract(const bitmap_data *source_bitmap, int16_t source_mipmap_index, int16_t source_face_index, bitmap_data *face_bitmap);
extern void bitmap_2d_uncompress_from_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t source_mipmap_index);
extern void bitmap_cube_map_face_insert(const bitmap_data *face_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, int16_t destination_face_index);
extern void bitmap_delete(bitmap_data *bitmap);

void bitmap_cm_uncompress_from_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t source_mipmap_index)
{
    bitmap_data *source_face = bitmap_2d_new(destination_bitmap->width, destination_bitmap->height, 0,
                                             source_bitmap->format);
    bitmap_data *destination_face = bitmap_2d_new(destination_bitmap->width, destination_bitmap->height, 0,
                                                  destination_bitmap->format);

    if ( source_face && source_face->base_address && destination_face && destination_face->base_address )
    {
        for ( int16_t face_index = 0; face_index < 6; face_index++ )
        {
            bitmap_cube_map_face_extract(source_bitmap, source_mipmap_index, face_index, source_face);
            bitmap_2d_uncompress_from_mipmap(source_face, destination_face, 0);
            bitmap_cube_map_face_insert(destination_face, destination_bitmap, 0, face_index);
        }
    }

    bitmap_delete(source_face);
    bitmap_delete(destination_face);
}
