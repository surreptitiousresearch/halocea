/* sample_finished @0x83763A48 — closes out the current sampling period for a net_graph: commits the
 * accumulated sample, then rederives `average` from the newest 5 slots of the 320-sample ring
 * (sum * 0.25, matching the disassembly exactly), and resets the period start time and accumulator for
 * the next period. */

#include <stdint.h>
#include "headers/net_graph.h"

extern void add_finished_data_sample(net_graph *graph, int sample);
extern uint32_t system_milliseconds(void);

extern void calculate_average(net_graph *const graph);
void sample_finished(net_graph *const graph)
{
    add_finished_data_sample(graph, graph->current_sample_data);

    /* DEVIATION: donor calculate_average@0x837633A0 inlined verbatim (samples[319..315]*0.25f, disasm-confirmed order+offsets+constant); collapsed to call, param folded to graph. */
    calculate_average(graph);
    graph->period_start_ms = system_milliseconds();
    graph->current_sample_data = 0;
}
