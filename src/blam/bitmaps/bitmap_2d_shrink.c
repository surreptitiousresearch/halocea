/* bitmap_2d_shrink @0x8377A5E8 — box-filter downscale of a 32-bit bitmap. The sample block is
 * min(scale, source_dim) in each axis, and the output dimensions are source_dim / sample_dim. A new
 * format-11 bitmap is allocated; each output texel is the rounded average of its source sample block. When
 * ignore_zero_alpha is set, fully-transparent source texels (alpha byte == 0) are excluded from the average;
 * an output texel whose block contributed no samples is written as 0. The averaged alpha has alpha_bias added
 * and is clamped to [0,255]. Returns the new bitmap (which may be non-null but empty if allocation of its
 * pixel store failed).
 *
 * Rounded division adds half the sample count before dividing.
 *
 * DEVIATION: the previous reconstruction transposed red and green, on the strength of Hex-Rays' BYTE1/BYTE2
 * names (little-endian register bytes, transposed on this big-endian target). There is no transposition:
 * extrwi r9, 8,8 @0x8377A730 is (texel>>16)&0xFF (red) and extrwi r8, 8,16 @0x8377A734 is (texel>>8)&0xFF
 * (green), and the pack at 0x8377A7B8-0x8377A7E4 returns each to its own bit position. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format.h"

extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);

bitmap_data * bitmap_2d_shrink(const bitmap_data *source_bitmap, int16_t scale, int16_t alpha_bias, uint8_t ignore_zero_alpha)
{
    int16_t source_width = source_bitmap->width;
    int16_t sample_width = scale <= source_width ? scale : source_width;
    int16_t source_height = source_bitmap->height;
    int16_t sample_height = scale > source_height ? source_height : scale;

    int out_width = source_width / sample_width;
    int out_height = source_height / sample_height;

    bitmap_data *dest = bitmap_2d_new(out_width, out_height, 0, _bitmap_format_a8r8g8b8);
    if ( dest && dest->base_address )
    {
        for ( int16_t y_out = 0; y_out < out_height; ++y_out )
        {
            for ( int16_t x_out = 0; x_out < out_width; ++x_out )
            {
                int sum_alpha = 0, sum_red = 0, sum_green = 0, sum_blue = 0, count = 0;
                int *dest_pixel = (int *)bitmap_2d_address(dest, x_out, y_out, 0);

                for ( int16_t sy = 0; sy < sample_height; ++sy )
                {
                    for ( int16_t sx = 0; sx < sample_width; ++sx )
                    {
                        unsigned int texel = *(unsigned int *)bitmap_2d_address(source_bitmap,
                                x_out * sample_width + sx, y_out * sample_height + sy, 0);
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

                if ( count )
                {
                    int alpha = (count / 2 + sum_alpha) / count + alpha_bias;
                    if ( alpha < 0 )
                        alpha = 0;
                    else if ( alpha > 255 )
                        alpha = 255;
                    *dest_pixel = (alpha << 24)
                        | (((count / 2 + sum_red) / count) << 16)
                        | (((count / 2 + sum_green) / count) << 8)
                        | ((count / 2 + sum_blue) / count);
                }
                else
                {
                    *dest_pixel = 0;
                }
            }
        }
    }
    return dest;
}
