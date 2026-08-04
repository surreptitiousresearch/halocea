/* bsp2d_test_point @ 0x838202A0 — descend a 2D BSP from `child_index` to the leaf (surface index)
 * containing a 2D point. At each node the point is classified against the node's 2D line
 * (a*x + b*y - c >= 0 picks the front child); leaf children are negative. Returns the surface index
 * (low 31 bits), or -1 if the point falls in the "no surface" leaf. Node (20 bytes): float a,b,c
 * then int back_child, front_child. */

#include "headers/bsp2d.h"
#include "headers/bsp2d_node.h"
#include "headers/real_point2d.h"

int bsp2d_test_point(const bsp2d *bsp, const real_point2d *point, int child_index)
{
    while ( child_index >= 0 )
    {
        float *node = (float *)&((bsp2d_node *)bsp->nodes.address)[child_index];
        int front = (node[1] * point->n[1] + node[0] * point->n[0]) - node[2] >= 0.0f;
        child_index = *(int *)&node[front + 3];
    }

    if ( child_index == -1 )
        return -1;
    return child_index & 0x7FFFFFFF;
}
