/* update_parameters_from_globals @0x83763280 */
#include "headers/net_graph.h"

extern void clear_graph(net_graph *graph);

void update_parameters_from_globals(net_graph *const graph)
{
    if ( graph->period_length_ms != gNetGraphSamplePeriod )
    {
        graph->period_length_ms = gNetGraphSamplePeriod;
        clear_graph(graph);
    }
}
