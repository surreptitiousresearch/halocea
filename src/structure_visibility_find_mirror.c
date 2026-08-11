/* structure_visibility_find_mirror @0x837C5268 — search the clusters visible from the camera's cluster for a
 * mirror (reflective) surface inside the view frustum. Starting from the camera cluster's PVS bit vector, each
 * visible cluster's mirror list is walked: a portal hull is built from each mirror's screen-projected polygon,
 * and a mirror counts as visible if the projection is fully inside the frustum (portal result 2) or its hull
 * overlaps the frustum's projection-bounds quad. The last visible mirror found fills in the result: plane,
 * cluster, and (for index-of-refraction shaders) the refraction index and depth.
 *
 * The mirror's shader tag is looked up to read shader type 3 (index_of_refraction) parameters; other shader
 * types leave those zero. Returns 1 if any mirror was found. */

#include <stdint.h>
#include "headers/render_camera.h"
#include "headers/render_frustum.h"
#include "headers/render_mirror.h"
#include "headers/render_globals.h"
#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/structure_mirror.h"
#include "headers/shader.h"
#include "headers/shader_environment.h"
#include "headers/portal_hull.h"
#include "headers/real_rectangle2d.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/global_tag_instances.h"
#include "headers/shader_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_plane3d.h"
extern void render_frustum_get_projection_bounds(const render_frustum *frustum, real_rectangle2d *bounds);
extern unsigned int *structure_bsp_get_cluster_pvs(structure_bsp *structure_bsp, int16_t cluster_index);
extern int16_t portal_hull_from_points(const render_camera *camera, const render_frustum *frustum, const real_plane3d *plane, int16_t vertex_count, const real_point3d *vertices, int16_t winding, portal_hull *hull);
extern int16_t convex_hull2d_intersect(int16_t p_count, const real_point2d *p, int q_count, const real_point2d *q, int16_t maximum_count, real_point2d *result, float epsilon);

uint8_t structure_visibility_find_mirror(const render_camera *camera, const render_frustum *frustum, render_mirror *result)
{
    structure_bsp *structure = global_structure_bsp;
    uint8_t found = 0;

    if (render.cluster_index == -1)
        return 0;

    real_rectangle2d projection_bounds;
    render_frustum_get_projection_bounds(frustum, &projection_bounds);

    real_point2d projection_quad[4];
    projection_quad[0].n[0] = projection_bounds.n[0];
    projection_quad[0].n[1] = projection_bounds.n[2];
    projection_quad[1].n[0] = projection_bounds.n[1];
    projection_quad[1].n[1] = projection_bounds.n[2];
    projection_quad[2].n[0] = projection_bounds.n[1];
    projection_quad[2].n[1] = projection_bounds.n[3];
    projection_quad[3].n[0] = projection_bounds.n[0];
    projection_quad[3].n[1] = projection_bounds.n[3];

    unsigned int *cluster_pvs = structure_bsp_get_cluster_pvs(structure, render.cluster_index);
    portal_hull hull;
    real_point2d clipped_hull[257];

    for (int16_t cluster_index = 0; cluster_index < structure->clusters.count; ++cluster_pvs)
    {
        unsigned int pvs_word = *cluster_pvs;
        if (pvs_word == 0)
        {
            cluster_index += 32;
            continue;
        }

        int bit = 0;
        do
        {
            if (cluster_index >= structure->clusters.count)
                break;

            if (((1 << bit) & pvs_word) != 0)
            {
                structure_cluster *cluster = (structure_cluster *)structure->clusters.address + cluster_index;
                int mirror_count = cluster->mirrors.count;
                if (mirror_count > 0)
                {
                    int mirror_index = 0;
                    do
                    {
                        structure_mirror *mirror = (structure_mirror *)cluster->mirrors.address + mirror_index;
                        int16_t hull_result = portal_hull_from_points(
                            camera, frustum, &mirror->plane, mirror->points.count,
                            (const real_point3d *)mirror->points.address, 1, &hull);

                        if ((!hull_result
                             && convex_hull2d_intersect(4, projection_quad, hull.vertex_count, hull.vertices,
                                                        256, clipped_hull, 0.000099999997f) != 0)
                            || hull_result == 2)
                        {
                            shader_environment *mirror_shader = TAG_GET(shader_environment, mirror->shader.index);
                            if (mirror_shader->shader.type == _shader_type_environment)
                            {
                                result->index_of_refraction = mirror_shader->environment.reflection.mirror_index_of_refraction;
                                result->depth = mirror_shader->environment.reflection.mirror_depth;
                            }
                            else
                            {
                                result->index_of_refraction = 0.0f;
                                result->depth = 0.0f;
                            }
                            found = 1;
                            result->plane.n.n[0] = mirror->plane.n.n[0];
                            result->plane.n.n[1] = mirror->plane.n.n[1];
                            result->plane.n.n[2] = mirror->plane.n.n[2];
                            result->plane.d = mirror->plane.d;
                            result->cluster_index = cluster_index;
                        }
                        mirror_index = (int16_t)(mirror_index + 1);
                    }
                    while (mirror_index < cluster->mirrors.count);
                }
            }
            ++cluster_index;
        }
        while (++bit < 32);
    }

    return found;
}
