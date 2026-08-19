/* bitmap_rebuild @ 0x836F4288
   Rebuilds a bitmap's hardware representation: allocates a new rasterizer
   bitmap if one does not yet exist, then notifies the rasterizer that the
   bitmap's contents changed. */

#include <stdint.h>
#include "headers/bitmap_data.h"

extern uint8_t rasterizer_bitmap_new(bitmap_data *bitmap);
extern void rasterizer_bitmap_changed(bitmap_data *bitmap);

void bitmap_rebuild(bitmap_data *bitmap)
{
    if ( bitmap->hardware_format == 0 )
        rasterizer_bitmap_new(bitmap);
    rasterizer_bitmap_changed(bitmap);
}
