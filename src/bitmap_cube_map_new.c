/* bitmap_cube_map_new @0x836F5580 — allocate a new cube-map bitmap_data (width==height, depth 1, signature
 * 'bitm', type cube_map). Flags default to pow2|free_on_delete; DXT1-family formats (dxt1..dxt5) instead add
 * the compressed bit, and p8_bump additionally ORs in the palettized bit. Allocates the pixel storage sized
 * for width*height pixels at the format's bits-per-pixel. Returns NULL if the initial allocation fails. */

#include <stdint.h>
#include <string.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"
#include "headers/bitmap_format.h"
#include "headers/bitmap_flags.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern int bitmap_get_pixel_count(const bitmap_data *bitmap);
extern const int8_t bitmap_format_bits_per_pixel_table[];  /* lbzx+extsb stride-1 byte table (all 15 binary access sites) */

bitmap_data * bitmap_cube_map_new(int16_t width, int16_t mipmap_count, int16_t format)
{
    bitmap_data *bitmap = dlMalloc(0x30u, "D:\\Projects\\code\\HCEX\\sources\\bitmaps\\bitmaps.c", 0x12Fu);
    if ( !bitmap )
        return bitmap;

    memset(bitmap, 0, sizeof(bitmap_data));
    bitmap->width = width;
    bitmap->height = width;
    bitmap->depth = 1;
    bitmap->signature = 0x6269746Du;   /* 'bitm' */
    bitmap->type = bitmap_type_cube_map;
    bitmap->format = format;
    bitmap->mipmap_count = mipmap_count;
    bitmap->hardware_format = 0;
    bitmap->flags = (1u << _bitmap_has_power_of_two_dimensions_bit)
                  | (1u << _bitmap_free_on_delete_bit);   /* 0x41 */

    if ( format >= _bitmap_format_dxt1 && format <= _bitmap_format_dxt5 )
        bitmap->flags = (1u << _bitmap_has_power_of_two_dimensions_bit)
                      | (1u << _bitmap_compressed_bit)
                      | (1u << _bitmap_free_on_delete_bit);   /* 0x43 */
    if ( format == _bitmap_format_p8_bump )
        bitmap->flags |= (1u << _bitmap_palettized_bit);   /* 0x4 */

    int pixel_count = bitmap_get_pixel_count(bitmap);
    bitmap->base_address = dlMalloc(pixel_count * bitmap_format_bits_per_pixel_table[format] / 8,
        "D:\\Projects\\code\\HCEX\\sources\\bitmaps\\bitmaps.c", 0x14Du);

    return bitmap;
}
