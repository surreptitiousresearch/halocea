/* data_type_to_string @0x83763D20 — indexes the net-graph data-type string table (see net_graph.c's use
 * of the same globals). */

#include "headers/net_graph.h"

extern const char *const data_type_strings[];

const char * data_type_to_string(const net_graph_data_type type)
{
    return data_type_strings[type];
}
