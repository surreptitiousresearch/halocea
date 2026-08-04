/* scenario_test_pvs @0x83703758 — true if cluster_index1 is in the potentially-visible-set bit vector of
 * cluster_index0 (one bit per cluster, 32 clusters per dword). */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/structure_bsp.h"
#include "headers/blam_data_globals.h"

#include "headers/structure_bsp.h"
extern unsigned int *structure_bsp_get_cluster_pvs(structure_bsp *structure_bsp, int16_t cluster_index);

uint8_t scenario_test_pvs(int16_t cluster_index0, int16_t cluster_index1)
{
    return BIT_VECTOR_TEST_FLAG(structure_bsp_get_cluster_pvs(global_structure_bsp, cluster_index0), cluster_index1);
}
