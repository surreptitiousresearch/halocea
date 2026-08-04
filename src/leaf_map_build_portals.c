/* leaf_map_build_portals @0x8381C180 — recursively descend a BSP node, pushing each visited node onto
 * leaf_map_globals' traversal stack (the back child tagged with the sign bit 0x80000000, the front child
 * untagged — leaf_map_build_portals_from_leaf strips this tag via & 0x7FFFFFFF), recursing into internal
 * children and building portals from leaf children (node index -1 is the "outside" leaf and is skipped
 * entirely). Node layout (12 bytes): [0] plane index, [1] back child, [2] front child (per bsp3d_test_point). */

#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/bsp3d.h"
#include "headers/bsp3d_node.h"

extern void leaf_map_build_portals_from_leaf(leaf_map *leaf_map, int ancestor_node_index, int leaf_index, int node_index, int16_t stack_depth);

void leaf_map_build_portals(leaf_map *leaf_map, int node_index)
{
    const bsp3d_node *node = &((const bsp3d_node *)leaf_map->bsp->nodes.address)[node_index];

    for ( int child_slot = 0; child_slot < 2; child_slot++ )
    {
        int stack_position = leaf_map_globals.node_stack_count;
        leaf_map_globals.node_stack[stack_position] = child_slot == 0 ? (node_index | 0x80000000) : node_index;
        int child_index = node->child_indices[child_slot];   /* [0] = back child, [1] = front child */
        leaf_map_globals.node_stack_count = stack_position + 1;

        if ( child_index >= 0 )
        {
            leaf_map_build_portals(leaf_map, child_index);
        }
        else if ( child_index != -1 )
        {
            leaf_map_build_portals_from_leaf(leaf_map, -1, child_index & 0x7FFFFFFF, 0, stack_position);
        }

        leaf_map_globals.node_stack_count = stack_position;
    }
}
