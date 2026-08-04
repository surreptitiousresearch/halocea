/* structure_clusters_in_sphere_recursive @0x83744E70 — flood-fill the BSP cluster graph collecting every
 * cluster reachable from cluster_index through portals that the query sphere crosses. Records the current
 * cluster (if there is buffer budget), stamps it visited with the global cluster marker, then for each of
 * the cluster's portals walks into the neighbor cluster on the far side — but only if that neighbor is not
 * yet stamped this traversal and the sphere actually intersects the portal. Returns the number of clusters
 * recorded (this one plus all descendants).
 *
 * deviation: the DB prototype shows an extra parameter. On this PPC ABI the float `radius` argument reserves
 * (skips) its GPR slot, so the real C signature is 5 params: (cluster_index, position, radius, maximum_count,
 * intersected_indices). r6 = maximum_count (remaining buffer budget), r7 = output buffer.
 *
 * Cluster record (104 bytes): portal_count@92 (int[23]), portal_indices array pointer@96 (int[24]).
 * Cluster portal record (64 bytes): front cluster@0, back cluster@2 (int16 each). */

#include <stdint.h>
#include "headers/structure_cluster.h"
#include "headers/cluster_portal.h"
#include "headers/structure_bsp.h"
#include "headers/structure_globals.h"
#include "headers/real_point3d.h"

extern uint8_t sphere_intersects_cluster_portal(structure_bsp *structure, int16_t portal_index, const real_point3d *point, float radius);

extern uint8_t structure_cluster_unmarked(int16_t cluster_index);
int16_t structure_clusters_in_sphere_recursive(int16_t cluster_index, const real_point3d *position,
        float radius, int16_t maximum_count, int16_t *intersected_indices)
{
    structure_bsp *bsp = global_structure_bsp;
    structure_cluster *cluster = &((structure_cluster *)bsp->clusters.address)[cluster_index];
    __int16 *out = intersected_indices;
    __int16 remaining = (__int16)(maximum_count - 1);
    int count;
    int portal_count;

    if ( maximum_count > 0 )
    {
        *intersected_indices = cluster_index;
        out = intersected_indices + 1;
    }

    if ( structure_globals.cluster_magic_numbers[cluster_index] != structure_globals.cluster_marker )
        structure_globals.cluster_magic_numbers[cluster_index] = structure_globals.cluster_marker;

    count = 1;
    portal_count = cluster->portal_indices.count;
    if ( portal_count > 0 )
    {
        __int16 i = 0;
        do
        {
            __int16 portal_index = ((const __int16 *)cluster->portal_indices.address)[i];
            const cluster_portal *portal = &((const cluster_portal *)bsp->cluster_portals.address)[portal_index];
            int neighbor = portal->cluster_indices[0];
            if ( neighbor == cluster_index )
                neighbor = portal->cluster_indices[1];

            /* DEVIATION: inlined copy of structure_cluster_unmarked@0x83744610 (zero-xref donor); collapses directly, arg = (__int16)neighbor, no constant-folding required since both donor params/return already match host types. */
            if ( structure_cluster_unmarked((__int16)neighbor)
                 && sphere_intersects_cluster_portal(bsp, portal_index, position, radius) )
            {
                __int16 added = structure_clusters_in_sphere_recursive((__int16)neighbor, position,
                        radius, remaining, out);
                count = (__int16)(count + added);
                remaining = (__int16)(remaining - added);
                out += added;
            }
            ++i;
        }
        while ( i < portal_count );
    }
    return count;
}
