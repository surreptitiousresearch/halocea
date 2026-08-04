/* hud_draw_bitmap_direct 0x8379F698 — draw a single HUD bitmap at a point with a uniform scale and rotation.
 * Builds a default unit texture-coordinate rect (or, for interface bitmaps, the bitmap's full pixel rect), uses
 * the caller's clip rect when supplied, computes the placement bounds, and submits the quad with no meter.
 *
 * DEVIATION: the decompiler mangled the call signature (FPR-shadow — scale/theta are float args 5/6, so color
 * and is_interface_bitmap land in r9/r10). Restored from the disassembly: hud_calculate_bitmap_bounds and
 * hud_draw_bitmap_internal receive the arguments below; xy_scale = {scale, scale}. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/real_rectangle2d.h"
#include "headers/real_vector2d.h"
#include "headers/point2d.h"

struct rasterizer_meter_parameters;

#include "headers/rasterizer_meter_parameters.h"
extern void hud_calculate_bitmap_bounds(const bitmap_data *bitmap, int16_t placement_type, const real_rectangle2d *clip, real_rectangle2d *bounds, uint8_t is_interface_bitmap);
extern void hud_draw_bitmap_internal(rasterizer_meter_parameters *meter_parameters, const bitmap_data *bitmap, const point2d *point, const real_rectangle2d *clip, const real_rectangle2d *bounds, const real_vector2d *xy_scale, float theta, unsigned int color);

void hud_draw_bitmap_direct(const bitmap_data *bitmap, int16_t placement, const point2d *point,
                            const real_rectangle2d *clip, float scale, float theta, unsigned int color,
                            uint8_t is_interface_bitmap)
{
    real_rectangle2d texture_coords;
    texture_coords.n[0] = 0.0f;
    texture_coords.n[1] = 1.0f;
    texture_coords.n[2] = 0.0f;
    texture_coords.n[3] = 1.0f;
    if ( is_interface_bitmap )
    {
        texture_coords.n[1] = (float)bitmap->width;
        texture_coords.n[3] = (float)bitmap->height;
    }

    const real_rectangle2d *effective_clip = clip ? clip : &texture_coords;

    real_vector2d xy_scale;
    xy_scale.n[0] = scale;
    xy_scale.n[1] = scale;

    real_rectangle2d bounds;
    hud_calculate_bitmap_bounds(bitmap, placement, effective_clip, &bounds, is_interface_bitmap);
    hud_draw_bitmap_internal(nullptr, bitmap, point, effective_clip, &bounds, &xy_scale, theta, color);
}
