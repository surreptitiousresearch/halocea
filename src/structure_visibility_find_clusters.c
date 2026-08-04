/* structure_visibility_find_clusters @0x837C5DF8 — seed the cluster visibility flood-fill from the camera's
 * cluster. Builds a screen-space portal hull from the camera's full projection rectangle (the four screen
 * corners), traverses outward through portals from the camera cluster marking visible clusters, then for each
 * cluster recorded this pass builds its clipped frustum bounds and sub-frustum. No-op when the camera is
 * outside any cluster. */

#include <stdint.h>
#include "headers/render_globals.h"
#include "headers/render_camera.h"
#include "headers/structure_visibility_globals.h"
#include "headers/portal_hull.h"
#include "headers/real_rectangle2d.h"
#include <string.h>

#include "headers/render_camera.h"
#include "headers/render_frustum.h"
#include "headers/render_camera.h"
extern void render_frustum_get_projection_bounds(const render_frustum *frustum, real_rectangle2d *bounds);
extern void structure_visibility_traverse_cluster(int16_t cluster_index, const portal_hull *visible_region);
extern int render_camera_build_clipped_frustum_bounds(const render_camera *camera, const real_rectangle2d *clip, real_rectangle2d *frustum_bounds);
extern void render_camera_build_frustum(const render_camera *camera, const real_rectangle2d *frustum_bounds, render_frustum *frustum, uint8_t build_projection);

void structure_visibility_find_clusters(void)
{
    if (render.cluster_index == -1)
        return;

    real_rectangle2d projection_bounds;
    render_frustum_get_projection_bounds(&render.frustum, &projection_bounds);

    unsigned int visited_cluster_flags[16];   /* 512-cluster dedup bit vector */
    structure_visibility_globals.visited_cluster_flags = visited_cluster_flags;

    portal_hull screen_hull;
    screen_hull.vertex_count = 4;
    screen_hull.vertices[0].n[0] = projection_bounds.n[0];   /* (x0, y0) */
    screen_hull.vertices[0].n[1] = projection_bounds.n[2];
    screen_hull.vertices[1].n[0] = projection_bounds.n[1];   /* (x1, y0) */
    screen_hull.vertices[1].n[1] = projection_bounds.n[2];
    screen_hull.vertices[2].n[0] = projection_bounds.n[1];   /* (x1, y1) */
    screen_hull.vertices[2].n[1] = projection_bounds.n[3];
    screen_hull.vertices[3].n[0] = projection_bounds.n[0];   /* (x0, y1) */
    screen_hull.vertices[3].n[1] = projection_bounds.n[3];

    memset(visited_cluster_flags, 0, sizeof(visited_cluster_flags));

    structure_visibility_traverse_cluster(render.cluster_index, &screen_hull);

    for (int cluster = 0; cluster < render.rendered_cluster_count; cluster = (__int16)(cluster + 1))
    {
        real_rectangle2d frustum_bounds;
        render_camera_build_clipped_frustum_bounds(&render.camera, &render.rendered_clusters[cluster].clip_bounds,
                                                   &frustum_bounds);
        render_camera_build_frustum(&render.camera, &frustum_bounds, &render.rendered_clusters[cluster].frustum, 0);
    }
}
