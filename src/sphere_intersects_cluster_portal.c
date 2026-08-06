/* sphere_intersects_cluster_portal @0x837443E8 — does a query sphere cross a structure cluster portal?
 * Used by the cluster flood-fill to decide whether to step through a portal into the neighboring cluster.
 *
 * Three rejection tests, cheapest first:
 *   1. Plane distance: if the sphere center is farther than `radius` from the portal's plane, the sphere
 *      can't reach the portal -> miss.
 *   2. Bounding sphere: if the sphere is outside the portal's bounding sphere (portal radius + query
 *      radius), -> miss.
 *   3. 2D containment: project the sphere center and the portal polygon onto the portal plane's dominant
 *      coordinate plane, then test the projected circle against the convex polygon. The 2D circle radius
 *      is sqrt(radius^2 - plane_distance^2) (the sphere's footprint on the plane).
 * Returns 1 on intersection, 0 on miss.
 *
 * radius arrives in f1 (4th argument, no trailing GPR-skip param here since the float is last). */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/cluster_portal.h"
#include "headers/bsp3d.h"
#include "headers/real_plane3d.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern int16_t projection_from_vector3d(const real_vector3d *n);
extern uint8_t convex_hull2d_test_circle(int16_t count, const real_point2d *points, const real_point2d *p, float radius);

uint8_t sphere_intersects_cluster_portal(structure_bsp *structure, int16_t portal_index,
        const real_point3d *point, float radius)
{
    const cluster_portal *portal = &((const cluster_portal *)structure->cluster_portals.address)[portal_index];
    int    plane_index = portal->plane_index;
    real_point2d vertices_2d[133];
    real_point2d center_2d;
    float  projected[3];
    const real_vector3d *plane_normal;
    const real_plane3d *collision_plane;
    float  plane_distance;
    float  dx, dy, dz, reach;
    int    axis, axis_sign, keep0, keep1;
    int    vertex_count, i;

    /* 1. distance from sphere center to portal plane */
    /* recovered: *((int *)collision_bsp.address + 4) -> bsp3d.planes.address (byte 0x10) */
    const bsp3d *collision_bsp = (const bsp3d *)structure->collision_bsp.address;
    collision_plane = &((const real_plane3d *)collision_bsp->planes.address)[plane_index];
    plane_distance = (point->n[0] * collision_plane->n.n[0]
                      + collision_plane->n.n[1] * point->n[1]
                      + collision_plane->n.n[2] * point->n[2])
                     - collision_plane->d;
    if ( __fabs(plane_distance) >= radius )
        return 0;

    /* 2. portal bounding sphere */
    dx = portal->centroid.n[0] - point->n[0];
    dy = portal->centroid.n[1] - point->n[1];
    dz = portal->centroid.n[2] - point->n[2];
    reach = portal->bounding_radius + radius;
    if ( dx * dx + dy * dy + dz * dz >= reach * reach )
        return 0;

    /* 3. project onto the plane's dominant coordinate plane, then 2D circle-in-polygon */
    plane_normal = &((const real_plane3d *)global_bsp3d->planes.address)[plane_index].n;
    axis = projection_from_vector3d(plane_normal);
    axis_sign = (plane_normal->n[axis] > 0.0f);
    keep0 = global_projection3d_mappings[axis][axis_sign][0];
    keep1 = global_projection3d_mappings[axis][axis_sign][1];

    /* slide the sphere center onto the plane along the normal */
    projected[0] = plane_normal->n[0] * -plane_distance + point->n[0];
    projected[1] = plane_normal->n[1] * -plane_distance + point->n[1];
    projected[2] = plane_normal->n[2] * -plane_distance + point->n[2];
    center_2d.n[0] = projected[keep0];
    center_2d.n[1] = projected[keep1];

    vertex_count = portal->vertices.count;
    if ( vertex_count > 0 )
    {
        const real_point3d *vertex_base = (const real_point3d *)portal->vertices.address;
        for ( i = 0; i < vertex_count; ++i )
        {
            const float *vertex = vertex_base[i].n;
            vertices_2d[i].n[0] = vertex[keep0];
            vertices_2d[i].n[1] = vertex[keep1];
        }
    }

    if ( !convex_hull2d_test_circle(vertex_count, vertices_2d, &center_2d,
                __fsqrts(radius * radius - plane_distance * plane_distance)) )
        return 0;
    return 1;
}
