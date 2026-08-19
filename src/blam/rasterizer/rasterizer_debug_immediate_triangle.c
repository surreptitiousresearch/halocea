/* rasterizer_debug_immediate_triangle @0x836A5F90 */
#include "headers/real_point3d.h"
#include "headers/real_rgb_color.h"

extern void _rasterizer_debug_immediate_triangle(const real_point3d *p0, const real_point3d *p1, const real_point3d *p2, const real_rgb_color *color0, const real_rgb_color *color1, const real_rgb_color *color2);

void rasterizer_debug_immediate_triangle(const real_point3d *p0, const real_point3d *p1,
                                         const real_point3d *p2, const real_rgb_color *color0,
                                         const real_rgb_color *color1, const real_rgb_color *color2)
{
    _rasterizer_debug_immediate_triangle(p0, p1, p2, color0, color1, color2);
}
