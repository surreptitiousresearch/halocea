/* leaf_map_build_portals_from_leaf @0x8381BEC8 — recursive descent that discovers the portals bounding one
 * leaf. For the given BSP node it visits both children; a child that is another node is recursed into (with a
 * decremented stack depth and an updated "ancestor" node), and a child that is a different leaf produces a
 * portal between the two leaves (leaf_map_build_portal_from_leaves). Which children are followed is gated by
 * (a) whether the node's splitting plane already appears on the traversal node stack, (b) for the root call,
 * the sign of the previously stacked node, and (c) for interior children, whether the leaf actually has a face
 * on this node. The "ancestor" node passed downward is either this node or the inherited ancestor depending on
 * that same face gate.
 *
 * BSP nodes are 3 ints {plane_index, back_child, front_child}; a negative child encodes a leaf as
 * (child & 0x7FFFFFFF), -1 meaning none. Leaves are 24 bytes {face_count, face_array_ptr, ...}; each face is
 * 16 bytes beginning with its node index. */

#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/bsp3d.h"
#include "headers/map_leaf.h"
#include "headers/map_leaf_face.h"

extern void leaf_map_build_portal_from_leaves(leaf_map *leaf_map, int node_index, int leaf_index0, int leaf_index1);

void leaf_map_build_portals_from_leaf(leaf_map *leaf_map, int ancestor_node_index, int leaf_index, int node_index, int16_t stack_depth)
{
    int *nodes = (int *)leaf_map->bsp->nodes.address;
    int *node = &nodes[3 * node_index];

    int expected_stack_node;
    if ( ancestor_node_index == -1 )
        expected_stack_node =
            leaf_map_globals.node_stack[leaf_map_globals.node_stack_count - stack_depth - 1];
    else
        expected_stack_node = -1;

    char plane_on_stack = 0;
    unsigned char stacked_node_negative = 0;   /* original leaves this uninitialized; only read when plane_on_stack */
    if ( leaf_map_globals.node_stack_count > 0 )
    {
        int scan = 0;
        int stacked_node;
        while ( 1 )
        {
            stacked_node = leaf_map_globals.node_stack[leaf_map_globals.node_stack_count - scan - 1];
            if ( nodes[2 * stacked_node + (stacked_node & 0x7FFFFFFF)] == *node )
                break;
            scan = (__int16)(scan + 1);
            if ( scan >= leaf_map_globals.node_stack_count )
                goto scanned;
        }
        plane_on_stack = 1;
        stacked_node_negative = stacked_node < 0;
    }
scanned:

    for ( int i = 0; i < 2; i = (__int16)(i + 1) )
    {
        /* prefer this node as the descent ancestor for the far child of the root call */
        int prefer_this_node =
            (ancestor_node_index == -1 && i != 0 && expected_stack_node < 0) ? 1 : 0;

        if ( ancestor_node_index == -1 && !i && expected_stack_node >= 0 )
            continue;

        int process_children;
        if ( !prefer_this_node )
        {
            process_children = 1;
        }
        else
        {
            /* only descend interior children the leaf actually touches through this node */
            map_leaf *leaf = &((map_leaf *)leaf_map->leaves.address)[leaf_index];
            int face_count = leaf->faces.count;
            __int16 matching_face = 0;
            if ( face_count <= 0 )
            {
                matching_face = -1;
            }
            else
            {
                map_leaf_face *faces = (map_leaf_face *)leaf->faces.address;
                __int16 face = 0;
                while ( faces[face].node_index != node_index )
                {
                    matching_face = ++face;
                    if ( face >= face_count )
                    {
                        matching_face = -1;
                        break;
                    }
                }
            }
            process_children = matching_face != -1;
        }

        if ( process_children
          && (ancestor_node_index == -1 || !plane_on_stack || stacked_node_negative != i) )
        {
            int child = node[i + 1];
            int descend_ancestor = prefer_this_node ? node_index : ancestor_node_index;
            if ( child >= 0 )
                leaf_map_build_portals_from_leaf(leaf_map, descend_ancestor, leaf_index, child, stack_depth - 1);
            else if ( child != -1 && (child & 0x7FFFFFFF) != leaf_index )
                leaf_map_build_portal_from_leaves(leaf_map, descend_ancestor, leaf_index, child);
        }
    }
}
