/* convex_hull2d_test_point @0x837FDAF0 — test whether a 2D point lies inside (or within `epsilon` of) a
 * convex polygon given as an ordered vertex ring. For each directed edge the signed area of the triangle
 * (edge_start, edge_end, point) must be >= -epsilon (point on the inside, allowing a small tolerance); if
 * any edge puts the point strictly outside, returns 0. Returns 1 if inside every edge. */

#include <stdint.h>
#include "headers/real_point2d.h"

uint8_t convex_hull2d_test_point(int16_t count, const real_point2d *points, const real_point2d *point,
        float epsilon)
{
    int i;

    if ( count <= 0 )
        return 1;

    for ( i = 0; i < count; ++i )
    {
        int next = (i + 1 < count) ? i + 1 : 0;   /* wrap to first vertex on the last edge */
        float side = (points[next].n[0] - points[i].n[0]) * (point->n[1] - points[i].n[1])
                     - (point->n[0] - points[i].n[0]) * (points[next].n[1] - points[i].n[1]);

        if ( side < -epsilon )
            return 0;
    }
    return 1;
}
