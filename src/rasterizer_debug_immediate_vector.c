/* rasterizer_debug_immediate_vector @0x836A5F28 — draw a debug line from p along
 * v scaled by size, in the given colour. */

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"

extern void _rasterizer_debug_immediate_line(const real_point3d *p0, const real_point3d *p1, const real_rgb_color *color0, const real_rgb_color *color1);

void rasterizer_debug_immediate_vector(const real_point3d *p, const real_vector3d *v, float size, const real_rgb_color *color, const real_rgb_color *unused)
{
    real_point3d endpoint;

    endpoint.n[0] = (v->n[0] * size) + p->n[0];
    endpoint.n[1] = (v->n[1] * size) + p->n[1];
    endpoint.n[2] = (v->n[2] * size) + p->n[2];

    /* Deviation: the disassembly passes the `color` register (r6) as both line
     * colours; the trailing `unused` param is never referenced. */
    _rasterizer_debug_immediate_line(p, &endpoint, color, color);
}
