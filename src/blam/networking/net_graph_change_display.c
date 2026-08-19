/* net_graph_change_display @0x83764320 — console command: reconfigure the on-screen network graph to a
 * new data type ("bytes"/"packets") and direction ("sent"/"received") looked up by name. Resets the
 * graph's accumulators and re-lays it out. Returns 0 if the graph is disabled or either name is unknown.
 *
 * The original loops walk each string table until a sentinel address (the adjacent rodata symbol);
 * reproduced here with the table counts, which equal those bounds. */

#include <stdint.h>
#include "headers/net_graph.h"
#include "headers/blam_data_globals.h"

extern int  stricmp(const char *a, const char *b);
extern void update_for_resolution_change(net_graph *graph, uint8_t force);

extern void initialize_graph(net_graph *const graph, net_graph_data_type type, net_graph_data_direction direction);
uint8_t net_graph_change_display(const char *type_string, const char *direction_string)
{
    if (!gNetGraphEnabled)
        return 0;

    net_graph_data_type type = net_graph_data_type_invalid;
    for (int i = 0; i < NUMBER_OF_NET_GRAPH_DATA_TYPES; ++i)
    {
        if (!stricmp(type_string, data_type_strings[i]))
        {
            type = (net_graph_data_type)i;
            break;
        }
    }

    net_graph_data_direction direction = net_graph_data_direction_invalid;
    for (int i = 0; i < NUMBER_OF_NET_GRAPH_DATA_DIRECTIONS; ++i)
    {
        if (!stricmp(direction_string, data_direction_strings[i]))
        {
            direction = (net_graph_data_direction)i;
            break;
        }
    }

    if (type == net_graph_data_type_invalid || direction == net_graph_data_direction_invalid)
        return 0;

    /* DEVIATION: donor initialize_graph@0x83764278 inlined here (0 xrefs); graph folded to &gGraph, type/direction pass through from the preceding name-table lookups */
    initialize_graph(&gGraph, type, direction);
    return 1;
}
