/* halo_tex_get_hardware_format @0x836F3F10 — return the HCEX hardware texture format for a Blam bitmap,
 * lazily creating the rasterizer hardware-format object if the bitmap has not been realized yet. */

#include <stdint.h>
#include "../headers/bitmap_data.h"

extern "C" uint8_t rasterizer_bitmap_new(bitmap_data *bitmap);
extern "C" int  hcex_tex_get_hardware_format(void *hardware_format, int block, int load);

int halo_tex_get_hardware_format(bitmap_data *bitmap, int block, int load)
{
    if ( !bitmap->hardware_format )
        rasterizer_bitmap_new(bitmap);
    return hcex_tex_get_hardware_format(bitmap->hardware_format, block, load);
}
