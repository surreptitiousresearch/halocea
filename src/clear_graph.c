/* clear_graph @0x83763188 — reset a net_graph: lay out the 320 baseline grid vertices along the bottom of
 * the graph extents (x spread evenly across [x0,x1], y fixed at the baseline y1, color -1), clear the
 * sample ring, and zero all running stats.
 * DEVIATION: the decompiler bailed ("local variable allocation has failed") on the soft-float int->float
 * vertex coordinate conversions; recovered from disassembly. the per-vertex conversions were recovered from disassembly and now use the
 * DB dynamic_screen_vertex struct. */

#include <string.h>
#include "headers/net_graph.h"
#include "headers/dynamic_screen_vertex.h"


void clear_graph(net_graph *graph)
{
    float baseline_y = (float)graph->extents.y1;
    int x0 = graph->extents.x0;
    int width = graph->extents.x1 - graph->extents.x0;

    for ( int i = 0; i < 320; ++i )
    {
        dynamic_screen_vertex *vertex = &graph->vertices[i];
        vertex->position.n[1] = baseline_y;
        int x = width * i / 320 + x0;
        vertex->position.n[0] = (float)x;
        vertex->color = (unsigned int)-1;
    }

    graph->current_sample_data = 0;
    memset(graph->samples, 0, sizeof(graph->samples));
    graph->period_start_ms = 0;
    graph->cumulative_bit_received_total = 0;
    graph->max = 1;
    graph->cumulative_bit_sent_total = 0;
    graph->cumulative_start_ms = 0;
    graph->average = 0.0f;
    graph->graph_empty = 1;
    graph->cumulative_bps_received = 0.0f;
    graph->cumulative_bps_sent = 0.0f;
}
