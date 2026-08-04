/* plane3d_distance_to_point @0x8369AE60 — signed distance from a point to a plane:
 * dot(plane.normal, point) - plane.d. Positive on the side the normal faces. */

#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"

float plane3d_distance_to_point(const real_plane3d *plane, const real_point3d *point)
{
    return plane->n.n[0] * point->n[0]
         + plane->n.n[1] * point->n[1]
         + plane->n.n[2] * point->n[2]
         - plane->d;
}
