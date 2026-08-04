/* bitmap_get_pixel_count @ 0x836F5000
   Sums the texel count across all mipmap levels. Each level halves w/h/d (min
   1); compressed (DXT) formats round width and height up to a multiple of 4;
   cubemaps (type 2) multiply by 6 faces. */

#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"
#include "headers/bitmap_type.h"

int bitmap_get_pixel_count(const bitmap_data *bitmap)
{
    short mipmap_count = bitmap->mipmap_count;
    int total = 0;
    if ( mipmap_count < 0 )
        return total;

    int depth = bitmap->depth;
    int width = bitmap->width;
    int height = bitmap->height;
    int compressed = bitmap->flags & (1u << _bitmap_compressed_bit);
    int type = bitmap->type;

    for ( int level = 0; level <= mipmap_count; level = (short)(level + 1) )
    {
        short level_width = width >> level;
        if ( (width >> level) <= 1 )
            level_width = 1;
        if ( compressed )
            level_width = (-level_width & 3) + level_width; /* round up to /4 */

        short level_height = height >> level;
        if ( (height >> level) <= 1 )
            level_height = 1;
        if ( compressed )
            level_height = (-level_height & 3) + level_height;

        short level_depth = depth >> level;
        if ( (depth >> level) <= 1 )
            level_depth = 1;

        int level_texels = level_depth * level_height * level_width;
        if ( type == bitmap_type_cube_map )
            level_texels *= 6; /* cubemap faces */
        total += level_texels;
    }
    return total;
}
