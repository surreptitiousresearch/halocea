/* bitmap_height_map @0x8377DEE8 — dispatch a "convert to height/bump map" operation by bitmap type:
 * 2D bitmaps go straight to bitmap_2d_height_map(); cube maps (type 2, plus anything >= 3) are rejected
 * with a warning; 3D bitmaps are processed one depth slice at a time via a scratch 2D bitmap
 * (extract/height-map/insert per slice), then the scratch bitmap is deleted. Also warns (and does
 * nothing else) if bump_height is non-positive. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"

#include <stdio.h>
typedef struct _iobuf _iobuf;

extern _iobuf *__iob_func(void);
extern int fprintf(_iobuf *stream, const char *format, ...);
extern int fflush(_iobuf *stream);

extern void bitmap_2d_height_map(bitmap_data *bitmap, float bump_height);
extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern void bitmap_3d_slice_extract(const bitmap_data *source_bitmap, int16_t source_mipmap_index, int16_t source_slice_index, bitmap_data *slice_bitmap);
extern void bitmap_3d_slice_insert(const bitmap_data *slice_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, int16_t destination_slice_index);
extern void bitmap_delete(bitmap_data *bitmap);

void bitmap_height_map(bitmap_data *bitmap, float bump_height)
{
    if ( bump_height <= 0.0f )
    {
        fprintf(__iob_func() + 1, "### WARNING importing special-effect bump map with zero-height\r\n");
        fflush(__iob_func() + 1);
        return;
    }

    unsigned __int16 type = (unsigned __int16)bitmap->type;

    if ( bitmap->type == bitmap_type_2d )
    {
        bitmap_2d_height_map(bitmap, bump_height);
        return;
    }

    if ( type != bitmap_type_3d )
    {
        if ( type < bitmap_type_white )
        {
            fprintf(__iob_func() + 1, "### WARNING tried to use a cube map as a height map\r\n");
            fflush(__iob_func() + 1);
        }
        return;
    }

    bitmap_data *slice = bitmap_2d_new(bitmap->width, bitmap->height, 0, bitmap->format);
    if ( slice && slice->base_address )
    {
        for ( __int16 depth = 0; depth < bitmap->depth; ++depth )
        {
            bitmap_3d_slice_extract(bitmap, 0, depth, slice);
            bitmap_2d_height_map(slice, bump_height);
            bitmap_3d_slice_insert(slice, bitmap, 0, depth);
        }
    }

    bitmap_delete(slice);
}
