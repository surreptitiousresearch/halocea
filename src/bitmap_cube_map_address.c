/* bitmap_cube_map_address @0x836F4518 — address of one texel of a (square, six-faced) cube map: skip the
 * pixels of every earlier mipmap level (6 faces of width² each, halving per level with a floor of 4 for
 * block-compressed formats — flags bit 1 — else 1), then index face-major/row-major within the level,
 * scaled by the format's bits per pixel. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"

extern const int8_t bitmap_format_bits_per_pixel_table[];  /* lbzx+extsb stride-1 byte table (all 15 binary access sites) */

void * bitmap_cube_map_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t face_index, int16_t mipmap_index)
{
    __int16 width = bitmap->width;
    int pixel_offset = 0;

    for ( __int16 level = 0; level < mipmap_index; level++ )
    {
        pixel_offset += 6 * width * width;
        __int16 minimum = (bitmap->flags & (1u << _bitmap_compressed_bit)) == 0 ? 1 : 4;
        __int16 halved = width >> 1;
        if ( minimum > halved )
            halved = minimum;
        width = halved;
    }

    return (char *)bitmap->base_address
         + ((face_index * width + y) * width + x + pixel_offset)
         * bitmap_format_bits_per_pixel_table[bitmap->format] / 8;
}
