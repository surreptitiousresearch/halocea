/* find_like_crossing @0x8381A990 — scans leaf_map_globals' node stack (top-down) for an entry whose bsp3d
 * node references `plane_index`, and reports which side of the plane that node's entry represents.
 *
 * DEVIATION: the node-array index expression `2*node + (node & 0x7FFFFFFF)` (int units) is exactly
 * `3 * (node & 0x7FFFFFFF)` in 32-bit arithmetic — 2*node differs from 2*(node & 0x7FFFFFFF) only by
 * 2*2^31 = 2^32, which wraps to 0 — so it is bsp3d_node indexing by the MASKED index, not by `node`.
 * Stack entries carry the traversal side in bit 31 (that is what `*side` reports), so the mask is
 * load-bearing and the two spellings differ for the negative entries this loop really does see.
 * Disasm 0x8381A9C0-A9D0: clrlwi m, n, 1 / slwi 2n / add / slwi 2 = 12*m. */

#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/bsp3d.h"
#include "headers/bsp3d_node.h"

uint8_t find_like_crossing(leaf_map *leaf_map, int plane_index, uint8_t *side)
{
    if ( leaf_map_globals.node_stack_count <= 0 )
        return 0;

    const bsp3d_node *nodes = (const bsp3d_node *)leaf_map->bsp->nodes.address;
    int levels_up = 0;
    int node;

    while ( 1 )
    {
        node = leaf_map_globals.node_stack[leaf_map_globals.node_stack_count - levels_up - 1];
        if ( nodes[node & 0x7FFFFFFF].plane_index == plane_index )
            break;

        ++levels_up;
        if ( levels_up >= leaf_map_globals.node_stack_count )
            return 0;
    }

    *side = node < 0;
    return 1;
}
