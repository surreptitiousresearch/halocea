#include <stdint.h>

#include "headers/net_graph.h"

extern uint32_t system_milliseconds(void);

void add_to_cumulative_total(net_graph *const graph, net_graph_data_direction direction, const int bytes)
{
    int bits = 8 * bytes;

    if ( direction )
        graph->cumulative_bit_received_total += bits;
    else
        graph->cumulative_bit_sent_total += bits;

    if ( !graph->cumulative_start_ms )
        graph->cumulative_start_ms = system_milliseconds();
}
