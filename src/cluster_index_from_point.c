#include "headers/structure_bsp.h"
#include "headers/structure_leaf.h"
#include "headers/real_point3d.h"
#include "headers/bsp3d.h"

extern int bsp3d_test_point(const bsp3d *bsp, int node_index, const real_point3d *point);

int cluster_index_from_point(const structure_bsp *structure_bsp, const real_point3d *point)
{
    int leaf_index = bsp3d_test_point((const bsp3d *)structure_bsp->collision_bsp.address, 0, point);

    if ( leaf_index != -1 )
        return ((structure_leaf *)structure_bsp->leaves.address)[leaf_index].cluster_index;
    return -1;
}
