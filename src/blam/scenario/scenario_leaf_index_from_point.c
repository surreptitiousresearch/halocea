/* scenario_leaf_index_from_point @ 0x83703648 — BSP3D leaf index containing a world point (in the
 * active scenario structure), or -1 if outside. */

#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

#include "headers/bsp3d.h"
extern int bsp3d_test_point(const bsp3d *bsp, int node_index, const real_point3d *point);

int scenario_leaf_index_from_point(const real_point3d *point)
{
    return bsp3d_test_point(global_bsp3d, 0, point);
}
