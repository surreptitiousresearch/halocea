/* add_to_graph_current_sample @0x837632B8 — accumulate a packets/bytes sample into the graph's current
 * bucket (only if `direction` matches the graph's configured direction), then mark the graph non-empty and
 * stamp its period start on first use. */

#include <stdint.h>
#include "headers/net_graph.h"

extern uint32_t system_milliseconds(void);

void add_to_graph_current_sample(net_graph *const graph, const net_graph_data_direction direction,
    const int packets, const int bytes)
{
    if ( direction == graph->direction )
    {
        if ( graph->type == net_graph_data_type_bytes )
            graph->current_sample_data += bytes;
        else if ( graph->type == net_graph_data_type_packets )
            graph->current_sample_data += packets;

        if ( graph->graph_empty )
        {
            graph->period_start_ms = system_milliseconds();
            graph->graph_empty = 0;
        }
    }
}
