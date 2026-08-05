/* convex_polygon2d_verify @0x837FE274 — checks that every point in a 2D polygon is a valid (non-NaN)
 * point; empty polygons (count <= 0) trivially pass. */

#include <stdint.h>
#include "headers/real_point2d.h"

extern uint8_t valid_real_point2d(const real_point2d *p);

uint8_t convex_polygon2d_verify(int16_t count, const real_point2d *points)
{
    for ( int16_t i = 0; i < count; ++i )
    {
        if ( !valid_real_point2d(&points[i]) )
            return 0;
    }

    return 1;
}
