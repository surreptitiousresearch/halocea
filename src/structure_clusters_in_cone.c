/* structure_clusters_in_cone @0x83745018 — flood-fill the structure BSP's cluster graph starting at
 * position_cluster_index, crossing each cluster portal whose bounding sphere intersects the cone (apex
 * `position`, axis `direction`, length `distance`, half-angle given by sine/cosine). Visited clusters are
 * stamped with a per-call magic marker so each is emitted once into cluster_indices (up to
 * maximum_cluster_count). Returns the number of clusters found.
 *
 * Reconstructed against disasm: the three float args (distance/sine/cosine) shadow GPR slots, so the
 * decompiler invented phantom args (a9/a10/a11) and mis-shifted the sphere_intersects_cone3d call — the
 * real call passes (portal_center, portal_radius, position, direction, distance, sine, cosine); the
 * decompiler's `v24` arg does not exist.
 *
 * Cluster records are 104 bytes (portal_count @+92, portal-index-list pointer @+96); portal records are
 * 64 bytes (neighbor cluster ids @+0/+2, bounding sphere center @+8, radius @+20). */

#include <stdint.h>
#include "headers/structure_cluster.h"
#include "headers/structure_bsp.h"
#include "headers/cluster_portal.h"
#include "headers/structure_globals.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern structure_globals_t structure_globals;
extern uint8_t sphere_intersects_cone3d(const real_point3d *center, float radius, const real_point3d *base, const real_vector3d *direction, float distance, float sine, float cosine);

extern uint8_t structure_cluster_unmarked(int16_t cluster_index);
int structure_clusters_in_cone(int16_t position_cluster_index, const real_point3d *position, const real_vector3d *direction, float distance, float sine, float cosine, int16_t maximum_cluster_count, int16_t *cluster_indices)
{
    structure_globals.cluster_marker_initialized = 1;
    int marker = ++structure_globals.cluster_marker;
    int found_count = 0;
    structure_bsp *bsp = global_structure_bsp;

    if (structure_globals.cluster_magic_numbers[position_cluster_index] != marker)
        structure_globals.cluster_magic_numbers[position_cluster_index] = marker;

    int16_t cluster_stack[512];
    cluster_stack[0] = position_cluster_index;
    int stack_depth = 1;

    while (stack_depth > 0)
    {
        if (found_count >= maximum_cluster_count)
            break;

        char *clusters = (char *)bsp->clusters.address;
        int16_t current_cluster = cluster_stack[--stack_depth];
        cluster_indices[found_count++] = current_cluster;

        structure_cluster *cluster = &((structure_cluster *)clusters)[current_cluster];
        int portal_count = cluster->portal_indices.count;
        if (portal_count > 0)
        {
            const int16_t *portal_index_list = (const int16_t *)cluster->portal_indices.address;
            for (int i = 0; i < portal_count; i++)
            {
                int16_t portal_index = portal_index_list[i];  /* portal-index list is a packed __int16[] */
                cluster_portal *portal = &((cluster_portal *)bsp->cluster_portals.address)[portal_index];

                int neighbor_cluster = portal->cluster_indices[0];
                if (neighbor_cluster == current_cluster)
                    neighbor_cluster = portal->cluster_indices[1]; /* the cluster on the far side of the portal */
                neighbor_cluster = (int16_t)neighbor_cluster;

                /* DEVIATION: inlined copy of structure_cluster_unmarked@0x83744610 (zero-xref donor); local `marker` cache is invariant == structure_globals.cluster_marker for the life of this call, so it collapses directly to a call with cluster_index=neighbor_cluster, no arg folding needed. */
                if (structure_cluster_unmarked(neighbor_cluster))
                {
                    uint8_t in_cone = sphere_intersects_cone3d(
                        &portal->centroid, portal->bounding_radius,
                        position, direction, distance, sine, cosine);
                    marker = structure_globals.cluster_marker; /* binary reloads; value is unchanged */
                    if (in_cone)
                    {
                        if (structure_globals.cluster_magic_numbers[neighbor_cluster] != structure_globals.cluster_marker)
                        {
                            structure_globals.cluster_magic_numbers[neighbor_cluster] = structure_globals.cluster_marker;
                            marker = structure_globals.cluster_marker;
                        }
                        cluster_stack[stack_depth++] = neighbor_cluster;
                    }
                }
            }
        }
    }

    structure_globals.cluster_marker_initialized = 0;
    return found_count;
}
