/* bitmap_cm_compress_to_mipmap @0x8377CCB8 — copy a cube map into one mipmap level of a (differently
 * formatted) destination cube map via two full-size 2D scratch bitmaps.
 *
 * Faithful shipped quirk (disasm-verified, bug class 12): each face is EXTRACTED into the source-format
 * scratch but the destination-format scratch — which nothing ever writes — is what gets INSERTED, and the
 * transparent_color parameter is never read. No conversion call exists between the two; reproduced
 * verbatim, not "fixed". */

#include <stdint.h>
#include "headers/bitmap_data.h"

extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern void bitmap_cube_map_face_extract(const bitmap_data *source_bitmap, int16_t source_mipmap_index, int16_t source_face_index, bitmap_data *face_bitmap);
extern void bitmap_cube_map_face_insert(const bitmap_data *face_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, int16_t destination_face_index);
extern void bitmap_delete(bitmap_data *bitmap);

void bitmap_cm_compress_to_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, const unsigned int *transparent_color)
{
    bitmap_data *source_face = bitmap_2d_new(source_bitmap->width, source_bitmap->height, 0,
                                             source_bitmap->format);
    bitmap_data *destination_face = bitmap_2d_new(source_bitmap->width, source_bitmap->height, 0,
                                                  destination_bitmap->format);

    if ( source_face && source_face->base_address && destination_face && destination_face->base_address )
    {
        for ( int16_t face_index = 0; face_index < 6; face_index++ )
        {
            bitmap_cube_map_face_extract(source_bitmap, 0, face_index, source_face);
            bitmap_cube_map_face_insert(destination_face, destination_bitmap, destination_mipmap_index,
                                        face_index);
        }
    }

    bitmap_delete(source_face);
    bitmap_delete(destination_face);
}
