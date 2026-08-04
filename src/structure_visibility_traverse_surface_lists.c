/* structure_visibility_traverse_surface_lists @0x837C4948 — mark visible environment surfaces using each
 * cluster's flat, lightmap-grouped surface list (the path taken when the BSP has no subcluster tree). The
 * list is a sequence of groups, each a 3-int header [lightmap_index, vertex_page, surface_count] followed by
 * that many surface indices. For each not-yet-marked surface, its triangle's three lightmap vertices are
 * frustum-tested and, if visible, the surface is flagged. Stops at the surface cap (0x4000). */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/structure_surface.h"
#include "headers/structure_lightmap.h"
#include "headers/structure_material.h"
#include "headers/render_globals.h"
#include "headers/render_frustum.h"
#include "headers/rendered_cluster.h"
#include "headers/render_limits.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

extern uint8_t render_frustum_triangle_visible(const render_frustum *frustum, const real_point3d *p0, const real_point3d *p1, const real_point3d *p2);

void structure_visibility_traverse_surface_lists(structure_bsp *structure)
{
    for (int rc = 0; rc < render.rendered_cluster_count; rc = (__int16)(rc + 1))
    {
        structure_cluster *cluster =
            (structure_cluster *)structure->clusters.address + render.rendered_clusters[rc].cluster_index;
        int *surface_list = (int *)cluster->surface_indices.address;
        render_frustum *frustum = (structures_use_pvs_for_vs || render.cluster_index == -1)
            ? &render.frustum
            : &render.rendered_clusters[rc].frustum;

        int total_count = cluster->surface_indices.count;
        int consumed = 0;
        while (consumed < total_count)
        {
            int lightmap_index = surface_list[0];
            int vertex_page = surface_list[1];
            int group_end = surface_list[2] + consumed + 3;
            consumed += 3;
            surface_list += 3;

            /* materials.address is structure_material[] (256 bytes each); vertex_page indexes it */
            structure_lightmap *lightmap =
                (structure_lightmap *)structure->lightmaps.address + lightmap_index;
            structure_material *vertex_group =
                (structure_material *)lightmap->materials.address + vertex_page;
            int vertex_buffer = (int)vertex_group->compressed_vertex_data.address;

            for (; consumed < group_end; ++consumed)
            {
                if (render.environment_surface_count >= MAXIMUM_RENDERED_ENVIRONMENT_SURFACES)
                    break;

                int surface_index = surface_list[0];
                int word = surface_list[0] >> 5;
                unsigned int bit = 1 << (surface_list[0] & 0x1F);
                ++surface_list;

                if ((render.environment_surface_flags[word] & bit) == 0)
                {
                    /* the 6-byte surface holds its three vertex indices; each lightmap vertex is 32 bytes
                     * (vertex_index << 5) past the group's vertex buffer */
                    unsigned __int16 *triangle =
                        ((structure_surface *)structure->surfaces.address)[surface_index].vertex_indices;
                    if (render_frustum_triangle_visible(frustum,
                            (const real_point3d *)((triangle[0] << 5) + vertex_buffer),
                            (const real_point3d *)((triangle[1] << 5) + vertex_buffer),
                            (const real_point3d *)((triangle[2] << 5) + vertex_buffer)))
                    {
                        render.environment_surface_flags[word] |= bit;
                        ++render.environment_surface_count;
                    }
                }
            }
        }
    }
}
