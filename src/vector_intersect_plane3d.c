/* vector_intersect_plane3d @0x837EA180 — parametric distance t along vector from point to the plane
 * (point + t*vector lies on plane), i.e. -(dot(point, normal) - distance) / dot(vector, normal). */

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_plane3d.h"

float vector_intersect_plane3d(const real_point3d *point, const real_vector3d *vector, const real_plane3d *plane)
{
    float point_distance = point->n[0] * plane->n.n[0]
                          + point->n[1] * plane->n.n[1]
                          + point->n[2] * plane->n.n[2]
                          - plane->d;

    float vector_dot_normal = vector->n[0] * plane->n.n[0]
                             + vector->n[1] * plane->n.n[1]
                             + vector->n[2] * plane->n.n[2];

    return -(point_distance / vector_dot_normal);
}
