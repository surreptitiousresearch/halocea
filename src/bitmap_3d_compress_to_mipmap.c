/* bitmap_3d_compress_to_mipmap @0x8377CBD0 — compress a 3D bitmap's slices into one mipmap level of a
 * (differently-formatted) destination bitmap: allocate two matching-size scratch 2D bitmaps (one in the
 * source format, one in the destination format), and for each depth slice, extract it from the source into
 * the source-format scratch, then insert the destination-format scratch into the target mipmap/slice. Both
 * scratch bitmaps are freed unconditionally at the end.
 *
 * SHIPPED BUG (same class as bitmap_cm_compress_to_mipmap): the insert reads destination_scratch, which is
 * never written by any extract call — only source_scratch is populated per slice. destination_scratch's
 * (garbage, freshly-allocated) contents are what actually get inserted every iteration. transparent_color
 * is likewise unread. Reproduced verbatim, as compiled. */

#include <stdint.h>
#include "headers/bitmap_data.h"

extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern void bitmap_3d_slice_extract(const bitmap_data *source_bitmap, int16_t source_mipmap_index, int16_t source_slice_index, bitmap_data *slice_bitmap);
extern void bitmap_3d_slice_insert(const bitmap_data *slice_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, int16_t destination_slice_index);
extern void bitmap_delete(bitmap_data *bitmap);

void bitmap_3d_compress_to_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, const unsigned int *transparent_color)
{
    bitmap_data *source_scratch = bitmap_2d_new(source_bitmap->width, source_bitmap->height, 0,
                                       source_bitmap->format);
    bitmap_data *destination_scratch = bitmap_2d_new(source_bitmap->width, source_bitmap->height, 0,
                                            destination_bitmap->format);

    if ( source_scratch && source_scratch->base_address
      && destination_scratch && destination_scratch->base_address )
    {
        for ( int16_t slice_index = 0; slice_index < source_bitmap->depth; slice_index++ )
        {
            bitmap_3d_slice_extract(source_bitmap, 0, slice_index, source_scratch);
            bitmap_3d_slice_insert(destination_scratch, destination_bitmap, destination_mipmap_index, slice_index);
        }
    }

    bitmap_delete(source_scratch);
    bitmap_delete(destination_scratch);
}
