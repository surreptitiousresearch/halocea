/* hud_calculate_point @0x8379E110 — resolve a HUD placement definition to a screen-space anchor
 * point. Deliberately deferred in an earlier session (its callers reconstructed a partial signature
 * from call sites only); fully reconstructed here from disasm_range(0x8379E110, 0x8379E110+980) since
 * the decompile fails local-variable allocation and merges the 2nd/3rd parameters and several locals
 * into corrupted 64-bit phantom values.
 *
 * Two placement modes selected by `absolute_placement->corner`:
 *   - corner < _hud_corner_center: safe-area-relative corner anchor. Each bit of `corner` flips which edge of
 *     `render.camera.window_bounds` the offset is measured from (bit 0 = right vs left, bit 1 = bottom
 *     vs top) and the sign the offset/safe-area margin is applied with; the result is then rebased
 *     into viewport space by subtracting `render.camera.viewport_bounds`'s origin. The safe-area
 *     margins (90px horizontal, 50px vertical, standard TV-overscan-style safe title/action margins)
 *     are dropped to 0 when `hud_calculate_point_no_safearea_fit` is set (which also forces scale to
 *     1.0 instead of the caller-supplied/global scale).
 *   - corner >= _hud_corner_center: centered placement — the offset is added to half of `render.camera.window_bounds`'s
 *     width/height, with no safe-area margin and no viewport rebase.
 * When `bitmap` is non-NULL, its `registration_point` is then folded in via a second switch on the
 * SAME `corner` value (the four corners/default, where default covers corner >= _hud_corner_center as well as any other value),
 * shifting the anchor by the registration point relative to the bitmap's own width/height so the point
 * lands at the correct corner of the bitmap rather than its registration origin.
 *
 * FAITHFUL QUIRK: the default case of that second switch (corner >= _hud_corner_center, matching the centered placement
 * mode) adjusts BOTH x and y by `bitmap->width / 2` — disasm confirms only `width` is ever read in
 * this branch, `height` is never loaded. Reproduced verbatim, not "fixed" to use height for y. */

#include <stdint.h>
#include "headers/hud_absolute_placement_definition.h"
#include "headers/hud_placement_definition.h"
#include "headers/bitmap_data.h"
#include "headers/point2d.h"
#include "headers/render_globals.h"
#include "headers/hud_corner.h"
#include "headers/blam_data_globals.h"


void hud_calculate_point(int16_t local_player_index, const hud_absolute_placement_definition *absolute_placement, const hud_placement_definition *placement, const bitmap_data *bitmap, uint8_t in_multiplayer, float override_scale, point2d *result)
{
    float scale = override_scale;
    if (!in_multiplayer || override_scale == 0.0f)
        scale = hcex_hud_globals_scale;
    if (hud_calculate_point_no_safearea_fit)
        scale = 1.0f;

    int16_t corner = absolute_placement->corner;
    float point_x, point_y;

    if (corner < _hud_corner_center)
    {
        float safe_area_x = 90.0f;
        float safe_area_y = 50.0f;
        if (hud_calculate_point_no_safearea_fit)
        {
            safe_area_x = 0.0f;
            safe_area_y = 0.0f;
        }

        float sign_x = (corner & 1) ? -1.0f : 1.0f;
        float sign_y = (corner & 2) ? -1.0f : 1.0f;
        short window_edge_x = (corner & 1) ? render.camera.window_bounds.__s1.x1 : render.camera.window_bounds.__s1.x0;
        short window_edge_y = (corner & 2) ? render.camera.window_bounds.__s1.y1 : render.camera.window_bounds.__s1.y0;

        point_x = (placement->offset.__s1.x * scale + safe_area_x) * sign_x + window_edge_x
                - render.camera.viewport_bounds.__s1.x0;
        point_y = (placement->offset.__s1.y * scale + safe_area_y) * sign_y + window_edge_y
                - render.camera.viewport_bounds.__s1.y0;
    }
    else
    {
        short window_width = render.camera.window_bounds.__s1.x1 - render.camera.window_bounds.__s1.x0;
        short window_height = render.camera.window_bounds.__s1.y1 - render.camera.window_bounds.__s1.y0;

        point_x = placement->offset.__s1.x * scale + window_width / 2;
        point_y = placement->offset.__s1.y * scale + window_height / 2;
    }

    if (bitmap)
    {
        switch (corner)
        {
        case _hud_corner_top_left:
            point_x += bitmap->registration_point.__s1.x * scale;
            point_y += bitmap->registration_point.__s1.y * scale;
            break;
        case _hud_corner_top_right:
            point_x += (bitmap->registration_point.__s1.x - bitmap->width) * scale;
            point_y += bitmap->registration_point.__s1.y * scale;
            break;
        case _hud_corner_bottom_left:
            point_x += bitmap->registration_point.__s1.x * scale;
            point_y += (bitmap->registration_point.__s1.y - bitmap->height) * scale;
            break;
        case _hud_corner_bottom_right:
            point_x += (bitmap->registration_point.__s1.x - bitmap->width) * scale;
            point_y += (bitmap->registration_point.__s1.y - bitmap->height) * scale;
            break;
        default:
            point_x += (bitmap->registration_point.__s1.x + bitmap->width / 2) * scale;
            point_y += (bitmap->registration_point.__s1.y + bitmap->width / 2) * scale;
            break;
        }
    }

    result->__s1.x = (int16_t)point_x;
    result->__s1.y = (int16_t)point_y;
}
