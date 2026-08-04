/* bitmap_delete @0x836F42E8 — release a bitmap: drop its rasterizer resources, then free its pixel
 * data and the record itself when the bitmap owns its allocation (flag 0x40). */

#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"

extern void rasterizer_bitmap_delete(bitmap_data *bitmap);
extern void dlFree(void *ptr);

void bitmap_delete(bitmap_data *bitmap)
{
    if ( bitmap )
    {
        rasterizer_bitmap_delete(bitmap);
        if ( (bitmap->flags & (1u << _bitmap_free_on_delete_bit)) != 0 )
        {
            if ( bitmap->base_address )
                dlFree(bitmap->base_address);
            dlFree(bitmap);
        }
    }
}
