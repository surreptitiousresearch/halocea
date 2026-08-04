/* bitmap_clone @0x8377A510 — allocate a new bitmap of the same shape (2D / 3D / cube map by type; types
 * past 2 return NULL) and copy the source's full pixel data and flags into it. Returns the clone (which
 * may have a NULL base_address on allocation failure, matching the compiled behavior of returning it
 * uncopied). */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"
#include <string.h>

extern bitmap_data *bitmap_2d_new(int16_t width, int16_t height, int16_t mipmap_count, int16_t format);
extern bitmap_data * bitmap_3d_new(int16_t width, int16_t height, int16_t depth, int16_t mipmap_count, int16_t format);
extern bitmap_data * bitmap_cube_map_new(int16_t width, int16_t mipmap_count, int16_t format);
extern void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index);
extern int bitmap_get_pixel_data_size(const bitmap_data *bitmap);

bitmap_data * bitmap_clone(const bitmap_data *source_bitmap)
{
    bitmap_data *clone;
    unsigned int type = (unsigned __int16)source_bitmap->type;

    if ( type == bitmap_type_2d )
    {
        clone = bitmap_2d_new(source_bitmap->width, source_bitmap->height, source_bitmap->mipmap_count,
                              source_bitmap->format);
    }
    else if ( type == bitmap_type_3d )
    {
        clone = bitmap_3d_new(source_bitmap->width, source_bitmap->height, source_bitmap->depth,
                              source_bitmap->mipmap_count, source_bitmap->format);
    }
    else if ( type == bitmap_type_cube_map )
    {
        clone = bitmap_cube_map_new(source_bitmap->width, source_bitmap->mipmap_count,
                                    source_bitmap->format);
    }
    else
    {
        return 0;
    }

    if ( clone && clone->base_address )
    {
        memcpy(bitmap_mipmap_address(clone, 0), bitmap_mipmap_address(source_bitmap, 0),
               bitmap_get_pixel_data_size(source_bitmap));
        clone->flags = source_bitmap->flags;
    }
    return clone;
}
