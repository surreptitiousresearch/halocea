/* bitmap_3d_height_map @0x8377C3E0 — convert every depth slice of a 3D bitmap into a height/bump map:
 * allocate a scratch 2D bitmap of matching dimensions and format, then for each slice extract it, run the
 * 2D height-map conversion, and insert it back. The scratch bitmap is freed unconditionally at the end
 * (bitmap_delete tolerates the NULL/failed-allocation case, matching the compiled control flow). */

#include <stdint.h>
#include "headers/bitmap_data.h"

extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern void bitmap_3d_slice_extract(const bitmap_data *source_bitmap, int16_t source_mipmap_index, int16_t source_slice_index, bitmap_data *slice_bitmap);
extern void bitmap_2d_height_map(bitmap_data *bitmap, float bump_height);
extern void bitmap_3d_slice_insert(const bitmap_data *slice_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, int16_t destination_slice_index);
extern void bitmap_delete(bitmap_data *bitmap);

void bitmap_3d_height_map(bitmap_data *bitmap, float bump_height)
{
    bitmap_data *slice = bitmap_2d_new(bitmap->width, bitmap->height, 0, bitmap->format);

    if ( slice && slice->base_address )
    {
        for ( int16_t slice_index = 0; slice_index < bitmap->depth; slice_index++ )
        {
            bitmap_3d_slice_extract(bitmap, 0, slice_index, slice);
            bitmap_2d_height_map(slice, bump_height);
            bitmap_3d_slice_insert(slice, bitmap, 0, slice_index);
        }
    }

    bitmap_delete(slice);
}
