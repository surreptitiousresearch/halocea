/* net_graph_data_received @0x83763DC8 — record a received-packet sample in the network graph and update
 * the cumulative received-bit total, starting the cumulative window on first call. */

#include <stdint.h>
#include "headers/net_graph.h"

#include "headers/net_graph.h"
extern void add_to_graph_current_sample(net_graph *const graph, const net_graph_data_direction direction, const int packets, const int bytes);
extern uint32_t system_milliseconds(void);

void net_graph_data_received(int packets, int bytes)
{
    add_to_graph_current_sample(&gGraph, net_graph_data_direction_received, packets, bytes);
    gGraph.cumulative_bit_received_total += 8 * bytes;
    if ( !gGraph.cumulative_start_ms )
        gGraph.cumulative_start_ms = system_milliseconds();
}
