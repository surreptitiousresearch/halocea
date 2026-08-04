#include "headers/leaf_map.h"

int node_stack_pop(void)
{
    int idx = (int)--leaf_map_globals.node_stack_count;
    return leaf_map_globals.node_stack[idx];
}
