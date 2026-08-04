/* rasterizer_bitmap_changed @ 0x83793D30
   Uploads a bitmap's pixels to its hardware texture after a CPU-side change,
   dispatching by bitmap type: 0 = 2D, 1 = 3D (volume), 2 = cubemap. Each upload
   is skipped unless a device, a pixel buffer, and a hardware texture all exist. */

#include "headers/bitmap_data.h"
#include "headers/bitmap_format_tables.h"
#include "headers/bitmap_type.h"
#include "headers/rasterizer_engine_globals.h"

extern void hcex_tex_2d_copytohw(void *hardware_format, void *pixels, int width, int height,
                                 int mipmap_levels, _D3DFORMAT format, int pixels_size);
extern void hcex_tex_3d_copytohw(void *hardware_format, void *pixels, int width, int height,
                                 int mipmap_levels, int depth, _D3DFORMAT format, int pixels_size);
extern void hcex_tex_cube_copytohw(void *hardware_format, void *pixels, int width, int height,
                                   int mipmap_levels, _D3DFORMAT format, int pixels_size);

void rasterizer_bitmap_changed(bitmap_data *bitmap)
{
    unsigned short type = (unsigned short)bitmap->type;

    if ( type == bitmap_type_2d )
    {
        if ( global_d3d_device && bitmap->base_address && bitmap->hardware_format )
            hcex_tex_2d_copytohw(bitmap->hardware_format, bitmap->base_address,
                                 bitmap->width, bitmap->height, bitmap->mipmap_count + 1,
                                 rasterizer_bitmap_format_table[bitmap->format], bitmap->pixels_size);
    }
    else if ( type == bitmap_type_3d )
    {
        if ( global_d3d_device && bitmap->base_address && bitmap->hardware_format )
            hcex_tex_3d_copytohw(bitmap->hardware_format, bitmap->base_address,
                                 bitmap->width, bitmap->height, bitmap->mipmap_count + 1,
                                 bitmap->depth,
                                 rasterizer_bitmap_format_table[bitmap->format], bitmap->pixels_size);
    }
    else if ( type <= bitmap_type_cube_map )  /* type == bitmap_type_cube_map; bitmap_type_white takes no upload path */
    {
        if ( global_d3d_device && bitmap->base_address && bitmap->hardware_format )
            hcex_tex_cube_copytohw(bitmap->hardware_format, bitmap->base_address,
                                   bitmap->width, bitmap->height, bitmap->mipmap_count + 1,
                                   rasterizer_bitmap_format_table[bitmap->format], bitmap->pixels_size);
    }
}
