/* bitmap_mipmap_get_width @0x836F48E8 — the pixel width of one mip level of a bitmap, floored to 1,
 * rounded up to a multiple of 4 for compressed (S3TC) bitmaps. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"

/* attested: return int16_t (extsh r3 at both exits; callers re-extsh) */
int16_t bitmap_mipmap_get_width(const bitmap_data *bitmap, int16_t mipmap_index)
{
    int16_t width = bitmap->width >> mipmap_index;
    if (width <= 1)
        width = 1;

    if (bitmap->flags & (1u << _bitmap_compressed_bit))
        return (-width & 3) + width; /* round up to a multiple of 4 */
    return width;
}
