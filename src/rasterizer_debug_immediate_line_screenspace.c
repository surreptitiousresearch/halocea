#include "headers/point2d.h"
#include "headers/real_rgb_color.h"

extern void _rasterizer_debug_immediate_line_screenspace(const point2d *p0, const point2d *p1, const real_rgb_color *color0, const real_rgb_color *color1);

void rasterizer_debug_immediate_line_screenspace(const point2d *p0, const point2d *p1,
                                                 const real_rgb_color *color0,
                                                 const real_rgb_color *color1)
{
    _rasterizer_debug_immediate_line_screenspace(p0, p1, color0, color1);
}
