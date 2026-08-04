/* bsp3d_test_point @ 0x837EA1D0 — descend a 3D BSP from `node_index` to find the leaf containing a
 * point. At each node the point is classified against the node's plane (signed distance >= 0 picks
 * the front child, < 0 the back child); leaf children are negative. Returns the leaf index (low 31
 * bits), or -1 if the point falls in the "outside" leaf (encoded as all-ones). Node (12 bytes):
 * [0] plane index, [1] back child, [2] front child. */

#include "headers/bsp3d.h"
#include "headers/bsp3d_node.h"
#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"

int bsp3d_test_point(const bsp3d *bsp, int node_index, const real_point3d *point)
{
    do
    {
        int *node = (int *)&((bsp3d_node *)bsp->nodes.address)[node_index];
        float *plane = (float *)&((real_plane3d *)bsp->planes.address)[node[0]];
        int front = (plane[0] * point->n[0] + (plane[1] * point->n[1] + plane[2] * point->n[2]))
                    - plane[3] >= 0.0f;
        node_index = node[front + 1];
    }
    while ( node_index >= 0 );

    if ( node_index == -1 )
        return -1;
    return node_index & 0x7FFFFFFF;
}
