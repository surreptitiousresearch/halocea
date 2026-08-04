/* render_frustum_sphere_visible @0x8376CCD8 — classify a world-space sphere against a view frustum: returns 0
 * when the sphere is fully outside, 1 when it straddles a frustum plane, and 2 when it lies fully inside. A
 * cheap AABB reject runs first, then each of the six frustum planes' signed distance is tested. */

#include <stdint.h>
#include "headers/render_frustum.h"
#include "headers/real_point3d.h"

static float plane_signed_distance(const real_plane3d *plane, const real_point3d *point)
{
    return (plane->normal.n[0] * point->n[0]
            + plane->normal.n[2] * point->n[2]
            + plane->normal.n[1] * point->n[1])
           - plane->distance;
}

int16_t render_frustum_sphere_visible(const render_frustum *frustum, const real_point3d *point, float radius)
{
    /* AABB reject against the frustum's world bounds [x0,x1,y0,y1,z0,z1]. */
    if (frustum->world_bounds.n[1] < point->n[0] - radius)
        return 0;
    if (frustum->world_bounds.n[3] < point->n[1] - radius)
        return 0;
    if (frustum->world_bounds.n[5] < point->n[2] - radius)
        return 0;
    if (frustum->world_bounds.n[0] > point->n[0] + radius)
        return 0;
    if (frustum->world_bounds.n[2] > point->n[1] + radius)
        return 0;
    if (frustum->world_bounds.n[4] > point->n[2] + radius)
        return 0;

    float distance0 = plane_signed_distance(&frustum->world_planes[0], point);
    if (distance0 > radius)
        return 0;
    float distance1 = plane_signed_distance(&frustum->world_planes[1], point);
    if (distance1 > radius)
        return 0;
    float distance2 = plane_signed_distance(&frustum->world_planes[2], point);
    if (distance2 > radius)
        return 0;
    float distance3 = plane_signed_distance(&frustum->world_planes[3], point);
    if (distance3 > radius)
        return 0;
    if (plane_signed_distance(&frustum->world_planes[4], point) > radius)
        return 0;
    float distance5 = plane_signed_distance(&frustum->world_planes[5], point);
    if (distance5 > radius)
        return 0;

    /* Fully inside only when every retained plane keeps the whole sphere on the inside half-space; any plane
     * the sphere crosses (signed distance >= -radius) makes it a straddling (partial) result. Plane 4 is not
     * reconsidered here, matching the original. */
    float negative_radius = -radius;
    if (distance0 >= negative_radius)
        return 1;
    if (distance1 >= negative_radius)
        return 1;
    if (distance2 >= negative_radius)
        return 1;
    if (distance3 >= negative_radius)
        return 1;
    if (distance5 >= negative_radius)
        return 1;

    return 2;
}
