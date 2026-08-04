#include "headers/real_point3d.h"
#include "headers/real_rgb_color.h"

extern void _rasterizer_debug_immediate_line(const real_point3d *p0, const real_point3d *p1, const real_rgb_color *color0, const real_rgb_color *color1);

void rasterizer_debug_immediate_line(const real_point3d *p0, const real_point3d *p1,
                                     const real_rgb_color *color0, const real_rgb_color *color1)
{
    _rasterizer_debug_immediate_line(p0, p1, color0, color1);
}
