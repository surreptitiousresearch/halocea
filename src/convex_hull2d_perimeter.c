/* convex_hull2d_perimeter @0x837FDCC4 — sums the edge lengths of a closed 2D convex hull (points[count-1]
 * back to points[0], then each consecutive pair). */

#include <stdint.h>
#include <math.h>
#include "headers/real_point2d.h"

float convex_hull2d_perimeter(int16_t count, const real_point2d *points)
{
    float dx = points[0].n[0] - points[count - 1].n[0];
    float dy = points[0].n[1] - points[count - 1].n[1];
    float perimeter = sqrtf(dx * dx + dy * dy);

    for ( __int16 i = 1; i < count; ++i )
    {
        dx = points[i].n[0] - points[i - 1].n[0];
        dy = points[i].n[1] - points[i - 1].n[1];
        perimeter += sqrtf(dx * dx + dy * dy);
    }

    return perimeter;
}
