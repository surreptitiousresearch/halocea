/* bitmap_verify @0x836F5158 — sanity-check a bitmap_data record before it is used or imported. Rejects a bad
 * 'bitm' signature, reserved flag bits (0xFE00), out-of-range type (>=4) or format (>0x11), non-positive or
 * absurdly large (>30000) width/height, and an invalid depth (must be 1..256, and >1 only for 3D textures,
 * type 1). The mipmap count must not exceed floor_log2 of the largest dimension. On import it additionally
 * requires the format-11 (P8/AY8-style) case with a valid base address, no mipmaps, and none of flags 2/4/8. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"
#include "headers/bitmap_format.h"
#include "headers/bitmap_flags.h"

extern int16_t floor_log2(unsigned int k);

uint8_t bitmap_verify(const bitmap_data *bitmap, uint8_t import)
{
    if ( bitmap->signature != 0x6269746Du ) /* 'bitm' */
        return 0;
    if ( (bitmap->flags & 0xFE00) != 0 ) /* reserved bits above the highest bitmap_flags bit (8) */
        return 0;

    unsigned int type = bitmap->type;
    if ( type >= number_of_bitmap_types || (unsigned int)bitmap->format > _bitmap_format_p8_bump )
        return 0;

    __int16 width = bitmap->width;
    if ( width <= 0 || width > 30000 )
        return 0;

    __int16 height = bitmap->height;
    if ( height <= 0 || height > 30000 )
        return 0;

    __int16 depth = bitmap->depth;
    if ( !(depth > 0 && depth <= 256 && (depth == 1 || type == bitmap_type_3d))
         || (unsigned __int16)bitmap->mipmap_count >= 0x8000u )
        return 0;

    int largest_dimension = width;
    if ( width <= (height > depth ? height : depth) )
        largest_dimension = (height <= depth) ? depth : height;

    int mipmap_count = bitmap->mipmap_count;
    if ( mipmap_count <= floor_log2(largest_dimension)
         && (!import
             || bitmap->format == _bitmap_format_a8r8g8b8
                && bitmap->base_address
                && !mipmap_count
                && (bitmap->flags & (1u << _bitmap_compressed_bit)) == 0
                && (bitmap->flags & (1u << _bitmap_palettized_bit)) == 0
                && (bitmap->flags & (1u << _bitmap_swizzled_bit)) == 0) )
    {
        return 1;
    }
    return 0;
}
