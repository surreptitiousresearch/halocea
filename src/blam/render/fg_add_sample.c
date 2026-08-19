/* fg_add_sample @0x83790BB8 — push one new sample onto a scrolling framerate graph. Slides the 4-sample
 * average window left and appends the sample (updating the displayed average), then scrolls the 512-vertex
 * plotted line one step to the left and sets the rightmost vertex's height from the new (max-clamped) sample.
 *
 * The sample is mapped to a 0..120 pixel bar height by (sample / max) * 120, and the vertex y is the plot
 * baseline (extents.n[2]) minus that height.
 *
 * DEVIATION: the original unrolls the vertex scroll 7 vertices per iteration (73 iterations == 511 vertices);
 * written here as the equivalent simple shift-left loop. Index math (12976 * index) is the fg_graph stride. */

#include "headers/fg_graph.h"
#include "headers/blam_data_globals.h"


void fg_add_sample(int index, float sample)
{
    fg_graph *graph = &fg_graphs[index];

    /* slide the averaging window left and append the new sample */
    float sample1 = graph->average_samples[1];
    float sample2 = graph->average_samples[2];
    float sample3 = graph->average_samples[3];
    graph->average_samples[0] = sample1;
    graph->average_samples[1] = sample2;
    graph->average_samples[2] = sample3;
    graph->average_samples[3] = sample;
    graph->average = (((sample1 + sample2) + sample3) + sample) * 0.25f;

    /* clamp to full scale and convert to a bar height in graph pixels */
    float max = graph->max;
    if (sample > max)
        sample = max;
    float bar_height = (sample / max) * 120.0f;

    /* scroll the plotted line one sample to the left (original: unrolled 7-wide, 73 iterations) */
    for (int i = 0; i < 511; ++i)
        graph->vertices[i].position.n[1] = graph->vertices[i + 1].position.n[1];

    /* newest sample at the right edge, measured up from the baseline */
    graph->vertices[511].position.n[1] = (float)graph->extents.n[2] - bar_height;
}
