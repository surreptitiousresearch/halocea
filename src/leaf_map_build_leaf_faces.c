/* leaf_map_build_leaf_faces @0x8381C268 — recursively descend a BSP node, pushing each visited node onto
 * leaf_map_globals' traversal stack (back child tagged with the sign bit 0x80000000, front child untagged
 * — same convention as leaf_map_build_portals.c), recursing into internal children. For a leaf child
 * (node index -1 is the "outside" leaf and is skipped), build a leaf face against every node currently on
 * the traversal stack, from most-recently-pushed down to the bottom. Node layout (12 bytes): [0] plane
 * index, [1] back child, [2] front child (per bsp3d_test_point). */

#include "headers/leaf_map.h"
#include "headers/bsp3d.h"
#include "headers/bsp3d_node.h"

extern void leaf_map_build_leaf_face_for_leaf_on_node(leaf_map *leaf_map, int leaf_index, int node);

void leaf_map_build_leaf_faces(leaf_map *leaf_map, unsigned int node_index)
{
    const bsp3d_node *node = &((const bsp3d_node *)leaf_map->bsp->nodes.address)[node_index];

    for ( int child_slot = 0; child_slot < 2; child_slot++ )
    {
        int stack_position = leaf_map_globals.node_stack_count;
        leaf_map_globals.node_stack[stack_position] = child_slot == 0 ? (node_index | 0x80000000) : node_index;
        int child_index = node->child_indices[child_slot];   /* [0] = back child, [1] = front child */
        int stack_count_after_push = ++leaf_map_globals.node_stack_count;

        if ( child_index >= 0 )
        {
            leaf_map_build_leaf_faces(leaf_map, child_index);
            stack_count_after_push = leaf_map_globals.node_stack_count;
        }
        else if ( child_index != -1 )
        {
            int stack_top = stack_count_after_push;
            if ( stack_count_after_push > 0 )
            {
                for ( int depth = 0; depth < leaf_map_globals.node_stack_count; depth++ )
                {
                    leaf_map_build_leaf_face_for_leaf_on_node(leaf_map, child_index,
                        leaf_map_globals.node_stack[stack_top - depth - 1]);
                    stack_count_after_push = leaf_map_globals.node_stack_count;
                    stack_top = leaf_map_globals.node_stack_count;
                }
            }
        }

        leaf_map_globals.node_stack_count = stack_count_after_push - 1;
    }
}
