/* initialize_graph @0x83764278 */
#include <stdint.h>
#include "headers/net_graph.h"

extern void update_for_resolution_change(net_graph *graph, uint8_t force);

void initialize_graph(net_graph *const graph, net_graph_data_type type, net_graph_data_direction direction)
{
    graph->period_start_ms = 0;
    graph->graph_empty = 1;
    graph->type = type;
    graph->direction = direction;
    graph->cumulative_bit_sent_total = 0;
    graph->cumulative_bit_received_total = 0;
    graph->period_length_ms = gNetGraphSamplePeriod;
    graph->cumulative_start_ms = 0;
    update_for_resolution_change(graph, 1);
}
