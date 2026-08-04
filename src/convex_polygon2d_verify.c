/* convex_polygon2d_verify @0x837FE274 — checks that every point in a 2D polygon is a valid (non-NaN)
 * point; empty polygons (count <= 0) trivially pass. */

#include "headers/real_point2d.h"

extern unsigned __int8 valid_real_point2d(const real_point2d *p);

unsigned __int8 convex_polygon2d_verify(__int16 count, const real_point2d *points)
{
    for ( __int16 i = 0; i < count; ++i )
    {
        if ( !valid_real_point2d(&points[i]) )
            return 0;
    }

    return 1;
}
