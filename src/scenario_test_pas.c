/* scenario_test_pas @0x837037B8 — tests whether two clusters' potential-audible-set (PAS) bit vectors
 * overlap, i.e. cluster_index1 is potentially audible from cluster_index0. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/blam_data_globals.h"


#include "headers/structure_bsp.h"
extern unsigned int *structure_bsp_get_cluster_pvs(structure_bsp *structure_bsp, int16_t cluster_index);
extern uint8_t bit_vector_and(int16_t count, const unsigned int *a, const unsigned int *b, unsigned int *result);

uint8_t scenario_test_pas(int16_t cluster_index0, int16_t cluster_index1)
{
    const unsigned int *pvs0 = structure_bsp_get_cluster_pvs(global_structure_bsp, cluster_index0);
    const unsigned int *pvs1 = structure_bsp_get_cluster_pvs(global_structure_bsp, cluster_index1);

    return bit_vector_and(global_structure_bsp->clusters.count, pvs0, pvs1, nullptr);
}
