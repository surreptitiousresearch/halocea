/* rasterizer_debug_immediate_linestrip_screenspace @0x836A5FB0 */
#include <stdint.h>
#include "headers/point2d.h"
#include "headers/real_rgb_color.h"

extern void _rasterizer_debug_immediate_linestrip_screenspace(const point2d *points, int16_t point_count, const real_rgb_color *color);

void rasterizer_debug_immediate_linestrip_screenspace(const point2d *points, int16_t point_count, const real_rgb_color *color)
{
    _rasterizer_debug_immediate_linestrip_screenspace(points, point_count, color);
}
