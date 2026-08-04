/* rasterizer_bitmap_3d_changed @0x8379366C — if a D3D device is active and the bitmap has both a base
 * address and an allocated hardware format, re-uploads its 3D texture data to the GPU. */

#include "headers/bitmap_data.h"
#include "headers/blam_data_globals.h"

extern void hcex_tex_3d_copytohw(void *hardware_format, void *base_address, int width, int height,
    int mipmap_count_plus_one, int depth, int bitmap_format, int pixels_size);
extern int rasterizer_bitmap_format_table[];

void rasterizer_bitmap_3d_changed(bitmap_data *bitmap)
{
    if ( global_d3d_device )
    {
        void *base_address = bitmap->base_address;

        if ( base_address )
        {
            void *hardware_format = bitmap->hardware_format;

            if ( hardware_format )
            {
                hcex_tex_3d_copytohw(hardware_format, base_address, bitmap->width, bitmap->height,
                    bitmap->mipmap_count + 1, bitmap->depth, rasterizer_bitmap_format_table[bitmap->format],
                    bitmap->pixels_size);
            }
        }
    }
}
