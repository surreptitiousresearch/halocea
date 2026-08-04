/* bsp2d_test_pill_recursive @0x837E2378 — descend the 2D BSP of one collision leaf, testing the projected swept
 * pill against it. At a leaf child (negative index) it defers to collision_surface_test_pill. At an interior node
 * it computes the projected pill's signed distance to the node line at both the sweep start and end, and recurses
 * into whichever side(s) the radius-expanded pill can reach. Returns TRUE on the first contact. */

#include <stdint.h>
#include "headers/test_pill_data.h"
#include "headers/bsp2d_node.h"

extern uint8_t collision_surface_test_pill(test_pill_data *data, int surface_index);

uint8_t bsp2d_test_pill_recursive(test_pill_data *data, int child_index)
{
    if ( child_index < 0 )
    {
        if ( collision_surface_test_pill(data, child_index & 0x7FFFFFFF) )
            return 1;
        return 0;
    }

    const float *node = (const float *)&((const bsp2d_node *)data->bsp->bsp2d.nodes.address)[child_index];
    float radius_plus = (data->radius + 0.00012207031f);

    float distance_start = (((data->p2d.n[1] * node[1]) + (data->p2d.n[0] * node[0]))
                                 - node[2]);
    float distance_end = (((data->v2d.n[1] * node[1]) + (data->v2d.n[0] * node[0]))
                               + distance_start);

    char reaches_back = (distance_start <= radius_plus || distance_end <= radius_plus);
    float radius_minus = (-data->radius - 0.00012207031f);
    char reaches_front = (distance_start >= radius_minus || distance_end >= radius_minus);

    const int *children = (const int *)node;
    if ( (reaches_back && bsp2d_test_pill_recursive(data, children[3]))
      || (reaches_front && bsp2d_test_pill_recursive(data, children[4])) )
    {
        return 1;
    }
    return 0;
}
