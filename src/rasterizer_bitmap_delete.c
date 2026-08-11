/* rasterizer_bitmap_delete @0x83793768 — release a bitmap's rasterizer-side resources: drop it from the
 * texture cache, then free its hardware texture handle. */

#include "headers/bitmap_data.h"

extern void texture_cache_bitmap_delete(bitmap_data *bitmap);
extern void hcex_tex_del(void *tex);

void rasterizer_bitmap_delete(bitmap_data *bitmap)
{
    texture_cache_bitmap_delete(bitmap);
    if ( bitmap )
    {
        if ( bitmap->hardware_format )
        {
            hcex_tex_del(bitmap->hardware_format);
            bitmap->hardware_format = 0;
        }
    }
}
