/* data_direction_to_string @0x83763D38 — indexes the net-graph data-direction string table (see
 * net_graph.c's use of the same globals). */

#include "headers/net_graph.h"

extern const char *const data_direction_strings[];

const char * data_direction_to_string(const net_graph_data_direction direction)
{
    return data_direction_strings[direction];
}
