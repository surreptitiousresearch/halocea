/* flag_tesselate_region @0x8380DE90 — stamp a `size`x`size` block of a flag's cloth-cell tesselation grid
 * (flag->cells, column-major with stride `height-1`, clipped to [0,width-1)x[0,height-1)) starting at
 * (x,y) with `tesselation`. For cells that lie exactly on the block's diagonal (per `tesselation`'s
 * orientation — 4/5 vs other for the horizontal axis, 4/2 vs other for the vertical), the cell is set
 * directly to `tesselation`; otherwise it gets a small 0-3 index selecting which side of that diagonal the
 * cell falls on (reproduced via the compiled sign/comparison bit-trick rather than re-derived, to avoid
 * introducing a sign-handling mistake). */

#include "headers/flag_definition.h"
#include "headers/flag_datum.h"

void flag_tesselate_region(flag_definition *flag_definition, flag_datum *flag, __int16 x, __int16 y,
    __int16 size, __int16 tesselation)
{
    __int16 origin_x = x;
    __int16 origin_y = y;
    __int16 x_end = size + x;

    for ( ; x < x_end; x++ )
    {
        __int16 y_end = origin_y + size;
        for ( __int16 row = y; row < y_end; row++ )
        {
            if ( x < 0 || row < 0 || x >= flag_definition->width - 1 )
                continue;

            __int16 height_stride = flag_definition->height - 1;
            if ( row >= height_stride )
                continue;

            __int16 horizontal_side, vertical_side;
            if ( tesselation == 4 || tesselation == 5 )
                horizontal_side = x - origin_x;
            else
                horizontal_side = size - x + origin_x - 1;

            if ( tesselation == 4 || tesselation == 2 )
                vertical_side = row - origin_y;
            else
                vertical_side = origin_y - row + size - 1;

            __int16 *cell = &flag->cells[height_stride * x + row].tesselation;
            if ( horizontal_side == vertical_side )
            {
                *cell = tesselation;
            }
            else
            {
                *cell = (vertical_side >> 15) + (vertical_side >= (unsigned int)horizontal_side)
                      + ((unsigned int)horizontal_side >> 31);
            }
        }
    }
}
