/* rasterizer_debug_immediate_point @0x836A5E20 — draw a debug "point" as three axis-aligned line segments
 * (X, Y, Z) of length `size` crossing through `p`, each drawn with `color` at both endpoints.
 *
 * DEVIATION: the decompiler shows a phantom 4th parameter passed as both color arguments to
 * rasterizer_debug_immediate_line; disasm and the DB's real 3-parameter prototype confirm it's the
 * function's own `color` parameter reused for both color0 and color1. */

#include "headers/real_point3d.h"
#include "headers/real_rgb_color.h"

extern void rasterizer_debug_immediate_line(const real_point3d *p0, const real_point3d *p1, const real_rgb_color *color0, const real_rgb_color *color1);

void rasterizer_debug_immediate_point(const real_point3d *p, float size, const real_rgb_color *color)
{
    float half_size = size * 0.5f;
    real_point3d p0, p1;

    p0.n[0] = p->n[0] - half_size;
    p0.n[1] = p->n[1];
    p0.n[2] = p->n[2];
    p1.n[0] = p->n[0] + half_size;
    p1.n[1] = p->n[1];
    p1.n[2] = p->n[2];
    rasterizer_debug_immediate_line(&p0, &p1, color, color);

    p0.n[0] = p->n[0];
    p0.n[1] = p->n[1] - half_size;
    p0.n[2] = p->n[2];
    p1.n[0] = p->n[0];
    p1.n[1] = p->n[1] + half_size;
    p1.n[2] = p->n[2];
    rasterizer_debug_immediate_line(&p0, &p1, color, color);

    p0.n[0] = p->n[0];
    p0.n[1] = p->n[1];
    p0.n[2] = p->n[2] - half_size;
    p1.n[0] = p->n[0];
    p1.n[1] = p->n[1];
    p1.n[2] = p->n[2] + half_size;
    rasterizer_debug_immediate_line(&p0, &p1, color, color);
}
