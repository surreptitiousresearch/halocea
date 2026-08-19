/* plane3d_from_point_and_normal @0x8369AE18 — construct a plane from a point on it and a
 * (presumed unit) normal: copies the normal into plane.n and sets plane.d = dot(normal, point). */

#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

real_plane3d *plane3d_from_point_and_normal(real_plane3d *plane, const real_point3d *point,
        const real_vector3d *normal)
{
    plane->n.n[0] = normal->n[0];
    plane->n.n[1] = normal->n[1];
    plane->n.n[2] = normal->n[2];
    plane->d = plane->n.n[0] * point->n[0]
             + plane->n.n[1] * point->n[1]
             + plane->n.n[2] * point->n[2];
    return plane;
}
