/* rasterizer_widget_submit @0x836A63A8 — public entry thunk. A single unconditional branch
 * (tail-call) to the real implementation _rasterizer_widget_submit; forwards all arguments
 * unchanged. Verified against disasm: `b _rasterizer_widget_submit`. */

#include "headers/real_point3d.h"

extern void _rasterizer_widget_submit(int object_index, int widget_index, const real_point3d *centroid,
                                      void (*render_proc)(int, int));

void rasterizer_widget_submit(int object_index, int widget_index, const real_point3d *centroid,
                              void (*render_proc)(int, int))
{
    _rasterizer_widget_submit(object_index, widget_index, centroid, render_proc);
}
