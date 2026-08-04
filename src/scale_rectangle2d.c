/* scale_rectangle2d @0x8379A598 — fit `source`'s aspect ratio inside `bounds`, scale the fitted rectangle
 * by `scale` (8.8 fixed-point), and center the result within the original bounds extent on each axis
 * independently. Writes and returns `destination`. */

#include <stdint.h>
#include "headers/rectangle2d.h"

rectangle2d * scale_rectangle2d(const rectangle2d *bounds, const rectangle2d *source, rectangle2d *destination, int16_t scale)
{
    __int16 bounds_width = bounds->x1 - bounds->x0;
    __int16 bounds_height = bounds->y1 - bounds->y0;
    __int16 source_width = source->x1 - source->x0;
    __int16 source_height = source->y1 - source->y0;

    __int16 original_bounds_width = bounds_width;
    __int16 original_bounds_height = bounds_height;

    if ( bounds_height * source_width <= bounds_width * source_height )
        bounds_width = bounds_height * source_width / source_height;
    else
        bounds_height = bounds_width * source_height / source_width;

    __int16 scaled_width = (__int16)(bounds_width * scale) >> 8;
    __int16 scaled_height = (__int16)(bounds_height * scale) >> 8;

    rectangle2d result;
    result.x0 = (original_bounds_width - scaled_width) / 2 + bounds->x0;
    result.x1 = result.x0 + scaled_width;
    result.y0 = (original_bounds_height - scaled_height) / 2 + bounds->y0;
    result.y1 = result.y0 + scaled_height;

    *destination = result;
    return destination;
}
