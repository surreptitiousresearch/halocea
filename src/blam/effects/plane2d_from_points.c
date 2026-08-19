/* plane2d_from_points @0x83740088 — build a 2D plane (line) through point0 and point1: the normal is the line
 * direction rotated 90 degrees, (dy, -dx), normalized; the distance is the projection of point0 onto the normal.
 * Returns null (and leaves the normal unnormalized with d = 0) when the two points are coincident. */

#include "headers/real_plane2d.h"
#include "headers/real_point2d.h"

extern float __fsqrts(float x);
extern double __fabs(double x);

real_plane2d *plane2d_from_points(real_plane2d *plane, const real_point2d *point0, const real_point2d *point1)
{
    float normal_x = point1->n[1] - point0->n[1];
    float normal_y = point0->n[0] - point1->n[0];
    plane->n.n[0] = normal_x;
    plane->n.n[1] = normal_y;

    float length = __fsqrts(normal_x * normal_x + normal_y * normal_y);
    if (__fabs(length) < 0.0001f || length == 0.0f)
    {
        plane->d = 0.0f;
        return nullptr;
    }

    float inverse_length = 1.0f / length;
    plane->n.n[0] = normal_x * inverse_length;
    plane->n.n[1] = normal_y * inverse_length;
    plane->d = point0->n[1] * (normal_y * inverse_length) + point0->n[0] * (normal_x * inverse_length);
    return plane;
}
