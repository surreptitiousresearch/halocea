/* bitmap_mipmap_get_row_pitch @0x836F4AC0 — byte pitch of one row of a mipmap level: the level's
 * width (>= 1), rounded up to a multiple of 4 for compressed (DXT) formats, times bits-per-pixel. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"
#include "headers/bitmap_format_tables.h"

int bitmap_mipmap_get_row_pitch(const bitmap_data *bitmap, int16_t mipmap_index)
{
    short width = bitmap->width >> mipmap_index;
    if ( width <= 1 )
        width = 1;
    if ( (bitmap->flags & (1u << _bitmap_compressed_bit)) != 0 )
        width = (-width & 3) + width;   /* round up to multiple of 4 */
    return bitmap_format_bits_per_pixel_table[bitmap->format] * width / 8;
}
