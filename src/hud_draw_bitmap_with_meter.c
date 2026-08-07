/* hud_draw_bitmap_with_meter @0x8379F550 — resolves a HUD placement to a screen anchor point and
 * draw bounds, then hands off to hud_draw_bitmap_internal to emit the actual quad.
 *
 * DEVIATION: the DB decompiler failed local-variable allocation and emitted a garbage 31-parameter
 * signature; the real one is TEN parameters, recovered from disasm_range(0x8379F550, 0x8379F694) with
 * the dead-GPR-shadow rule for the two float args (scale/theta). The prologue reads exactly two stack
 * bytes — arg_57 = param 9 in_multiplayer @0x8379F5EC, arg_5F = param 10 is_interface_bitmap @0x8379F564;
 * a prior 11-param recon used params 10 and 11 for those two roles, so both were off by one slot.
 * `hud_calculate_point` (now fully reconstructed in its own file) takes the drawn bitmap as its 4th
 * argument, used to fold the bitmap's registration point into the anchor point; this call site passes
 * a literal NULL since the point is fed straight to hud_calculate_bitmap_bounds/hud_draw_bitmap_internal
 * instead, which apply the bitmap's own placement math separately.
 *
 * FAITHFUL QUIRK: `override_scale` is passed as the literal constant 0.0f at this call site — this
 * is intentional, not a corruption: hud_calculate_point falls back to the global `hcex_hud_globals_scale`
 * whenever override_scale == 0.0, which is exactly the behavior this caller wants (no per-call override). */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/hud_scaling_flags.h"
#include "headers/hud_absolute_placement_definition.h"
#include "headers/hud_placement_definition.h"
#include "headers/point2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/real_vector2d.h"
#include "headers/render_globals.h"
#include "headers/blam_data_globals.h"

typedef struct rasterizer_meter_parameters rasterizer_meter_parameters;


extern void hud_calculate_point(int16_t local_player_index, const hud_absolute_placement_definition *absolute_placement, const hud_placement_definition *placement, const bitmap_data *bitmap, uint8_t in_multiplayer, float override_scale, point2d *result);

extern void hud_calculate_bitmap_bounds(const bitmap_data *bitmap, int16_t placement_type, const real_rectangle2d *clip, real_rectangle2d *bounds, uint8_t is_interface_bitmap);

extern void hud_draw_bitmap_internal(rasterizer_meter_parameters *meter_parameters, const bitmap_data *bitmap, const point2d *point, const real_rectangle2d *clip, const real_rectangle2d *bounds, const real_vector2d *xy_scale, float theta, unsigned int color);

void hud_draw_bitmap_with_meter(rasterizer_meter_parameters *meter_parameters, const bitmap_data *bitmap, const hud_absolute_placement_definition *absolute_placement, const hud_placement_definition *placement, const real_rectangle2d *clip, float scale, float theta, unsigned int color32, uint8_t in_multiplayer, uint8_t is_interface_bitmap)
{
    real_rectangle2d default_clip;
    default_clip.__s1.x0 = 0.0f;
    default_clip.__s1.x1 = 1.0f;
    default_clip.__s1.y0 = 0.0f;
    default_clip.__s1.y1 = 1.0f;

    if (is_interface_bitmap)
    {
        default_clip.__s1.x1 = (float)bitmap->width;
        default_clip.__s1.y1 = (float)bitmap->height;
    }

    if (!clip)
        clip = &default_clip;

    real_vector2d xy_scale;
    xy_scale.__s1.i = placement->scale.__s1.i * scale;
    xy_scale.__s1.j = placement->scale.__s1.j * scale;

    unsigned char use_multiplayer_scaling;
    if (in_multiplayer && !(placement->multiplayer_scaling_flags & (1u << _hud_dont_scale_offset_bit)))
        use_multiplayer_scaling = 1;
    else
        use_multiplayer_scaling = 0;

    point2d point;
    hud_calculate_point(render.local_player_index, absolute_placement, placement, 0,
                         use_multiplayer_scaling, 0.0f, &point);

    real_rectangle2d bounds;
    hud_calculate_bitmap_bounds(bitmap, absolute_placement->corner, clip, &bounds, is_interface_bitmap);

    /* unused_flags: unset at this call site (dead r9); hud_draw_bitmap_internal's own body never reads it */
    hud_draw_bitmap_internal(meter_parameters, bitmap, &point, clip, &bounds, &xy_scale, theta, color32); /* attested 8-param: phantom trailing arg dropped */
}
