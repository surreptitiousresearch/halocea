/* find_like_crossing @0x8381A990 — scans leaf_map_globals' node stack (top-down) for an entry whose bsp3d
 * node references `plane_index`, and reports which side of the plane that node's entry represents.
 *
 * DEVIATION: the node-array index expression `2*node + (node & 0x7FFFFFFF)` (DWORD units) is preserved
 * verbatim rather than simplified to `12*node` (the bsp3d_node stride used elsewhere, e.g.
 * bsp3d_test_point.c): for non-negative node indices the two are identical, but leaf_map stack entries can
 * be negative (leaf references), and the mask's effect on that path is not confirmed to reduce the same
 * way, so the original arithmetic is kept exactly rather than risk misreading it. */

#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/bsp3d.h"

uint8_t find_like_crossing(leaf_map *leaf_map, int plane_index, uint8_t *side)
{
    if ( leaf_map_globals.node_stack_count <= 0 )
        return 0;

    int levels_up = 0;
    int node;

    while ( 1 )
    {
        node = leaf_map_globals.node_stack[leaf_map_globals.node_stack_count - levels_up - 1];
        if ( *((int *)leaf_map->bsp->nodes.address + 2 * node + (node & 0x7FFFFFFF)) == plane_index )
            break;

        ++levels_up;
        if ( levels_up >= leaf_map_globals.node_stack_count )
            return 0;
    }

    *side = node < 0;
    return 1;
}
