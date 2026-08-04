/* structure_clusters_in_sphere @0x837451B8 — find the BSP structure clusters whose geometry intersects a
 * sphere, starting from the cluster the sphere's center is in. A non-positive radius degenerates to "just
 * the seed cluster". Otherwise it bumps the global cluster visit marker (so the recursive flood-fill can
 * dedupe visited clusters) and delegates to the recursive walker. Returns the number of clusters written
 * to intersected_indices.
 *
 * deviation: the DB prototype carries a phantom extra parameter. Verified against the call site
 * (objects_in_sphere) and disassembly: the real signature is (cluster_index, position, radius,
 * maximum_count, intersected_indices) — r5 is maximum_count, r6 is the output buffer. */

#include <stdint.h>
#include "headers/structure_globals.h"
#include "headers/real_point3d.h"

extern int16_t structure_clusters_in_sphere_recursive(int16_t cluster_index, const real_point3d *position, float radius, int16_t maximum_count, int16_t *intersected_indices);

int16_t structure_clusters_in_sphere(int16_t cluster_index, const real_point3d *position, float radius,
        int16_t maximum_count, int16_t *intersected_indices)
{
    int16_t result = 0;

    if ( cluster_index == -1 )
        return 0;

    if ( radius <= 0.0f )
    {
        if ( maximum_count > 0 )
        {
            intersected_indices[0] = cluster_index;
            return 1;
        }
    }
    else
    {
        structure_globals.cluster_marker_initialized = 1;
        ++structure_globals.cluster_marker;
        result = structure_clusters_in_sphere_recursive(cluster_index, position, radius,
                maximum_count, intersected_indices);
        structure_globals.cluster_marker_initialized = 0;
    }
    return result;
}
