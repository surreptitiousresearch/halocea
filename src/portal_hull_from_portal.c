#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/cluster_portal.h"
#include "headers/bsp3d.h"
#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"
#include "headers/portal_hull.h"
#include "headers/render_globals.h"
#include "headers/blam_data_globals.h"

#include "headers/render_camera.h"
#include "headers/render_frustum.h"
extern int16_t portal_hull_from_points(const render_camera *camera, const render_frustum *frustum, const real_plane3d *plane, int16_t vertex_count, const real_point3d *vertices, int16_t winding, portal_hull *hull);

int16_t portal_hull_from_portal(int16_t portal_index, uint8_t direction, portal_hull *result)
{
    bsp3d *collision_bsp = (bsp3d *)global_structure_bsp->collision_bsp.address;
    cluster_portal *portal = &((cluster_portal *)global_structure_bsp->cluster_portals.address)[portal_index];

    int plane_index = portal->plane_index;
    real_plane3d *plane = &((real_plane3d *)collision_bsp->planes.address)[plane_index];
    int16_t vertex_count = portal->vertices.count;
    const real_point3d *vertices = (const real_point3d *)portal->vertices.address;

    return portal_hull_from_points(&render.camera, &render.frustum, plane, vertex_count, vertices,
            direction == 0 ? 1 : -1, result);
}
