/* node_stack_pop @0x8381A8F0 */
#include "headers/leaf_map.h"

int node_stack_pop(void)
{
    int idx = (int)--leaf_map_globals.node_stack_count;
    return leaf_map_globals.node_stack[idx];
}
