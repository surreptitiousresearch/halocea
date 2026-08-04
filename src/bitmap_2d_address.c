/* bitmap_2d_address @0x836F4330 — compute the address of texel (x,y) at the given mipmap level of a 2D
 * bitmap, accounting for the pixel-offset of preceding mip levels (each level halves width/height down to a
 * minimum block size: 1 for linear formats, 4 for DXT/compressed (flags & 2)). */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format_tables.h"
#include "headers/bitmap_flags.h"

char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index)
{
    int16_t width  = bitmap->width;
    int16_t height = bitmap->height;
    int     pixel_offset = 0;

    if ( mipmap_index > 0 )
    {
        int min_block = (bitmap->flags & (1u << _bitmap_compressed_bit)) == 0 ? 1 : 4;
        int16_t level = 0;
        do
        {
            pixel_offset += width * height;

            int next_width = width >> 1;
            if ( min_block > next_width )
                next_width = (bitmap->flags & (1u << _bitmap_compressed_bit)) == 0 ? 1 : 4;
            int next_height = height >> 1;
            if ( min_block > next_height )
                next_height = (bitmap->flags & (1u << _bitmap_compressed_bit)) == 0 ? 1 : 4;

            width  = (int16_t)next_width;
            height = (int16_t)next_height;
            ++level;
        }
        while ( level < mipmap_index );
    }

    return (char *)bitmap->base_address
         + (width * y + x + pixel_offset) * bitmap_format_bits_per_pixel_table[bitmap->format] / 8;
}
