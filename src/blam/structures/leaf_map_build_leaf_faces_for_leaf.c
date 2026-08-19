/* leaf_map_build_leaf_faces_for_leaf @0x8381C110 — build the leaf's faces from every node currently on the
 * BSP traversal stack (leaf_map_globals.node_stack), nearest node first. */

#include "headers/leaf_map.h"

extern void leaf_map_build_leaf_face_for_leaf_on_node(leaf_map *leaf_map, int leaf_index, int node);

void leaf_map_build_leaf_faces_for_leaf(leaf_map *leaf_map, int leaf_index)
{
    int node_stack_count = leaf_map_globals.node_stack_count;

    if ( node_stack_count > 0 )
    {
        int i = 0;

        do
        {
            leaf_map_build_leaf_face_for_leaf_on_node(
                leaf_map,
                leaf_index,
                leaf_map_globals.node_stack[node_stack_count - i - 1]);
            node_stack_count = leaf_map_globals.node_stack_count;
            ++i;
        } while ( i < leaf_map_globals.node_stack_count );
    }
}
