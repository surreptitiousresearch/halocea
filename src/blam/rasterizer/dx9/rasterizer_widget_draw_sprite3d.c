/* rasterizer_widget_draw_sprite3d @0x836A63D8 — public entry thunk. A single unconditional branch
 * (tail-call) to _rasterizer_widget_draw_sprite3d; forwards all arguments unchanged.
 * Verified against disasm: `b _rasterizer_widget_draw_sprite3d`. */

#include "headers/real_point3d.h"
#include "headers/real_vector2d.h"

extern void _rasterizer_widget_draw_sprite3d(const real_point3d *point, float radius, const real_vector2d *scale, float rotation, unsigned int color);

void rasterizer_widget_draw_sprite3d(const real_point3d *point, float radius, const real_vector2d *scale,
                                     float rotation, unsigned int color)
{
    _rasterizer_widget_draw_sprite3d(point, radius, scale, rotation, color);
}
