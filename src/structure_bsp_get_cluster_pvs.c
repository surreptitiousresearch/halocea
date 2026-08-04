/* structure_bsp_get_cluster_pvs @ 0x837B47A0 — pointer to a cluster's potentially-visible-set bit
 * vector. Each cluster's PVS is ceil(cluster_count/32) dwords (one bit per cluster), packed back to
 * back in cluster_data; this indexes the row for `cluster_index`. */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/structure_bsp.h"

unsigned int *structure_bsp_get_cluster_pvs(structure_bsp *structure_bsp, int16_t cluster_index)
{
    return (unsigned int *)((char *)structure_bsp->cluster_data.address
                            + 4 * BIT_VECTOR_SIZE_IN_LONGS(structure_bsp->clusters.count) * cluster_index);
}
