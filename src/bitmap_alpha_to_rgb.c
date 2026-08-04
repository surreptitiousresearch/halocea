/* bitmap_alpha_to_rgb @0x8377A4B0 — replaces every pixel's 4 bytes with its own byte-0 value splatted across
 * all 4 channels (byte 0, in this bitmap's memory byte order, is the alpha channel — hence the name).
 *
 * DEVIATION: the decompiler rendered the splat as a chain of `__ROL4__`/shift/or operations on a
 * zero-extended byte; reproduced with the same read-before-overwrite ordering (each pixel's original byte 0
 * is read prior to that pixel being rewritten) but as a plain byte splat rather than the rotate-idiom. */

#include <stdint.h>
#include "headers/bitmap_data.h"

extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);
extern int bitmap_get_pixel_count(const bitmap_data *bitmap);

void bitmap_alpha_to_rgb(bitmap_data *bitmap)
{
    unsigned char *pixel = (unsigned char *)bitmap_2d_address(bitmap, 0, 0, 0);
    int pixel_count = bitmap_get_pixel_count(bitmap);

    for ( int i = 0; i < pixel_count; ++i )
    {
        unsigned char alpha = pixel[0];
        unsigned int splat = (alpha << 24) | (alpha << 16) | (alpha << 8) | alpha;

        *(unsigned int *)pixel = splat;
        pixel += 4;
    }
}
