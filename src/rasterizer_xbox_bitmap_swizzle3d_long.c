#include <stdint.h>
#include "headers/blam_data_globals.h"
/* rasterizer_xbox_bitmap_swizzle3d_long @0x837E3778 — write a linear dword-per-voxel `src` volume into
 * `dst` in swizzled (Morton-order) layout. Dword-granularity sibling of
 * rasterizer_xbox_bitmap_swizzle3d_word.c / _byte.c: same x/y/z bit-interleave masks from
 * compute_swizzle_masks, destination offset scaled by 4 (dword stride). */

extern unsigned int rasterizer_swizzle_x_mask;

extern void compute_swizzle_masks(int16_t width, int16_t height, int16_t depth);

void rasterizer_xbox_bitmap_swizzle3d_long(char *dst, char *src, int16_t width, int16_t height, int16_t depth)
{
    int src_index = 0;
    unsigned int x_swizzle = 0;
    unsigned int y_swizzle = 0;
    unsigned int z_swizzle = 0;

    compute_swizzle_masks(width, height, depth);

    for ( __int16 slice = 0; slice < depth; ++slice )
    {
        y_swizzle = 0;

        for ( __int16 row = 0; row < height; ++row )
        {
            x_swizzle = 0;
            char *source = &src[4 * src_index - 4];

            for ( __int16 col = 0; col < width; ++col )
            {
                source += 4;
                ++src_index;
                *(unsigned int *)&dst[4 * (z_swizzle | y_swizzle | x_swizzle)] = *(unsigned int *)source;
                x_swizzle = (x_swizzle - rasterizer_swizzle_x_mask) & rasterizer_swizzle_x_mask;
            }

            y_swizzle = (y_swizzle - ay) & ay;
        }

        z_swizzle = (z_swizzle - az) & az;
    }
}
