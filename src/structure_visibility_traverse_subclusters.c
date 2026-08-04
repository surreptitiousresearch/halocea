/* structure_visibility_traverse_subclusters @0x837C4768 — mark the visible environment surfaces using the
 * BSP subcluster tree. For each rendered cluster, each of its subclusters whose AABB passes the cluster's
 * frustum has all of its (not-yet-marked) surfaces flagged in render.environment_surface_flags. The frustum
 * used is the cluster's own sub-frustum unless PVS visibility is active or the camera is outside any cluster
 * (then the main frustum). Stops once the surface cap (0x4000) is reached. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/structure_subcluster.h"
#include "headers/render_globals.h"
#include "headers/render_frustum.h"
#include "headers/rendered_cluster.h"
#include "headers/real_rectangle3d.h"
#include "headers/blam_data_globals.h"
#include "headers/render_limits.h"

#include "headers/real_rectangle3d.h"
extern int16_t render_frustum_cube_visible(const render_frustum *frustum, const real_rectangle3d *bounds, uint8_t test_frustum_against_cube);

void structure_visibility_traverse_subclusters(structure_bsp *structure)
{
    for (int rc = 0; rc < render.rendered_cluster_count; rc = (__int16)(rc + 1))
    {
        if (render.environment_surface_count >= MAXIMUM_RENDERED_ENVIRONMENT_SURFACES)
            break;

        rendered_cluster *entry = &render.rendered_clusters[rc];
        structure_cluster *cluster = (structure_cluster *)structure->clusters.address + entry->cluster_index;
        render_frustum *frustum = (structures_use_pvs_for_vs || render.cluster_index == -1)
            ? &render.frustum
            : &entry->frustum;

        int subcluster_count = cluster->subclusters.count;
        for (int sci = 0; sci < subcluster_count; sci = (__int16)(sci + 1))
        {
            if (render.environment_surface_count >= MAXIMUM_RENDERED_ENVIRONMENT_SURFACES)
                break;

            structure_subcluster *subcluster = (structure_subcluster *)cluster->subclusters.address + sci;
            if (!render_frustum_cube_visible(frustum, &subcluster->world_bounds, 0))
                continue;

            int *surface_list = (int *)subcluster->surface_indices.address;
            int surface_count = subcluster->surface_indices.count;
            for (int k = 0; k < surface_count; ++k)
            {
                int surface_index = surface_list[k];
                int word = surface_index >> 5;
                unsigned int bit = 1 << (surface_index & 0x1F);
                if ((render.environment_surface_flags[word] & bit) == 0)
                {
                    if (render.environment_surface_count >= MAXIMUM_RENDERED_ENVIRONMENT_SURFACES)
                        break;
                    render.environment_surface_flags[word] |= bit;
                    ++render.environment_surface_count;
                }
            }
        }
    }
}
