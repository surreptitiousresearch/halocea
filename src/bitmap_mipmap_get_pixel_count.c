/* bitmap_mipmap_get_pixel_count @0x836F49A0 — pixel count of one mipmap level: width/height/depth shifted
 * down by the level (floored at 1), width and height rounded up to multiples of 4 for block-compressed
 * formats (flags bit 1), and multiplied by 6 for cube maps (type 2). */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"
#include "headers/bitmap_type.h"

int bitmap_mipmap_get_pixel_count(const bitmap_data *bitmap, int16_t mipmap_index)
{
    __int16 width = bitmap->width >> mipmap_index;
    if ( width <= 1 )
        width = 1;
    __int16 height = bitmap->height >> mipmap_index;
    if ( height <= 1 )
        height = 1;
    __int16 depth = bitmap->depth >> mipmap_index;
    if ( depth <= 1 )
        depth = 1;

    if ( (bitmap->flags & (1u << _bitmap_compressed_bit)) != 0 )
    {
        width += -width & 3;    /* round up to block multiple of 4 */
        height += -height & 3;
    }

    int pixel_count = depth * height * width;
    if ( (unsigned __int16)bitmap->type == bitmap_type_cube_map )
        pixel_count *= 6;
    return pixel_count;
}
