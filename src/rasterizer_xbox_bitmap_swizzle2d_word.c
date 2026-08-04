#include <stdint.h>
#include "headers/blam_data_globals.h"
/* rasterizer_xbox_bitmap_swizzle2d_word @0x837E33F0 — 16-bit-per-pixel variant of
 * rasterizer_xbox_bitmap_swizzle2d_byte: write a linear word-per-pixel `src` image into `dst` in swizzled
 * (Morton-order) layout via compute_swizzle_masks' x/y interleave masks and the "(value - mask) & mask"
 * masked-increment trick. Unlike the byte variant's source, the compiled code here does not re-zero the x
 * swizzle counter per row — it relies on it wrapping to 0 after `width` (a power of two) steps. */

extern unsigned int rasterizer_swizzle_x_mask;

extern void compute_swizzle_masks(int16_t width, int16_t height, int16_t depth);

void rasterizer_xbox_bitmap_swizzle2d_word(void *dst, const void *src, int16_t width, int16_t height)
{
    int src_index = 0;
    unsigned int x_swizzle = 0;
    unsigned int y_swizzle = 0;

    compute_swizzle_masks(width, height, 1);

    for ( __int16 row = 0; row < height; ++row )
    {
        for ( __int16 col = 0; col < width; ++col )
        {
            ((unsigned __int16 *)dst)[y_swizzle | x_swizzle] = ((const unsigned __int16 *)src)[src_index];
            ++src_index;
            x_swizzle = (x_swizzle - rasterizer_swizzle_x_mask) & rasterizer_swizzle_x_mask;
        }
        y_swizzle = (y_swizzle - ay) & ay;
    }
}
