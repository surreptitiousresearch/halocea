/* rasterizer_transparent_geometry_initialize @ 0x83768250
   Allocates the transparent-geometry group tables (primary, sorted-index, and
   secondary) from the Blam pool, resets the counts, and initializes the
   auxiliary vertex buffer. Fails if any allocation fails. */

#include <stdint.h>
#include "headers/transparent_geometry_globals.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern uint8_t rasterizer_transparent_geometry_initialize_aux_buffer(void);

#define TG_SRC "D:\\Projects\\code\\HCEX\\sources\\rasterizer\\rasterizer_transparent_geometry.c"

/* Caller re-normalizes r3 with `clrlwi r11,r3,24` => 8-bit (Blam boolean) return. */
uint8_t rasterizer_transparent_geometry_initialize(void)
{
    transparent_geometry_groups =
        dlMalloc(0xFC00u, TG_SRC, 0x29);
    transparent_geometry_group_sorted_indices =
        dlMalloc(0x300u, TG_SRC, 0x2B);
    transparent_geometry_groups2 =
        dlMalloc(0x1500u, TG_SRC, 0x2E);

    transparent_geometry_group_count2 = 0;
    transparent_geometry_group_count = 0;

    if ( !transparent_geometry_groups
      || !transparent_geometry_group_sorted_indices
      || !transparent_geometry_groups2 )
        return 0;

    if ( !rasterizer_transparent_geometry_initialize_aux_buffer() )
        return 0;
    return 1;
}
