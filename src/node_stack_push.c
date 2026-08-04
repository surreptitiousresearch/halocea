/* node_stack_push @0x8381A8C8 — pushes a BSP traversal node index onto leaf_map_globals' node stack. */

#include "headers/leaf_map.h"

void node_stack_push(int node)
{
    leaf_map_globals.node_stack[leaf_map_globals.node_stack_count++] = node;
}
