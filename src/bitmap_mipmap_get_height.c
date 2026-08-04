/* bitmap_mipmap_get_height @0x836F4930 — the pixel height of one mip level of a bitmap, floored to 1,
 * rounded up to a multiple of 4 for compressed (S3TC) bitmaps. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"

/* attested: return int16_t (extsh r3 at both exits; callers re-extsh) */
int16_t bitmap_mipmap_get_height(const bitmap_data *bitmap, int16_t mipmap_index)
{
    int16_t height = bitmap->height >> mipmap_index;
    if (height <= 1)
        height = 1;

    if (bitmap->flags & (1u << _bitmap_compressed_bit))
        return (-height & 3) + height; /* round up to a multiple of 4 */
    return height;
}
