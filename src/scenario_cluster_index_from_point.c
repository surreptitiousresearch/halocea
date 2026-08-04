/* scenario_cluster_index_from_point @0x83703660 — locate the structure-BSP cluster containing a world point by
 * descending the 3D BSP to its leaf, then reading that leaf's cluster index. Returns -1 when the point is outside
 * the BSP. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/bsp3d.h"
#include "headers/structure_bsp.h"
#include "headers/structure_leaf.h"
#include "headers/blam_data_globals.h"


extern int bsp3d_test_point(const bsp3d *bsp, int node_index, const real_point3d *point);

int16_t scenario_cluster_index_from_point(const real_point3d *point)
{
    int leaf_index = bsp3d_test_point(global_bsp3d, 0, point);
    if ( leaf_index != -1 )
        return ((structure_leaf *)global_structure_bsp->leaves.address)[leaf_index].cluster_index;
    return leaf_index;
}
