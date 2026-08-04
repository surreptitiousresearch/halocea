/* bitmap_2d_vector_map @0x8377C4E0 — renormalize a 2D normal-map bitmap in place. For every texel it decodes
 * the low three channels from [0,255] into signed [-1,1] components, normalizes the resulting vector (leaving
 * near-zero vectors untouched), re-encodes to [0,255], and preserves the original alpha channel. The result is
 * built in a scratch buffer and copied back over the bitmap's mip-0 pixels.
 *
 * DEVIATION: the decompiler's __int64 v9/v10 register puns (LODWORD/HIDWORD/BYTE1/BYTE2) are plain per-channel
 * byte extraction — the three source channels are (pixel & 0xFF), (pixel >> 8) & 0xFF, (pixel >> 16) & 0xFF.
 * 0.0078431377 = 2/255 (encode [0,255] -> [-1,1]); 127.5/0.5 do the inverse round; the 0.0001 guard skips
 * degenerate vectors. Note the shipped channel arrangement: source channels 0/1/2 map to output bytes
 * 0/2/1 respectively (reproduced verbatim). Hex-Rays flagged "local variable allocation has failed". */

#include <stdint.h>
#include "headers/bitmap_data.h"

extern int bitmap_get_pixel_data_size(const bitmap_data *bitmap);
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);
extern void *memcpy(void *dst, const void *src, unsigned int size);
extern double __fabs(double x);
extern float __fsqrts(float x);

void bitmap_2d_vector_map(bitmap_data *bitmap)
{
    unsigned int pixel_data_size = bitmap_get_pixel_data_size(bitmap);
    char *converted = dlMalloc(pixel_data_size,
            "D:\\Projects\\code\\HCEX\\sources\\bitmaps\\bitmap_utilities.c", 0x587u);
    if ( !converted )
        return;

    for ( int y = 0; y < bitmap->height; y = (__int16)(y + 1) )
    {
        for ( int x = 0; x < bitmap->width; x = (__int16)(x + 1) )
        {
            int pixel = *(int *)bitmap_2d_address(bitmap, x, y, 0);
            int channel0 = pixel & 0xFF;
            int channel1 = (pixel >> 8) & 0xFF;
            int channel2 = (pixel >> 16) & 0xFF;

            float n0 = (float)channel0 * 0.0078431377f - 1.0f;
            float n1 = (float)channel1 * 0.0078431377f - 1.0f;
            float n2 = (float)channel2 * 0.0078431377f - 1.0f;
            float magnitude = __fsqrts(n1 * n1 + (n0 * n0 + n2 * n2));
            if ( __fabs(magnitude) >= 0.000099999997f )
            {
                float inverse = 1.0f / magnitude;
                n1 = inverse * ((float)channel1 * 0.0078431377f - 1.0f);
                n2 = inverse * ((float)channel2 * 0.0078431377f - 1.0f);
                n0 = inverse * ((float)channel0 * 0.0078431377f - 1.0f);
            }

            int out0 = (int)((n0 + 1.0f) * 127.5f + 0.5f);
            int out1 = (int)((n2 + 1.0f) * 127.5f + 0.5f);
            int out2 = (int)((n1 + 1.0f) * 127.5f + 0.5f);
            *(int *)&converted[4 * (bitmap->width * y + x)] =
                (out2 << 16) | (out1 << 8) | out0 | (pixel & 0xFF000000);
        }
    }

    void *destination = bitmap_mipmap_address(bitmap, 0);
    memcpy(destination, converted, pixel_data_size);
    dlFree(converted);
}
