#include <stdint.h>
#include "headers/blam_data_globals.h"
/* rasterizer_xbox_bitmap_swizzle3d_word @0x837E3678 — write a linear word-per-voxel `src` volume into
 * `dst` in swizzled (Morton-order) layout. Word-granularity sibling of
 * rasterizer_xbox_bitmap_swizzle3d_byte.c: same x/y/z bit-interleave masks from compute_swizzle_masks,
 * but the destination offset is doubled (word stride) and the source is walked with an explicit running
 * pointer rather than an index. */

extern unsigned int rasterizer_swizzle_x_mask;

extern void compute_swizzle_masks(int16_t width, int16_t height, int16_t depth);

void rasterizer_xbox_bitmap_swizzle3d_word(char *dst, char *src, int16_t width, int16_t height, int16_t depth)
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
            char *source = &src[2 * src_index - 2];

            for ( __int16 col = 0; col < width; ++col )
            {
                source += 2;
                ++src_index;
                *(unsigned __int16 *)&dst[2 * (z_swizzle | y_swizzle | x_swizzle)] = *(unsigned __int16 *)source;
                x_swizzle = (x_swizzle - rasterizer_swizzle_x_mask) & rasterizer_swizzle_x_mask;
            }

            y_swizzle = (y_swizzle - ay) & ay;
        }

        z_swizzle = (z_swizzle - az) & az;
    }
}
