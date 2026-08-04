/* plane2d_distance_to_point @0x837E0030 — signed distance from a point to a 2D plane: dot(plane.n, point) - plane.d. */

#include "headers/real_plane2d.h"
#include "headers/real_point2d.h"

float plane2d_distance_to_point(const real_plane2d *plane, const real_point2d *point)
{
    return plane->n.n[0] * point->n[0] + plane->n.n[1] * point->n[1] - plane->d;
}
