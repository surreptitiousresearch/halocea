/* bitmap_2d_height_map @0x8377C038 — convert a 32-bit height map bitmap into a tangent-space normal map,
 * in place (top mipmap only). For each pixel it samples the R channel (memory byte 1 of the big-endian
 * ARGB word) of the pixel and its four wrap-around neighbors, scaled by
 * bitmap->height * bump_height / 255; picks per axis the steeper "uphill" neighbor slope (zero if the
 * center is a local maximum on that axis, with the axis sign giving the slope direction); builds the
 * normal as the cross product of the two tangents (u = (sign_x, 0, slope_x), v = (0, sign_y, slope_y)),
 * flips it upward if z is negative, normalizes it (skipped when the length is under 1e-4), and packs it
 * biased to 0..255 as R = x, G = y, B = z, keeping the source pixel's alpha. Results are written into a
 * scratch buffer so later pixels still sample the original heights, then copied over the mipmap.
 * Sibling of bitmap_2d_smooth / bitmap_2d_shrink (same scratch + dlMalloc site pattern).
 *
 * DEVIATIONS (decompiler emitted "local variable allocation has failed"): the height channel is the R
 * byte — extrwi 8,8 = bits 16-23 of the word and the fifth sample is an explicit lbz of memory byte 1
 * (the decompiler's BYTE1() names refer to little-endian register bytes and read as G); the __int64
 * OVERLAPPED puns are plain unsigned-byte-to-float conversions; the height scale really uses
 * bitmap->height (not width), reproduced as shipped. */

#include <stdint.h>
#include <string.h>
#include "headers/bitmap_data.h"

extern int bitmap_get_pixel_data_size(const bitmap_data *bitmap);
extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern float __fsqrts(float x);
extern double __fabs(double x);

void bitmap_2d_height_map(bitmap_data *bitmap, float bump_height)
{
    unsigned int pixel_data_size = bitmap_get_pixel_data_size(bitmap);
    unsigned int *scratch = dlMalloc(pixel_data_size,
            "D:\\Projects\\code\\HCEX\\sources\\bitmaps\\bitmap_utilities.c", 0x4D4u);
    if ( !scratch )
        return;

    /* Height scale is derived from the bitmap HEIGHT (shipped code, reproduced). */
    float height_scale = ((float)bitmap->height * bump_height) * 0.0039215689f;

    for ( int y = 0; y < bitmap->height; y = (int16_t)(y + 1) )
    {
        for ( int x = 0; x < bitmap->width; x = (int16_t)(x + 1) )
        {
            unsigned int center_pixel = *(unsigned int *)bitmap_2d_address(bitmap, x, y, 0);

            int16_t left_x  = x ? (int16_t)(x - 1) : (int16_t)(bitmap->width - 1);
            unsigned int left_pixel = *(unsigned int *)bitmap_2d_address(bitmap, left_x, y, 0);

            int16_t right_x = (x == bitmap->width - 1) ? 0 : (int16_t)(x + 1);
            unsigned int right_pixel = *(unsigned int *)bitmap_2d_address(bitmap, right_x, y, 0);

            int16_t up_y = y ? (int16_t)(y - 1) : (int16_t)(bitmap->height - 1);
            unsigned int up_pixel = *(unsigned int *)bitmap_2d_address(bitmap, x, up_y, 0);

            int16_t down_y = (y == bitmap->height - 1) ? 0 : (int16_t)(y + 1);

            /* R channel of each sample (memory byte 1 of the big-endian ARGB word). */
            float center = (float)((center_pixel >> 16) & 0xFF) * height_scale;
            float left   = (float)((left_pixel   >> 16) & 0xFF) * height_scale;
            float right  = (float)((right_pixel  >> 16) & 0xFF) * height_scale;
            float up     = (float)((up_pixel     >> 16) & 0xFF) * height_scale;
            float down   = (float)*((uint8_t *)bitmap_2d_address(bitmap, x, down_y, 0) + 1)
                         * height_scale;

            float slope_x, sign_x;
            if ( center > left && center > right )
            {
                slope_x = 0.0f;
                sign_x = 1.0f;
            }
            else if ( left > right )
            {
                sign_x = -1.0f;
                slope_x = left - center;
            }
            else
            {
                slope_x = right - center;
                sign_x = 1.0f;
            }

            float slope_y, sign_y;
            if ( center > up && center > down )
            {
                slope_y = 0.0f;
                sign_y = 1.0f;
            }
            else if ( up > down )
            {
                sign_y = -1.0f;
                slope_y = up - center;
            }
            else
            {
                slope_y = down - center;
                sign_y = 1.0f;
            }

            /* normal = (sign_x, 0, slope_x) x (0, sign_y, slope_y), z-terms kept verbatim for
             * bit-exactness (fmsubs against 0.0). */
            float normal_z = sign_y * sign_x;
            float normal_x = slope_y * 0.0f - (sign_y * slope_x);
            float normal_y = slope_x * 0.0f - (slope_y * sign_x);

            if ( normal_z < 0.0f )
            {
                normal_x = -normal_x;
                normal_y = -normal_y;
                normal_z = -normal_z;
            }

            float length = __fsqrts((normal_z * normal_z)
                    + ((normal_y * normal_y) + (normal_x * normal_x)));
            if ( __fabs(length) >= 0.000099999997f )
            {
                float inverse_length = 1.0f / length;
                normal_x = inverse_length * normal_x;
                normal_y = inverse_length * normal_y;
                normal_z = inverse_length * normal_z;
            }

            scratch[bitmap->width * y + x] =
                    (((unsigned int)(int)((normal_x + 1.0f) * 127.5f) << 16)
                    | ((unsigned int)(int)((normal_y + 1.0f) * 127.5f) << 8)
                    | (unsigned int)(int)((normal_z + 1.0f) * 127.5f))
                    | (center_pixel & 0xFF000000);
        }
    }

    memcpy(bitmap_mipmap_address(bitmap, 0), scratch, pixel_data_size);
    dlFree(scratch);
}
