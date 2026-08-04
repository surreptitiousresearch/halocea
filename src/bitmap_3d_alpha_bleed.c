/* bitmap_3d_alpha_bleed @0x8377BD80 — bleed opaque color outward into the fully-transparent texels of a 3D
 * (volume) bitmap so bilinear filtering and mipmap generation don't drag the background color into visible
 * edges (source: bitmap_utilities.c). For each depth slice it extracts the slice to a scratch 2D bitmap and runs
 * `passes` bleed passes: each pass copies every texel into a work buffer unchanged unless the texel is fully
 * transparent (alpha == 0), in which case it scans the 3x3 neighborhood (in raster order, nearest row first) for
 * the first non-zero texel and adopts that neighbor's RGB (alpha left at 0). The work buffer is written back to
 * the slice after each pass, so successive passes propagate color one ring further; the slice is then inserted
 * back into the volume. The scratch bitmap is freed at the end.
 *
 * The neighbor search reproduces the decompiler's found-flag early-out: both the dy and dx loops stop the moment
 * a source is found (matching the `while (!found)` / `if (offset > 1) break` structure). The x/y neighbor
 * coordinates are computed as __int16 as in the binary. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include <string.h>

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);

extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern void bitmap_3d_slice_extract(const bitmap_data *source_bitmap, int16_t source_mipmap_index, int16_t source_slice_index, bitmap_data *slice_bitmap);
extern int bitmap_get_pixel_data_size(const bitmap_data *bitmap);
extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);
extern void bitmap_3d_slice_insert(const bitmap_data *slice_bitmap, bitmap_data *destination_bitmap, int16_t destination_mipmap_index, int16_t destination_slice_index);
extern void bitmap_delete(bitmap_data *bitmap);

void bitmap_3d_alpha_bleed(bitmap_data *bitmap, int16_t passes)
{
    bitmap_data *slice = bitmap_2d_new(bitmap->width, bitmap->height, 0, bitmap->format);
    if ( slice && slice->base_address )
    {
        for ( int z = 0; z < bitmap->depth; ++z )
        {
            bitmap_3d_slice_extract(bitmap, 0, z, slice);
            unsigned int pixel_data_size = bitmap_get_pixel_data_size(slice);
            char *buffer = dlMalloc(pixel_data_size,
                    "D:\\Projects\\code\\HCEX\\sources\\bitmaps\\bitmap_utilities.c", 0x422);
            if ( buffer )
            {
                for ( int pass = 0; pass < passes; ++pass )
                {
                    for ( int y = 0; y < slice->height; ++y )
                    {
                        unsigned int *source_row = (unsigned int *)bitmap_2d_address(slice, 0, y, 0);
                        int width = slice->width;
                        char *dest_row = &buffer[4 * width * y];
                        for ( int x = 0; x < width; ++x )
                        {
                            unsigned int texel = source_row[x];
                            if ( (texel & 0xFF000000) == 0 )
                            {
                                unsigned __int8 found = 0;
                                for ( int dy = -1; dy <= 1 && !found; ++dy )
                                {
                                    for ( int dx = -1; dx <= 1 && !found; ++dx )
                                    {
                                        __int16 nx = (__int16)(dx + x);
                                        __int16 ny = (__int16)(dy + y);
                                        if ( nx >= 0 && ny >= 0 && nx < slice->width && ny < slice->height )
                                        {
                                            unsigned int neighbor =
                                                    *(unsigned int *)bitmap_2d_address(slice, nx, ny, 0);
                                            if ( neighbor )
                                            {
                                                texel = neighbor & 0xFFFFFF;
                                                found = 1;
                                            }
                                        }
                                    }
                                }
                            }
                            *(unsigned int *)&dest_row[4 * x] = texel;
                        }
                    }
                    memcpy(bitmap_mipmap_address(slice, 0), buffer, pixel_data_size);
                }
                dlFree(buffer);
            }
            bitmap_3d_slice_insert(slice, bitmap, 0, z);
        }
    }
    bitmap_delete(slice);
}
