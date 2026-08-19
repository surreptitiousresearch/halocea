/* net_graph_update @ 0x83763E38 — advance the bandwidth graph: roll finished sample
 * periods into the history, recompute the running 5-sample average, and update totals. */
#include <stdint.h>
#include "headers/net_graph.h"

#include "headers/net_graph.h"
#include "headers/net_graph.h"
#include "headers/net_graph.h"
extern void clear_graph(net_graph *graph);
extern uint32_t system_milliseconds(void);
extern void add_finished_data_sample(net_graph *graph, int sample);
extern void update_cumulative(net_graph *graph);

extern void calculate_average(net_graph *const graph);
void net_graph_update(void)
{
    unsigned int now;

    if ( gGraph.period_length_ms != gNetGraphSamplePeriod )
    {
        gGraph.period_length_ms = gNetGraphSamplePeriod;
        clear_graph(&gGraph);
    }

    now = system_milliseconds();
    if ( !gGraph.graph_empty )
    {
        unsigned int elapsed = now - gGraph.period_start_ms;
        while ( elapsed >= gGraph.period_length_ms )
        {
            /* DEVIATION: sum+average block is calculate_average@0x837633A0 inlined verbatim (samples[319..315]*0.25f, disasm-confirmed offsets+constant); collapsed to call, donor's graph param folded to &gGraph. */
            add_finished_data_sample(&gGraph, gGraph.current_sample_data);
            calculate_average(&gGraph);
            gGraph.period_start_ms = system_milliseconds();
            elapsed -= gGraph.period_length_ms;
            gGraph.current_sample_data = 0;
        }
    }
    update_cumulative(&gGraph);
}
