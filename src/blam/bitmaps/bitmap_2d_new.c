/* bitmap_2d_new @ 0x836F5320
   Allocates a 2D bitmap_data and its pixel buffer. Sets power-of-two,
   compressed (DXT, formats 14-16) and palettized (format 17) flags, then
   allocates pixels sized from the pixel count and bits-per-pixel table. */

#include <stdint.h>
#include <string.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format_tables.h"
#include "headers/bitmap_format.h"
#include "headers/bitmap_flags.h"
#include "headers/bitmap_type.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern int bitmap_get_pixel_count(const bitmap_data *bitmap);

#define BITMAPS_SRC "D:\\Projects\\code\\HCEX\\sources\\bitmaps\\bitmaps.c"

bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format)
{
    bitmap_data *bitmap = dlMalloc(0x30u, BITMAPS_SRC, 0xB8);
    if ( !bitmap )
        return bitmap;

    memset(bitmap, 0, sizeof(bitmap_data));
    bitmap->width = width;
    bitmap->height = height;
    bitmap->format = format;
    bitmap->mipmap_count = mipmap_count;
    bitmap->signature = 0x6269746Du; /* 'bitm' */
    bitmap->depth = 1;
    bitmap->type = bitmap_type_2d;
    bitmap->flags = 1u << _bitmap_free_on_delete_bit;

    if ( ((width - 1) & width) == 0 && ((height - 1) & height) == 0 )
        bitmap->flags = (1u << _bitmap_free_on_delete_bit) | (1u << _bitmap_has_power_of_two_dimensions_bit);
    if ( format >= BITMAP_FIRST_COMPRESSED_FORMAT && format <= BITMAP_LAST_COMPRESSED_FORMAT )
        bitmap->flags |= 1u << _bitmap_compressed_bit;   /* DXT1/3/5 */
    if ( format == _bitmap_format_p8_bump )
        bitmap->flags |= 1u << _bitmap_palettized_bit;

    int pixel_count = bitmap_get_pixel_count(bitmap);
    bitmap->base_address = dlMalloc(
        pixel_count * bitmap_format_bits_per_pixel_table[format] / 8,
        BITMAPS_SRC, 0xD5);
    return bitmap;
}
