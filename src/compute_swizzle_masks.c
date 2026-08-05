#include <stdint.h>
#include "headers/blam_data_globals.h"
/* compute_swizzle_masks @0x837E30A0 — computes X/Y/Z bit-interleave (swizzle) masks for a
 * width x height x depth volume texture. rasterizer_swizzle_x_mask (the DB's own auto-name; kept verbatim, no better
 * name resolved) holds the X mask, alongside the DB's own ay/az globals for Y/Z. */

extern unsigned int rasterizer_swizzle_x_mask;

void compute_swizzle_masks(int16_t width, int16_t height, int16_t depth)
{
    unsigned int z_mask = 0;
    unsigned int y_mask = 0;
    unsigned int x_mask = 0;
    unsigned int size = 1;
    unsigned int bit = 1;
    unsigned int advanced;

    az = 0;
    ay = 0;
    rasterizer_swizzle_x_mask = 0;

    do
    {
        advanced = 0;

        if ( size < (uint16_t)width )
        {
            x_mask |= bit;
            bit *= 2;
            advanced = bit;
        }
        if ( size < (uint16_t)height )
        {
            y_mask |= bit;
            bit *= 2;
            ay = y_mask;
            advanced = bit;
        }
        if ( size < (uint16_t)depth )
        {
            z_mask |= bit;
            bit *= 2;
            az = z_mask;
            advanced = bit;
        }

        size *= 2;
    }
    while ( advanced );

    rasterizer_swizzle_x_mask = x_mask;
}
