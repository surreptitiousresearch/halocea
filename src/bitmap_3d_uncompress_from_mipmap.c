/* bitmap_3d_uncompress_from_mipmap @0x8377CEF8 — uncompress one mipmap level of a 3D bitmap into a
 * (full-size) destination 3D bitmap: per depth slice, extract the source slice into a source-format
 * scratch, uncompress it into a destination-format scratch, and insert that into the destination's level 0.
 * Slice-based sibling of bitmap_cm_uncompress_from_mipmap.c (cube-map faces); unlike
 * bitmap_3d_compress_to_mipmap.c's shipped bug, this direction correctly threads the decompressed
 * scratch through to the insert. */

#include <stdint.h>
#include "headers/bitmap_data.h"

extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern void bitmap_3d_slice_extract(const bitmap_data *source_bitmap, int16_t source_mipmap_index, int16_t source_slice_index, bitmap_data *slice_bitmap);
extern void bitmap_2d_uncompress_from_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t source_mipmap_index);
extern void bitmap_3d_slice_insert(const bitmap_data *slice_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, int16_t destination_slice_index);
extern void bitmap_delete(bitmap_data *bitmap);

void bitmap_3d_uncompress_from_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t source_mipmap_index)
{
    bitmap_data *source_slice = bitmap_2d_new(destination_bitmap->width, destination_bitmap->height, 0,
                                              source_bitmap->format);
    bitmap_data *destination_slice = bitmap_2d_new(destination_bitmap->width, destination_bitmap->height, 0,
                                                   destination_bitmap->format);

    if ( source_slice && source_slice->base_address && destination_slice && destination_slice->base_address )
    {
        for ( __int16 slice_index = 0; slice_index < source_bitmap->depth; slice_index++ )
        {
            bitmap_3d_slice_extract(source_bitmap, source_mipmap_index, slice_index, source_slice);
            bitmap_2d_uncompress_from_mipmap(source_slice, destination_slice, 0);
            bitmap_3d_slice_insert(destination_slice, destination_bitmap, 0, slice_index);
        }
    }

    bitmap_delete(source_slice);
    bitmap_delete(destination_slice);
}
