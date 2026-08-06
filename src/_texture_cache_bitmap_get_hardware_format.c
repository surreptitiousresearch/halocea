/* _texture_cache_bitmap_get_hardware_format @0x836F4200 — return the hardware texture handle for a
 * bitmap, lazily uploading it (rasterizer_bitmap_new) if it has not been realized yet. `block` waits
 * for the upload, `load` forces it. */

#include <stdint.h>
#include "headers/bitmap_data.h"

extern uint8_t rasterizer_bitmap_new(bitmap_data *bitmap);
extern int  hcex_tex_get_hardware_format(void *hardware_format, int block, int load); /* DEVIATION: block/load are int, not unsigned char -- def src/hcex/hcex_tex_get_hardware_format.cpp + prologue mr r30,r4 / cmpwi r5 @0x823F2114 */

int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block,
        uint8_t load)
{
    if ( !bitmap->hardware_format )
        rasterizer_bitmap_new(bitmap);
    return hcex_tex_get_hardware_format(bitmap->hardware_format, block, load);
}
