/* update_for_resolution_change @0x83763F58 — recompute a network bandwidth graph's on-screen geometry when the
 * render resolution changes (or when forced). The graph box occupies the bottom-right of the screen: 40% of the
 * frame width and 20% of the frame height, inset 64 pixels from the bottom-right corner. This rebuilds the data
 * extents, clears the sample/border vertex buffers, lays out the 5-point border outline, derives the label
 * placement rectangles in a virtual 640x480 space, and formats the "<type> <direction>" label string.
 *
 * The frame size is taken from rasterizer_globals.screen_bounds (rectangle2d {y0,x0,y1,x1}); a change in derived
 * width/height (or a forced refresh) triggers the rebuild.
 *
 * DEVIATION: the decompiler reported "local variable allocation has failed" and emitted garbled __int64/__int128
 * dword-shuffle output for this function; it was reconstructed directly from the disassembly. Every int->float
 * conversion below corresponds to an fcfid/frsp sequence, and every float->short to an fctiwz; written as plain
 * casts. Border vertex color -256 (0xFFFFFF00 ARGB) preserved verbatim. */

#include <stdint.h>
#include <string.h>
#include "headers/net_graph.h"
#include "headers/rasterizer_globals.h"

extern void clear_graph(net_graph *graph);
extern int _snprintf_0(char *string, unsigned int count, const char *format, ...);

extern const char *const data_type_strings[];
extern const char *const data_direction_strings[];

void update_for_resolution_change(net_graph *graph, uint8_t force)
{
    int frame_height = rasterizer_globals.screen_bounds.y1 - rasterizer_globals.screen_bounds.y0;
    int frame_width  = rasterizer_globals.screen_bounds.x1 - rasterizer_globals.screen_bounds.x0;

    if (graph->frame_height == frame_height && graph->frame_width == frame_width && !force)
        return;

    int bottom = rasterizer_globals.screen_bounds.y1 - 64;
    int right  = rasterizer_globals.screen_bounds.x1 - 64;

    graph->frame_height = frame_height;
    graph->frame_width = frame_width;

    float frame_width_f  = (float)frame_width;
    float frame_height_f = (float)frame_height;
    float bottom_f = (float)bottom;
    float right_f  = (float)right;

    graph->graph_width  = frame_width_f * 0.40000001f;
    graph->graph_height = frame_height_f * 0.2f;

    float top_f  = bottom_f - graph->graph_height;
    float left_f = right_f - graph->graph_width;

    graph->extents.y0 = (short)top_f;
    graph->extents.x0 = (short)left_f;
    graph->extents.y1 = (short)bottom;
    graph->extents.x1 = (short)right;

    memset(graph->vertices, 0, sizeof(graph->vertices));
    memset(graph->border, 0, sizeof(graph->border));
    clear_graph(graph);

    /* border outline: TL -> TR -> BR -> BL -> back to TL (5-point line strip) */
    graph->border[0].position.x = left_f - 1.0f;
    graph->border[0].position.y = top_f - 1.0f;
    graph->border[0].color = 0xFFFFFF00u;
    graph->border[1].position.x = right_f + 1.0f;
    graph->border[1].position.y = top_f - 1.0f;
    graph->border[1].color = 0xFFFFFF00u;
    graph->border[2].position.x = right_f + 1.0f;
    graph->border[2].position.y = bottom_f + 1.0f;
    graph->border[2].color = 0xFFFFFF00u;
    graph->border[3].position.x = left_f - 1.0f;
    graph->border[3].position.y = bottom_f + 1.0f;
    graph->border[3].color = 0xFFFFFF00u;
    graph->border[4].position.x = left_f - 1.0f;
    graph->border[4].position.y = top_f - 1.0f;
    graph->border[4].color = 0xFFFFFF00u;

    /* label placement rectangles, in a virtual 640x480 coordinate space */
    float top_i   = (float)graph->extents.y0;
    float left_i  = (float)graph->extents.x0;
    float right_i = (float)graph->extents.x1;
    float scale_x = 640.0f / frame_width_f;
    float scale_y = 480.0f / frame_height_f;

    graph->label_extents.y0 = (short)(top_i * scale_y);
    graph->label_extents.x0 = (short)(left_i * scale_x);
    graph->label_extents.y1 = 480;
    graph->label_extents.x1 = 640;

    graph->label_max_extents.y0 = (short)(top_i * scale_y);
    graph->label_max_extents.x0 = (short)(right_i * scale_x);
    graph->label_max_extents.y1 = 480;
    graph->label_max_extents.x1 = 640;

    graph->label_avg_extents.y0 = (short)((graph->graph_height * 0.5f + top_i) * scale_y);
    graph->label_avg_extents.x0 = (short)(right_i * scale_x);
    graph->label_avg_extents.y1 = 480;
    graph->label_avg_extents.x1 = 640;

    _snprintf_0(graph->label, 0x200u, "%s %s", data_type_strings[graph->type],
               data_direction_strings[graph->direction]);
}
