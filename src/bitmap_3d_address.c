/* bitmap_3d_address @0x836F4410 — compute the byte address of voxel (x,y,z) within a given mipmap level
 * of a 3D bitmap: walks mipmap_index levels accumulating each level's pixel count (width*height*depth),
 * halving each dimension per level with a floor of 1 (or 4 for block-compressed formats, flags bit 0x2),
 * then converts the final (level-relative) voxel offset to bytes via the format's bits-per-pixel. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"

extern const int8_t bitmap_format_bits_per_pixel_table[];  /* lbzx+extsb stride-1 byte table (all 15 binary access sites) */

char *bitmap_3d_address(const bitmap_data *bitmap, __int16 x, __int16 y, __int16 z, __int16 mipmap_index)
{
    __int16 width = bitmap->width;
    __int16 height = bitmap->height;
    __int16 depth = bitmap->depth;
    int accumulated_pixels = 0;

    if ( mipmap_index > 0 )
    {
        __int16 minimum_dimension = (bitmap->flags & (1 << _bitmap_compressed_bit)) == 0 ? 1 : 4;

        for ( __int16 level = 0; level < mipmap_index; level++ )
        {
            accumulated_pixels += width * height * depth;

            width >>= 1;
            if ( width < minimum_dimension )
                width = minimum_dimension;

            height >>= 1;
            if ( height < minimum_dimension )
                height = minimum_dimension;

            depth >>= 1;
            if ( depth < 1 )
                depth = 1;
        }
    }

    int voxel_offset = (height * z + y) * width + x + accumulated_pixels;
    return (char *)bitmap->base_address + voxel_offset * bitmap_format_bits_per_pixel_table[bitmap->format] / 8;
}
