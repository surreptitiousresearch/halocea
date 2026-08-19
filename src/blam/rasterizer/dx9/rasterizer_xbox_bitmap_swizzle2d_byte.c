#include <stdint.h>
#include "headers/blam_data_globals.h"
/* rasterizer_xbox_bitmap_swizzle2d_byte @0x837E3338 — write a linear byte-per-pixel `src` image into `dst`
 * in swizzled (Morton-order) layout: the destination offset's row/column bit-interleave masks come from
 * compute_swizzle_masks(width, height, 1) (x mask in rasterizer_swizzle_x_mask, y mask in `ay`), and each step advances
 * within its mask via the classic "(value - mask) & mask" bit-decrement trick instead of a table lookup. */

extern unsigned int rasterizer_swizzle_x_mask;

extern void compute_swizzle_masks(int16_t width, int16_t height, int16_t depth);

void rasterizer_xbox_bitmap_swizzle2d_byte(uint8_t *dst, const uint8_t *src, int16_t width, int16_t height)
{
    int src_index = 0;
    unsigned int x_swizzle = 0;
    unsigned int y_swizzle = 0;

    compute_swizzle_masks(width, height, 1);

    for ( int16_t row = 0; row < height; ++row )
    {
        x_swizzle = 0;

        for ( int16_t col = 0; col < width; ++col )
        {
            dst[y_swizzle | x_swizzle] = src[src_index];
            ++src_index;
            x_swizzle = (x_swizzle - rasterizer_swizzle_x_mask) & rasterizer_swizzle_x_mask;
        }

        y_swizzle = (y_swizzle - ay) & ay;
    }
}
