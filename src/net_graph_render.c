/* net_graph_render @0x83764250 — draw the network bandwidth/latency graph when it is enabled, by re-running its
 * resolution-dependent layout/draw pass. */

#include <stdint.h>
#include "headers/net_graph.h"
#include "headers/blam_data_globals.h"

extern void update_for_resolution_change(net_graph *graph, uint8_t force);

void net_graph_render(void)
{
    if (gNetGraphEnabled)
        update_for_resolution_change(&gGraph, 0);
}
