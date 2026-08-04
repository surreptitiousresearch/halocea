/* structure_visibility_traverse_cluster @0x837C57D8 — the recursive portal flood-fill that determines which
 * clusters are visible from the camera cluster. Records the cluster as rendered (expanding its 2D clip bounds
 * by the incoming visible region), then for each cluster portal: rejects the neighbor if out of range, already
 * being visited, or outside the camera PVS; projects the portal polygon into screen space; and recurses into
 * the neighbor through the portal hull clipped against the current visible region. Portals that fill the view
 * (or overflow the clip) recurse with the full incoming region; portals entirely beyond the far plane (when no
 * sky is visible) are skipped. The visiting mark is cleared on the way out so a cluster can be reached again
 * through a different portal path. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/collision_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/cluster_portal.h"
#include "headers/render_globals.h"
#include "headers/render_camera.h"
#include "headers/structure_visibility_globals.h"
#include "headers/rendered_cluster.h"
#include "headers/portal_hull.h"
#include "headers/real_rectangle2d.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/blam_data_globals.h"

#include "headers/render_camera.h"
#include "headers/render_frustum.h"
extern unsigned int *structure_bsp_get_cluster_pvs(structure_bsp *structure_bsp, int16_t cluster_index);
extern int16_t portal_hull_from_points(const render_camera *camera, const render_frustum *frustum, const real_plane3d *plane, int16_t vertex_count, const real_point3d *vertices, int16_t winding, portal_hull *hull);
extern int16_t convex_hull2d_intersect(int16_t p_count, const real_point2d *p, int q_count, const real_point2d *q, int16_t maximum_count, real_point2d *result, float epsilon);

void structure_visibility_traverse_cluster(int16_t cluster_index, const portal_hull *visible_region)
{
    structure_bsp *bsp = global_structure_bsp;
    structure_cluster *cluster = (structure_cluster *)bsp->clusters.address + cluster_index;
    int word = cluster_index >> 5;
    unsigned int bit = 1 << (cluster_index & 0x1F);
    unsigned int *cluster_pvs = structure_bsp_get_cluster_pvs(bsp, render.cluster_index);

    structure_visibility_globals.visited_cluster_flags[word] |= bit;

    rendered_cluster *entry;
    if ((render.visible_cluster_flags[word] & bit) != 0)
    {
        entry = &render.rendered_clusters[structure_visibility_globals.rendered_cluster_indices[cluster_index]];
    }
    else
    {
        __int16 slot = render.rendered_cluster_count;
        structure_visibility_globals.rendered_cluster_indices[cluster_index] = slot;
        render.rendered_cluster_count = slot + 1;
        entry = &render.rendered_clusters[slot];
        entry->cluster_index = cluster_index;
        entry->clip_bounds = *global_null_rectangle2d;
    }

    float *clip = entry->clip_bounds.n;   /* [min_x, max_x, min_y, max_y] */
    render.visible_cluster_flags[word] |= bit;

    /* expand this cluster's clip bounds to enclose the incoming visible region */
    const real_point2d *vertices = visible_region->vertices;
    for (int i = 0; i < visible_region->vertex_count; i = (__int16)(i + 1))
    {
        if (clip[0] > vertices->n[0]) clip[0] = vertices->n[0];
        if (clip[1] < vertices->n[0]) clip[1] = vertices->n[0];
        if (clip[2] > vertices->n[1]) clip[2] = vertices->n[1];
        if (clip[3] < vertices->n[1]) clip[3] = vertices->n[1];
        ++vertices;
    }

    int portal_count = cluster->portal_indices.count;
    if (portal_count > 0)
    {
        portal_hull projected_portal;
        portal_hull clipped_hull;
        int portal_iterator = 0;
        __int16 neighbor;

        while (1)
        {
            /* portal_indices holds __int16 indices; the decompiler's <<6 is the 64-byte cluster_portal stride */
            int portal_index = ((const __int16 *)cluster->portal_indices.address)[portal_iterator];
            cluster_portal *portal = &((cluster_portal *)bsp->cluster_portals.address)[portal_index];
            neighbor = portal->cluster_indices[cluster_index == portal->cluster_indices[0]];

            if (neighbor < 0)
                goto next_portal;
            if (neighbor >= bsp->clusters.count)
                goto next_portal;

            int neighbor_word = neighbor >> 5;
            unsigned int neighbor_bit = 1 << (neighbor & 0x1F);
            if ((structure_visibility_globals.visited_cluster_flags[neighbor_word] & neighbor_bit) != 0
                || (cluster_pvs[neighbor_word] & neighbor_bit) == 0)
                goto next_portal;

            {
                int plane_index = portal->plane_index;
                /* dword 4 = byte 16 = collision_bsp.bsp3d.planes.address (DB: bsp3d @0, bsp3d.planes tag_block @12, .address @4); real_plane3d = 16B */
                const collision_bsp *collision = (const collision_bsp *)bsp->collision_bsp.address;
                const real_plane3d *plane =
                    &((const real_plane3d *)collision->bsp3d.planes.address)[plane_index];
                __int16 hull_result = portal_hull_from_points(&render.camera, &render.frustum, plane,
                    portal->vertices.count,
                    (const real_point3d *)portal->vertices.address,
                    (cluster_index != portal->cluster_indices[0]) ? 1 : -1, &projected_portal);

                if (hull_result == 2)
                {
                    /* portal fills the view — recurse with the full incoming region */
                    structure_visibility_traverse_cluster(neighbor, visible_region);
                    goto next_portal;
                }
                if (hull_result)
                    goto next_portal;       /* portal not visible */

                if (!render.visible_sky_model)
                {
                    __int16 portal_vertex_count = (__int16)portal->vertices.count;
                    unsigned char any_vertex_near = 0;
                    if (portal_vertex_count > 0)
                    {
                        const real_point3d *portal_vertices = (const real_point3d *)portal->vertices.address;
                        int k = 0;
                        any_vertex_near = 1;
                        while ((render.camera.forward.n[0]
                                    * (portal_vertices[k].n[0] - render.camera.position.n[0])
                                + (render.camera.forward.n[2]
                                       * (portal_vertices[k].n[2] - render.camera.position.n[2])
                                   + render.camera.forward.n[1]
                                       * (portal_vertices[k].n[1] - render.camera.position.n[1])))
                               > render.camera.z_far)
                        {
                            ++k;
                            if (k >= portal_vertex_count)
                            {
                                any_vertex_near = 0;
                                break;
                            }
                        }
                    }
                    if (!any_vertex_near)
                        goto next_portal;   /* whole portal is beyond the far plane */
                }

                int intersection_count = convex_hull2d_intersect(visible_region->vertex_count,
                    visible_region->vertices, projected_portal.vertex_count, projected_portal.vertices,
                    256, clipped_hull.vertices, 0.000099999997);
                clipped_hull.vertex_count = intersection_count;
                if (intersection_count > 0)
                {
                    structure_visibility_traverse_cluster(neighbor, &clipped_hull);
                    goto next_portal;
                }
                if (intersection_count == -1)
                {
                    /* clip overflowed — recurse with the full incoming region */
                    structure_visibility_traverse_cluster(neighbor, visible_region);
                    goto next_portal;
                }
            }

        next_portal:
            ++portal_iterator;
            if (portal_iterator >= portal_count)
                goto done;
        }
    }

done:
    structure_visibility_globals.visited_cluster_flags[word] &= ~bit;
}
