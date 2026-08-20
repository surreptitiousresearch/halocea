/* bitmap_3d_smooth @0x8377B098 — in-place separable smoothing convolution of a 32-bit (format-11) 3D (volume)
 * bitmap by a 1D filter of half-width `filter_size` (taps at [-filter_size, +filter_size], coefficients in
 * filter_coefficients). The 3D analogue of bitmap_2d_smooth: it runs three passes over the top mipmap —
 * horizontal (x), vertical (y), then depth (z) — each wrapping toroidally on its own axis. The x pass reads the
 * live voxels into a scratch copy, the y pass reads scratch back into the live voxels, the z pass reads the live
 * voxels back into scratch, and a final memcpy copies scratch into the live voxels. Each of the four channels is
 * accumulated independently and normalized by a right shift of 2*filter_size with round-to-nearest (adding
 * 1<<(2*filter_size-1) before the shift) — the coefficient sum is assumed to be 2^(2*filter_size). A filter
 * larger than the bitmap in any dimension is rejected with a stderr warning.
 *
 * DEVIATION: the previous reconstruction transposed red and green (same BYTE1/BYTE2 big-endian misreading as
 * bitmap_2d_smooth). extrwi r30, 8,8 @0x8377B1E8 is (texel>>16)&0xFF and extrwi r15, 8,16 @0x8377B1EC is
 * (texel>>8)&0xFF; the pack at 0x8377B21C-0x8377B268 returns each to its own bit position. The scratch buffer
 * holds the full pixel-data size; all passes index it as the flattened voxel address ((height*z + y)*width + x). */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include <stdio.h>
#include <string.h>

extern int bitmap_get_pixel_data_size(const bitmap_data *bitmap);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
typedef struct _iobuf _iobuf;
extern _iobuf *__iob_func(void);
/* fprintf provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
/* fflush provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

void bitmap_3d_smooth(bitmap_data *bitmap, int16_t filter_size, const int16_t *filter_coefficients)
{
    if ( bitmap->width < filter_size || bitmap->height < filter_size || bitmap->depth < filter_size )
    {
        fprintf(__iob_func() + 1, "### WARNING tried to smooth a bitmap with a filter which is too large");
        fflush(__iob_func() + 1);
        return;
    }

    unsigned int pixel_data_size = bitmap_get_pixel_data_size(bitmap);
    unsigned int *pixels = bitmap_mipmap_address(bitmap, 0);
    unsigned int *scratch = dlMalloc(pixel_data_size,
            "D:\\Projects\\code\\HCEX\\sources\\bitmaps\\bitmap_utilities.c", 0x2C7u);
    if ( !scratch )
        return;

    int round_add = 1 << (2 * filter_size - 1);
    int shift = 2 * filter_size;

    /* horizontal pass: pixels -> scratch, wrapping in x */
    for ( int z = 0; z < bitmap->depth; z = (int16_t)(z + 1) )
    {
        for ( int y = 0; y < bitmap->height; y = (int16_t)(y + 1) )
        {
            int width = bitmap->width;
            for ( int x = 0; x < width; x = (int16_t)(x + 1) )
            {
                int sum_alpha = 0, sum_red = 0, sum_green = 0, sum_blue = 0;
                for ( int tap = -filter_size; tap <= filter_size; tap = (int16_t)(tap + 1) )
                {
                    int source_width = bitmap->width;
                    int wrapped = (int16_t)((source_width + tap + x) % source_width);
                    int coefficient = filter_coefficients[tap + filter_size];
                    unsigned int texel = pixels[wrapped + (bitmap->height * z + y) * source_width];
                    sum_alpha += (texel >> 24) * coefficient;
                    sum_red += ((texel >> 16) & 0xFF) * coefficient;
                    sum_green += ((texel >> 8) & 0xFF) * coefficient;
                    sum_blue += (texel & 0xFF) * coefficient;
                }
                width = bitmap->width;
                scratch[(bitmap->height * z + y) * width + x] =
                        ((((((round_add + sum_alpha) >> shift << 8) | ((round_add + sum_red) >> shift)) << 8)
                                | ((round_add + sum_green) >> shift)) << 8)
                        | ((round_add + sum_blue) >> shift);
            }
        }
    }

    /* vertical pass: scratch -> pixels, wrapping in y */
    for ( int z = 0; z < bitmap->depth; z = (int16_t)(z + 1) )
    {
        for ( int y = 0; y < bitmap->height; y = (int16_t)(y + 1) )
        {
            int width = bitmap->width;
            for ( int x = 0; x < width; x = (int16_t)(x + 1) )
            {
                int sum_alpha = 0, sum_red = 0, sum_green = 0, sum_blue = 0;
                for ( int tap = -filter_size; tap <= filter_size; tap = (int16_t)(tap + 1) )
                {
                    int source_height = bitmap->height;
                    int wrapped = (int16_t)((source_height + tap + y) % source_height);
                    int coefficient = filter_coefficients[tap + filter_size];
                    unsigned int texel = scratch[(wrapped + source_height * z) * width + x];
                    sum_alpha += (texel >> 24) * coefficient;
                    sum_red += ((texel >> 16) & 0xFF) * coefficient;
                    sum_green += ((texel >> 8) & 0xFF) * coefficient;
                    sum_blue += (texel & 0xFF) * coefficient;
                }
                width = bitmap->width;
                pixels[(bitmap->height * z + y) * width + x] =
                        ((((((round_add + sum_alpha) >> shift << 8) | ((round_add + sum_red) >> shift)) << 8)
                                | ((round_add + sum_green) >> shift)) << 8)
                        | ((round_add + sum_blue) >> shift);
            }
        }
    }

    /* depth pass: pixels -> scratch, wrapping in z */
    for ( int z = 0; z < bitmap->depth; z = (int16_t)(z + 1) )
    {
        for ( int y = 0; y < bitmap->height; y = (int16_t)(y + 1) )
        {
            int width = bitmap->width;
            for ( int x = 0; x < width; x = (int16_t)(x + 1) )
            {
                int sum_alpha = 0, sum_red = 0, sum_green = 0, sum_blue = 0;
                for ( int tap = -filter_size; tap <= filter_size; tap = (int16_t)(tap + 1) )
                {
                    int source_depth = bitmap->depth;
                    int wrapped = (int16_t)((source_depth + tap + z) % source_depth);
                    int coefficient = filter_coefficients[tap + filter_size];
                    unsigned int texel = pixels[(wrapped * bitmap->height + y) * width + x];
                    sum_alpha += (texel >> 24) * coefficient;
                    sum_red += ((texel >> 16) & 0xFF) * coefficient;
                    sum_green += ((texel >> 8) & 0xFF) * coefficient;
                    sum_blue += (texel & 0xFF) * coefficient;
                }
                width = bitmap->width;
                scratch[(bitmap->height * z + y) * width + x] =
                        ((((((round_add + sum_alpha) >> shift << 8) | ((round_add + sum_red) >> shift)) << 8)
                                | ((round_add + sum_green) >> shift)) << 8)
                        | ((round_add + sum_blue) >> shift);
            }
        }
    }

    memcpy(pixels, scratch, pixel_data_size);
    dlFree(scratch);
}
