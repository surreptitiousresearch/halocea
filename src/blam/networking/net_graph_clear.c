/* net_graph_clear @0x83763F48 — reset the network performance graph's sample buffer. */

#include "headers/net_graph.h"
#include "headers/blam_data_globals.h"

extern void clear_graph(net_graph *graph);

void net_graph_clear(void)
{
    clear_graph(&gGraph);
}
