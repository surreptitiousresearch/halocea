/* scenario_location_from_point @ 0x83704210 — resolve the full BSP location (leaf + cluster) of a
 * world point in the active scenario structure, clearing the bonus field. */

#include <stdint.h>
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/structure_bsp.h"
#include "headers/structure_leaf.h"
#include "headers/blam_data_globals.h"

#include "headers/bsp3d.h"
extern int bsp3d_test_point(const bsp3d *bsp, int node_index, const real_point3d *point);

void scenario_location_from_point(location *location, const real_point3d *point)
{
    int leaf_index = bsp3d_test_point(global_bsp3d, 0, point);
    int16_t cluster_index;

    location->leaf_index = leaf_index;
    if ( leaf_index == -1 )
        cluster_index = -1;
    else
        cluster_index = ((structure_leaf *)global_structure_bsp->leaves.address)[leaf_index].cluster_index;
    location->cluster_index = cluster_index;
    location->bonus = 0;
}
