/* rasterizer_bitmap_cm_changed @0x837936F8 — pushes a modified cube-map bitmap's pixel data to the hardware
 * texture, if a D3D device is active and the bitmap has both a base address and a hardware format. Mirrors
 * rasterizer_bitmap_2d_changed.c for the cube-map upload path. */

#include "headers/bitmap_data.h"
#include "headers/rasterizer_engine_globals.h"
#include "headers/bitmap_format_tables.h"

extern void hcex_tex_cube_copytohw(void *tex, void *tex_data, int width, int height, int mipmaps, _D3DFORMAT fmt, int tex_size);

void rasterizer_bitmap_cm_changed(bitmap_data *bitmap)
{
    if ( !global_d3d_device )
        return;

    void *base_address = bitmap->base_address;
    if ( !base_address )
        return;

    void *hardware_format = bitmap->hardware_format;
    if ( !hardware_format )
        return;

    hcex_tex_cube_copytohw(hardware_format, base_address, bitmap->width, bitmap->height,
        bitmap->mipmap_count + 1, rasterizer_bitmap_format_table[bitmap->format], bitmap->pixels_size);
}
