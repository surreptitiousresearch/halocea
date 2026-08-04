/* fg_init @0x83790540 — (re)build the framerate graph geometry whenever the screen dimensions change. Lays out
 * the plot box, the evenly-spaced 512-vertex plot line (all at the baseline, white), the 5-vertex border outline,
 * the running-average window, and the three on-screen label rectangles (title / max / average) positioned in the
 * virtual 640x480 coordinate space. Early-outs when the frame size is unchanged.
 *
 * DEVIATIONS (decompiler reported "local variable allocation has failed"; reconstructed from disassembly):
 *   - The decompiler invented an `__int64 a1` parameter and a return value out of OVERLAPPED stack slots; this is
 *     a void(void) function. The "neoffire_crouching" / "_ui_tex_list" strings are constant-pool address
 *     artifacts, not real data.
 *   - The "FPS" label is written as one inlined 4-byte word store (0x46505300), not a strcpy.
 *   - All int<->float conversions are fcfid/fctiwz round-trips; restored as plain casts. */

#include "headers/fg_graph.h"
#include "headers/rasterizer_globals.h"
#include "headers/blam_data_globals.h"


extern void *memset(void *destination, int value, unsigned int size);

void fg_init(void)
{
    int screen_height = rasterizer_globals.screen_bounds.n[2] - rasterizer_globals.screen_bounds.n[0];
    int screen_width  = rasterizer_globals.screen_bounds.n[3] - rasterizer_globals.screen_bounds.n[1];

    if (fg_frame_height == screen_height && fg_frame_width == screen_width)
        return;  /* dimensions unchanged — nothing to rebuild */

    fg_frame_height = screen_height;
    fg_frame_width = screen_width;

    fg_graph *graph = &fg_graphs[0];

    float plot_right = (float)(rasterizer_globals.screen_bounds.n[3] - 64);

    /* plot box: top 30, left 64, baseline 150, right (screen_right - 64) */
    graph->extents.n[0] = 30;
    graph->extents.n[1] = 64;
    graph->extents.n[2] = 150;
    graph->extents.n[3] = (__int16)(int)plot_right;

    float virtual_x_scale = 640.0f / (float)screen_width;
    float virtual_y_scale = 480.0f / (float)screen_height;

    memset(graph->vertices, 0, sizeof(graph->vertices));
    memset(graph->border, 0, sizeof(graph->border));

    graph->average_samples[0] = 0.0f;
    graph->average_samples[1] = 0.0f;
    graph->average_samples[2] = 0.0f;
    graph->average_samples[3] = 0.0f;

    /* evenly-spaced x grid across the plot width; every vertex starts at the baseline, opaque white */
    float plot_width = plot_right - 64.0f;
    for (int i = 0; i < 512; ++i)
    {
        graph->vertices[i].position.n[1] = 150.0f;
        graph->vertices[i].position.n[0] = (float)(int)((float)i * plot_width * 0.001953125f + 64.0f);
        graph->vertices[i].color = 0xFFFFFFFF;
    }

    /* border line-strip outlining the plot, 1px outside it, opaque yellow */
    float border_right = plot_right + 1.0f;
    graph->border[0].position.n[0] = 63.0f;        graph->border[0].position.n[1] = 29.0f;
    graph->border[1].position.n[0] = border_right;  graph->border[1].position.n[1] = 29.0f;
    graph->border[2].position.n[0] = border_right;  graph->border[2].position.n[1] = 151.0f;
    graph->border[3].position.n[0] = 63.0f;        graph->border[3].position.n[1] = 151.0f;
    graph->border[4].position.n[0] = 63.0f;        graph->border[4].position.n[1] = 29.0f;
    graph->border[0].color = 0xFFFFFF00;
    graph->border[1].color = 0xFFFFFF00;
    graph->border[2].color = 0xFFFFFF00;
    graph->border[3].color = 0xFFFFFF00;
    graph->border[4].color = 0xFFFFFF00;

    graph->max = 60.0f;
    graph->average = 0.0f;

    *(unsigned int *)graph->label = 0x46505300;  /* "FPS\0", inlined word store */

    /* label rectangles in virtual 640x480 space; n[2]/n[3] span the whole screen so text is never clipped */
    graph->label_extents.n[0] = (__int16)(int)(30.0f * virtual_y_scale);
    graph->label_extents.n[1] = (__int16)(int)(64.0f * virtual_x_scale);
    graph->label_extents.n[2] = 480;
    graph->label_extents.n[3] = 640;

    graph->label_max_extents.n[0] = graph->label_extents.n[0];
    graph->label_max_extents.n[1] = (__int16)(int)(plot_right * virtual_x_scale);
    graph->label_max_extents.n[2] = 480;
    graph->label_max_extents.n[3] = 640;

    graph->label_avg_extents.n[0] = (__int16)(int)((30.0f + 60.0f) * virtual_y_scale);
    graph->label_avg_extents.n[1] = graph->label_max_extents.n[1];
    graph->label_avg_extents.n[2] = 480;
    graph->label_avg_extents.n[3] = 640;
}
