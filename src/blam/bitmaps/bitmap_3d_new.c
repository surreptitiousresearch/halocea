/* bitmap_3d_new @0x836F5448 — allocate a new 3D bitmap_data (signature 'bitm' (1651078253), type 1).
 * Flags default to 0x40; if width/height/depth are all powers of two, flags become 0x41 instead.
 * DXT1-family formats (14..16) additionally OR in 0x2, and format 17 additionally ORs in 0x4. Allocates
 * pixel storage sized for width*height*depth pixels at the format's bits-per-pixel. Returns NULL if the
 * initial allocation fails. Sibling of bitmap_cube_map_new.c/bitmap_2d_new.c. */

#include <stdint.h>
#include <string.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format.h"
#include "headers/bitmap_flags.h"
#include "headers/bitmap_type.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern int bitmap_get_pixel_count(const bitmap_data *bitmap);
extern const int8_t bitmap_format_bits_per_pixel_table[];  /* lbzx+extsb stride-1 byte table (all 15 binary access sites) */

bitmap_data * bitmap_3d_new(int16_t width, int16_t height, int16_t depth, int16_t mipmap_count, int16_t format)
{
    bitmap_data *bitmap = dlMalloc(0x30u, "D:\\Projects\\code\\HCEX\\sources\\bitmaps\\bitmaps.c", 0xF5u);
    if ( !bitmap )
        return bitmap;

    memset(bitmap, 0, sizeof(bitmap_data));
    bitmap->width = width;
    bitmap->height = height;
    bitmap->depth = depth;
    bitmap->format = format;
    bitmap->signature = 0x6269746Du;   /* 'bitm' */
    bitmap->type = bitmap_type_3d;
    bitmap->flags = 1u << _bitmap_free_on_delete_bit;
    bitmap->mipmap_count = mipmap_count;

    if ( ((width - 1) & width) == 0 && ((height - 1) & height) == 0 && ((depth - 1) & depth) == 0 )
        bitmap->flags = (1u << _bitmap_free_on_delete_bit) | (1u << _bitmap_has_power_of_two_dimensions_bit);

    if ( format >= BITMAP_FIRST_COMPRESSED_FORMAT && format <= BITMAP_LAST_COMPRESSED_FORMAT )
        bitmap->flags |= 1u << _bitmap_compressed_bit;
    if ( format == _bitmap_format_p8_bump )
        bitmap->flags |= 1u << _bitmap_palettized_bit;

    int pixel_count = bitmap_get_pixel_count(bitmap);
    bitmap->base_address = dlMalloc(pixel_count * bitmap_format_bits_per_pixel_table[format] / 8,
        "D:\\Projects\\code\\HCEX\\sources\\bitmaps\\bitmaps.c", 0x112u);

    return bitmap;
}
