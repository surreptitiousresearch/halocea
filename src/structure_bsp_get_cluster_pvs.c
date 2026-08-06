/* structure_bsp_get_cluster_pvs @ 0x837B47A0 — pointer to a cluster's potentially-visible-set bit
 * vector. Each cluster's PVS is ceil(cluster_count/32) dwords (one bit per cluster), packed back to
 * back in cluster_data; this indexes the row for `cluster_index`. */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/structure_bsp.h"

unsigned int *structure_bsp_get_cluster_pvs(structure_bsp *structure_bsp, int16_t cluster_index)
{
    /* DEVIATION: the `4 *` byte scale is the decompiler's lowering of typed pointer arithmetic —
     * disasm computes the row in longs and scales it once (slwi r11,r6,2 @0x837B47B8), i.e. the
     * multiplier IS sizeof(unsigned int). Indexed in element units; the row LENGTH stays runtime. */
    return (unsigned int *)structure_bsp->cluster_data.address
           + BIT_VECTOR_SIZE_IN_LONGS(structure_bsp->clusters.count) * cluster_index;
}
