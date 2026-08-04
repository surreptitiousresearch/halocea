/* calculate_average @0x837633A0 — averages the last 5 samples of a net_graph into `average`.
 *
 * DEVIATION: the decompiler invented 2 extra unused parameters (int a2, __int64 a3); DB prototype and
 * disasm confirm a plain single-param `void calculate_average(net_graph*)`. The multiply constant is
 * 0.25 exactly, per disasm's immediate (not 0.2, despite summing 5 samples). */

#include "headers/net_graph.h"

void calculate_average(net_graph *const graph)
{
    int sum = graph->samples[319] + graph->samples[318] + graph->samples[317]
            + graph->samples[316] + graph->samples[315];

    graph->average = (float)sum * 0.25f;
}
