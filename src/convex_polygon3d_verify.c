/* convex_polygon3d_verify @0x837FE7A0 — return whether a 3D polygon (count vertices) is convex and free of
 * NaN coordinates. A reference normal is taken from the first three vertices; then for every vertex the
 * signed turn (cross product of the outgoing and incoming edges) is projected onto that normal. If any
 * vertex has a NaN coordinate, or any turn projects negative (a reflex/concave corner, beyond a small
 * tolerance), the polygon is rejected. A degenerate count (<= 0) is accepted.
 *
 * The reference normal is computed from points[0..2] before the count check, matching the binary (callers
 * pass real polygons with count >= 3). */

#include "headers/real_point3d.h"

extern int _isnan(double x);

unsigned __int8 convex_polygon3d_verify(__int16 count, const real_point3d *points)
{
    float normal_z = (points[2].n[1] - points[1].n[1]) * (points[0].n[0] - points[1].n[0])
                   - (points[2].n[0] - points[1].n[0]) * (points[0].n[1] - points[1].n[1]);
    float normal_x = (points[2].n[2] - points[1].n[2]) * (points[0].n[1] - points[1].n[1])
                   - (points[2].n[1] - points[1].n[1]) * (points[0].n[2] - points[1].n[2]);
    float normal_y = (points[0].n[2] - points[1].n[2]) * (points[2].n[0] - points[1].n[0])
                   - (points[2].n[2] - points[1].n[2]) * (points[0].n[0] - points[1].n[0]);

    if ( count <= 0 )
        return 1;

    for ( int i = 0; i < count; i = (__int16)(i + 1) )
    {
        const real_point3d *previous = &points[(i ? i : count) - 1];
        const real_point3d *current = &points[i];
        const real_point3d *next = (i == count - 1) ? points : &points[i + 1];

        if ( _isnan(current->n[0]) || _isnan(current->n[1]) || _isnan(current->n[2]) )
            return 0;

        float turn =
            ((next->n[2] - current->n[2]) * (previous->n[1] - current->n[1])
                - (next->n[1] - current->n[1]) * (previous->n[2] - current->n[2])) * normal_x
          + ((next->n[1] - current->n[1]) * (previous->n[0] - current->n[0])
                - (previous->n[1] - current->n[1]) * (next->n[0] - current->n[0])) * normal_z
          + ((previous->n[2] - current->n[2]) * (next->n[0] - current->n[0])
                - (next->n[2] - current->n[2]) * (previous->n[0] - current->n[0])) * normal_y;

        if ( turn < -0.000001f )
            return 0;
    }

    return 1;
}
