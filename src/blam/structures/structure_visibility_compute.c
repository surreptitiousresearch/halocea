/* structure_visibility_compute @0x837C5F08 — compute the set of visible BSP clusters/surfaces for the current
 * camera. Clears the per-frame visibility bit vectors (all-visible when the camera is outside any cluster),
 * finds the clusters reachable from the camera, and — when PVS-for-visibility is enabled — overwrites the
 * visible set with the camera cluster's precomputed PVS and rebuilds the rendered-cluster list (recording each
 * visible cluster's slot and its projection bounds). Finally walks either the subcluster tree or the flat
 * surface lists depending on the BSP's cluster-data layout. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/render_globals.h"
#include "headers/structure_visibility_globals.h"
#include "headers/rendered_cluster.h"
#include <string.h>
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"


extern void structure_visibility_find_clusters(void);
extern unsigned int *structure_bsp_get_cluster_pvs(structure_bsp *structure_bsp, int16_t cluster_index);
extern void render_frustum_get_projection_bounds(const render_frustum *frustum, real_rectangle2d *bounds);
extern void structure_visibility_traverse_subclusters(structure_bsp *structure);
extern void structure_visibility_traverse_surface_lists(structure_bsp *structure);

void structure_visibility_compute(void)
{
    structure_bsp *bsp = global_structure_bsp;

    int cluster_fill = (render.cluster_index == -1) ? -1 : 0;
    memset(render.visible_cluster_flags, cluster_fill, 4 * BIT_VECTOR_SIZE_IN_LONGS(bsp->clusters.count));
    render.environment_surface_count = 0;
    memset(render.environment_surface_flags, 0, 4 * BIT_VECTOR_SIZE_IN_LONGS(bsp->surfaces.count));
    render.rendered_cluster_count = 0;
    structure_visibility_find_clusters();

    if (structures_use_pvs_for_vs)
    {
        render.rendered_cluster_count = 0;
        int pvs_word_count = BIT_VECTOR_SIZE_IN_LONGS(bsp->clusters.count);
        unsigned int *cluster_pvs = structure_bsp_get_cluster_pvs(bsp, render.cluster_index);
        memcpy(render.visible_cluster_flags, cluster_pvs, 4 * pvs_word_count);

        for (int cluster = 0; cluster < bsp->clusters.count; cluster = (int16_t)(cluster + 1))
        {
            if (BIT_VECTOR_TEST_FLAG(render.visible_cluster_flags, cluster))
            {
                int16_t slot = render.rendered_cluster_count;
                structure_visibility_globals.rendered_cluster_indices[cluster] = slot;
                render.rendered_cluster_count = slot + 1;
                rendered_cluster *entry = &render.rendered_clusters[slot];
                entry->cluster_index = cluster;
                render_frustum_get_projection_bounds(&render.frustum, &entry->clip_bounds);
            }
        }
    }

    /* dword 13 = byte 52 into clusters[0] = structure_cluster.subclusters.count (DB: structure_cluster.subclusters tag_block @52, tag_block.count @0) */
    if (((const structure_cluster *)bsp->clusters.address)->subclusters.count)
    {
        structure_visibility_traverse_subclusters(bsp);
    }
    else
    {
        if (!once_1)
            once_1 = 1;
        structure_visibility_traverse_surface_lists(bsp);
    }
}
