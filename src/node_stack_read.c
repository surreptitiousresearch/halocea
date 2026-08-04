/* node_stack_read @0x8381A918 — reads the node `levels_up` entries below the top of leaf_map_globals'
 * node stack (levels_up == 0 reads the current top). */

#include <stdint.h>
#include "headers/leaf_map.h"

int node_stack_read(int16_t levels_up)
{
    return leaf_map_globals.node_stack[leaf_map_globals.node_stack_count - levels_up - 1];
}
