/* add_finished_data_sample @0x83763490 — push one new sample into a net_graph's 320-sample sliding
 * window, update the running max (rescanning the whole window when the current max ages out), and
 * refresh the cached per-vertex screen Y-positions used to draw the graph's polyline.
 *
 * DEVIATION: the DB's decompiler output for this one is unusable past the first ~15 lines (flagged
 * "local variable allocation has failed") — the Y-position computation is a wall of __int64/DWORD1/
 * HIDWORD register-punning from a huge stack frame confusing the local allocator (same corruption
 * class as physics_compute_biped_collision/game_engine_update_teleporter). Rebuilt entirely from
 * disasm_range: the clean, single-sample case at the tail of the "max unchanged" branch
 * (disasm_range(0x83763994, 0x837639E8)) resolves to exactly
 *   vertex.position.n[1] = (float)graph->extents.n[2] - ((float)sample / (float)graph->max) * graph->graph_height
 * (confirmed field-by-field: 0x1C=graph_height, 0x28=extents.n[2] as a 16-bit load, 0x23D8=max,
 * 0x5D4=samples[319] — all matching net_graph.h's existing offsets exactly). The same formula was
 * then confirmed to open the "max changed" branch's very first stored vertex (disasm_range(0x8376359C,
 * 0x83763620), i=0 case using samples[0]), so that branch is a plain per-sample application of the
 * same formula across all 320 samples — rebuilt as a clean loop rather than transcribing the
 * decompiler's 10-wide unrolled punning.
 *
 * DEVIATION: the two vertex-array "shift left by one" loops (a 39-iteration 8-wide unrolled loop
 * starting from a raw pointer 20 bytes before vertices[0] — deliberately straddling the samples[]/
 * vertices[] array boundary — plus a 7-iteration tail loop) together are exactly equivalent to
 * `vertices[i].position.n[1] = vertices[i+1].position.n[1]` for i in [0,318]; rebuilt as one plain
 * loop rather than reproducing the cross-array pointer arithmetic (confirmed via disasm_range(
 * 0x8376392C, 0x83763994) that the first and last stores land exactly on vertices[0] and vertices[318]
 * respectively).
 *
 * FAITHFUL: after the max-rescan, the compiled code guards the `max_sample_lifetime = new_index + 1`
 * store with a check that the destination field's own address is non-null (disasm_range(0x83763570,
 * 0x8376358C) — decompiled elsewhere as the nonsensical `graph != (net_graph*const)-212`). Since that
 * address is always `&graph->max_sample_lifetime`, it can only be null if `graph` itself is an
 * impossible pointer value; reproduced here as an unconditional store since the guard is dead in any
 * reachable call. */

#include "headers/net_graph.h"

extern void _blkmov(void *dst, const void *src, int size);

void add_finished_data_sample(net_graph *graph, int sample)
{
    int previous_max = graph->max;

    /* Slide the 320-sample window left by one and append the new sample at the end. */
    _blkmov(graph->samples, &graph->samples[1], sizeof(int) * 319);
    graph->samples[319] = sample;

    if (sample < graph->max)
    {
        graph->max_sample_lifetime--;
        if (graph->max_sample_lifetime == 0)
        {
            /* The current max has aged out of the window: rescan all 320 samples for the new max and
             * reseed its lifetime to (index + 1) — how many more samples can arrive before this new
             * max itself ages out of the window. */
            int new_max = 1;
            int new_max_index = 319;
            for (int i = 0; i < 320; i++)
            {
                if (graph->samples[i] >= new_max)
                {
                    new_max = graph->samples[i];
                    new_max_index = i;
                }
            }
            graph->max_sample_lifetime = new_max_index + 1;
            graph->max = new_max;
        }
    }
    else
    {
        graph->max = sample;
        graph->max_sample_lifetime = 320;
    }

    if (previous_max == graph->max)
    {
        /* Max unchanged: shift the cached vertex Y-positions left by one and compute just the newest
         * one against the (unchanged) max. */
        for (int i = 0; i < 319; i++)
            graph->vertices[i].position.n[1] = graph->vertices[i + 1].position.n[1];

        graph->vertices[319].position.n[1] = (float)graph->extents.n[2]
                - ((float)graph->samples[319] / (float)graph->max) * graph->graph_height;
    }
    else
    {
        /* Max changed this frame: every cached Y-position was scaled against the old max, so
         * recompute all 320 from scratch against the new one. */
        for (int i = 0; i < 320; i++)
        {
            graph->vertices[i].position.n[1] = (float)graph->extents.n[2]
                    - ((float)graph->samples[i] / (float)graph->max) * graph->graph_height;
        }
    }
}
