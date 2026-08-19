/* rasterizer_xbox_bitmap_get_max_mipmap_count @0x837E3B18 — clamp a bitmap's stored mipmap_count to the
 * maximum meaningful for its dimensions: min(mipmap_count, floor_log2(largest_dimension)). Only bitmaps
 * that carry mipmaps (flag 1) and aren't the "external/linear" kind (flag 0x10) get a nonzero count;
 * volume textures (flag 2) count width/height in 4-texel blocks while depth is counted directly.
 *
 * DEVIATION: the decompiler expanded this into two floor_log2 early-returns plus a shared tail that
 * recomputes the exact same largest-dimension max a second (and third) time — an artifact of the single
 * source-level `min(mipmap_count, floor_log2(dim))` having two return paths. Verified across both branches
 * (3D at 0x837E3B68, 2D at 0x837E3C08) that every recomputation yields the same max; collapsed to one. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"

extern int16_t floor_log2(unsigned int k);

int16_t rasterizer_xbox_bitmap_get_max_mipmap_count(const bitmap_data *bitmap)
{
    uint16_t flags = bitmap->flags;

    if ( (flags & 1) == 0 || (flags & (1u << _bitmap_linear_bit)) != 0 )
        return 0;

    int width  = bitmap->width;
    int height = bitmap->height;
    int depth  = bitmap->depth;

    if ( (flags & (1u << _bitmap_compressed_bit)) != 0 )
    {
        width  /= 4;
        height /= 4;
    }

    int largest = height;
    if ( largest < depth )
        largest = depth;
    if ( largest < width )
        largest = width;

    int16_t max_mipmap_count = floor_log2(largest);
    if ( bitmap->mipmap_count <= max_mipmap_count )
        return bitmap->mipmap_count;
    return max_mipmap_count;
}
