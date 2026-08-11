/* process_samples @0x83763AB8 — advance a net_graph's sample period(s): while enough time has elapsed
 * (possibly more than one period at once, e.g. after a stall), push the accumulated current_sample_data as
 * a finished sample, recompute the rolling average from the last 4 samples, and start a new period.
 *
 * DEVIATION: the decompiler rendered the 4-sample sum as __int64 HIDWORD/LODWORD register-pun arithmetic;
 * it's plain int addition of samples[316..319]. */

#include <stdint.h>
#include "headers/net_graph.h"

extern uint32_t system_milliseconds(void);
extern void add_finished_data_sample(net_graph *graph, int sample);

extern void calculate_average(net_graph *const graph);
void process_samples(net_graph *const graph)
{
    unsigned int now = system_milliseconds();

    if ( graph->graph_empty )
        return;

    unsigned int elapsed = now - graph->period_start_ms;
    if ( elapsed < graph->period_length_ms )
        return;

    do
    {
        add_finished_data_sample(graph, graph->current_sample_data);

        /* DEVIATION: donor calculate_average@0x837633A0 inlined here; disasm shows the true sum includes samples[315] (loaded [r31+0x5C4]) which the prior hand-transcription dropped -- collapsing to the call both matches the donor verbatim and fixes that bug. */
        calculate_average(graph);

        unsigned int next_now = system_milliseconds();
        unsigned int period_length_ms = graph->period_length_ms;
        graph->period_start_ms = next_now;
        elapsed -= period_length_ms;
        graph->current_sample_data = 0;
    }
    while ( elapsed >= graph->period_length_ms );
}
