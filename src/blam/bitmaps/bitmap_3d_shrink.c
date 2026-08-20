/* bitmap_3d_shrink @0x8377A830 — box-filter downscale of a 32-bit 3D (volume) bitmap; the 3D analogue of
 * bitmap_2d_shrink. The sample block is min(scale, source_dim) in each of x/y/z, and the output dimensions are
 * source_dim / sample_dim per axis. A new format-11 3D bitmap is allocated; each output voxel is the rounded
 * average of its source sample block. When ignore_zero_alpha is set, fully-transparent source voxels (alpha byte
 * == 0) are excluded from the average; an output voxel whose block contributed no samples is written as 0. The
 * averaged alpha has alpha_bias added and is clamped to [0,255]. Returns the new bitmap (possibly non-null but
 * empty if its pixel store could not be allocated).
 *
 * Rounded division adds half the sample count.
 *
 * DEVIATION: the previous reconstruction transposed red and green (same BYTE1/BYTE2 big-endian misreading as
 * bitmap_2d_shrink). extrwi r9, 8,8 @0x8377A9D4 is (texel>>16)&0xFF (red) and extrwi r8, 8,16 @0x8377A9D8 is
 * (texel>>8)&0xFF (green); the pack at 0x8377AA4C-0x8377AAA0 returns each to its own bit position. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format.h"

extern bitmap_data * bitmap_3d_new(int16_t width, int16_t height, int16_t depth, int16_t mipmap_count, int16_t format);
extern char *bitmap_3d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t z, int16_t mipmap_index);

bitmap_data * bitmap_3d_shrink(const bitmap_data *source_bitmap, int16_t scale, int16_t alpha_bias, uint8_t ignore_zero_alpha)
{
    int16_t source_width = source_bitmap->width;
    int16_t sample_width = scale <= source_width ? scale : source_width;
    int16_t source_height = source_bitmap->height;
    int16_t sample_height = scale <= source_height ? scale : source_height;
    int16_t source_depth = source_bitmap->depth;
    int16_t sample_depth = scale > source_depth ? source_depth : scale;

    int out_width = source_width / sample_width;
    int out_height = source_height / sample_height;
    int out_depth = source_depth / sample_depth;

    bitmap_data *dest = bitmap_3d_new(out_width, out_height, out_depth, 0, _bitmap_format_a8r8g8b8);
    if ( dest && dest->base_address )
    {
        for ( int16_t z_out = 0; z_out < out_depth; ++z_out )
        {
            for ( int16_t y_out = 0; y_out < out_height; ++y_out )
            {
                for ( int16_t x_out = 0; x_out < out_width; ++x_out )
                {
                    int sum_alpha = 0, sum_red = 0, sum_green = 0, sum_blue = 0, count = 0;
                    /* bitmap_3d_address returns char*; the destination voxel is a packed 32-bit
                     * ARGB word (stw @0x8377AAA0), matching the (unsigned int *) read below */
                    unsigned int *dest_voxel =
                        (unsigned int *)bitmap_3d_address(dest, x_out, y_out, z_out, 0);

                    for ( int16_t sz = 0; sz < sample_depth; ++sz )
                    {
                        for ( int16_t sy = 0; sy < sample_height; ++sy )
                        {
                            for ( int16_t sx = 0; sx < sample_width; ++sx )
                            {
                                unsigned int texel = *(unsigned int *)bitmap_3d_address(source_bitmap,
                                        x_out * sample_width + sx, y_out * sample_height + sy,
                                        z_out * sample_depth + sz, 0);
                                if ( (texel >> 24) != 0 || !ignore_zero_alpha )
                                {
                                    sum_alpha += texel >> 24;
                                    sum_red += (texel >> 16) & 0xFF;
                                    sum_green += (texel >> 8) & 0xFF;
                                    sum_blue += texel & 0xFF;
                                    ++count;
                                }
                            }
                        }
                    }

                    if ( count )
                    {
                        int alpha = (count / 2 + sum_alpha) / count + alpha_bias;
                        if ( alpha < 0 )
                            alpha = 0;
                        else if ( alpha > 255 )
                            alpha = 255;
                        *dest_voxel = (alpha << 24)
                            | (((count / 2 + sum_red) / count) << 16)
                            | (((count / 2 + sum_green) / count) << 8)
                            | ((count / 2 + sum_blue) / count);
                    }
                    else
                    {
                        *dest_voxel = 0;
                    }
                }
            }
        }
    }
    return dest;
}
