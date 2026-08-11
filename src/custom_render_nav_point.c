/* custom_render_nav_point @0x837ED6E8 — draw one HUD navigation waypoint (objective marker) for a
 * local player: project the world position to the screen, clamp it to an ellipse inscribed in the
 * margin-inset render window when off-screen (rotating the arrow to point at the target), draw the
 * arrow sprite from hud_globals->waypoint, and, for on-screen waypoints with render_distance set,
 * draw the distance in meters below/right of the arrow via hud_draw_numbers.
 *
 * Reconstructed from disassembly (decompiler reported "local variable allocation has failed"). The
 * decompiler's rectangle2d n[] labels suggested transposed center/margin pairings; against the DB
 * layout {y0, x0, y1, x1} every pairing is consistent (width with left/right, height with top/bottom)
 * — a standard ellipse containment/projection. Notable decompiler corrections are marked DEVIATION. */

#include <stdint.h>
#include <string.h>
#include <math.h>

#include "headers/hud_globals.h"
#include "headers/hud_waypoint_arrow.h"
#include "headers/hud_waypoint_arrow_flags.h"
#include "headers/waypoint_type.h"
#include "headers/render_globals.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_point2d.h"
#include "headers/point2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_rgb_color.h"
#include "headers/bitmap_data.h"
#include "headers/number_hud_element_definition.h"
#include "headers/hud_absolute_placement_definition.h"
#include "headers/hud_corner.h"
#include "headers/blam_data_globals.h"


#include "headers/render_camera.h"
#include "headers/render_frustum.h"

extern float __fsqrts(float x);
extern int local_player_get_player_index(int16_t local_player_index);
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern uint8_t render_camera_view_to_screen(const render_camera *camera, const render_frustum *frustum, const real_point3d *view_point, real_point2d *screen_point);
extern void hud_retrieve_bitmap_and_bounding_rect(int bitmap_group_index, int16_t sequence_index, int16_t frame_index, const bitmap_data **bitmap, const real_rectangle2d **clip);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern real_rgb_color *pixel32_to_real_rgb_color(unsigned int pixel, real_rgb_color *color);
extern void hud_draw_bitmap_direct(const bitmap_data *bitmap, int16_t placement, const point2d *point, const real_rectangle2d *clip, float scale, float theta, unsigned int color, uint8_t is_interface_bitmap);
extern void hud_draw_numbers(int16_t local_player_index, const hud_absolute_placement_definition *placement, const number_hud_element_definition *numbers, int16_t value, int16_t decimal_value, int16_t draw_flags, int flash_reference_time, float override_scale);

void custom_render_nav_point(int16_t local_player_index, const real_point3d *position_pointer, int16_t nav_index, int16_t waypoint_type, uint8_t render_distance)
{
    hud_waypoint_arrow *arrow = (hud_waypoint_arrow *)hud_globals->waypoint.arrows.address + nav_index;
    real_point3d view_point = *position_pointer;

    int unit_index = local_player_get_player_index(local_player_index);
    if ( unit_index != -1 )
        unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum,
                local_player_get_player_index(local_player_index))->unit_index;
    real_point3d camera_position;
    unit_get_camera_position(unit_index, &camera_position);

    float delta_x = position_pointer->n[0] - camera_position.n[0];
    float delta_y = position_pointer->n[1] - camera_position.n[1];
    float delta_z = position_pointer->n[2] - camera_position.n[2];
    float distance = __fsqrts(delta_y * delta_y + (delta_x * delta_x + delta_z * delta_z));

    float arrow_scale;
    if ( distance <= 15.0f )
        arrow_scale = (float)pow(1.0f - distance * 0.06666667f, 0.7) + 0.5f;
    else
        arrow_scale = 0.5f;

    matrix4x3_transform_point(&render.frustum.world_to_view, &view_point, &view_point);

    /* arrow_state doubles as the sequence-index selector: the incoming waypoint_type, forced to 1
     * when the point is off-screen or clamped to the screen-edge ellipse */
    int16_t arrow_state = waypoint_type;
    float rotation = 0.0f;
    real_point2d screen_point;
    float offset_x;
    float offset_y;

    if ( waypoint_type == _waypoint_off_screen
      || !render_camera_view_to_screen(&render.camera, &render.frustum, &view_point, &screen_point) )
    {
        arrow_state = _waypoint_off_screen;
        offset_x = view_point.n[0];
        offset_y = -view_point.n[1];
    }
    else
    {
        int center_x = (render.camera.viewport_bounds.__s1.x1 - render.camera.viewport_bounds.__s1.x0) / 2
                + render.camera.viewport_bounds.__s1.x0;
        int center_y = (render.camera.viewport_bounds.__s1.y1 - render.camera.viewport_bounds.__s1.y0) / 2
                + render.camera.viewport_bounds.__s1.y0;
        offset_x = screen_point.n[0] - (float)center_x;
        offset_y = screen_point.n[1] - (float)center_y;
    }
    screen_point.n[0] = offset_x;
    screen_point.n[1] = offset_y;

    int window_width = render.camera.window_bounds.__s1.x1 - render.camera.window_bounds.__s1.x0;
    int window_height = render.camera.window_bounds.__s1.y1 - render.camera.window_bounds.__s1.y0;
    float semi_axis_x = ((float)window_width
            - (hud_globals->waypoint.right_offset + hud_globals->waypoint.left_offset)) * 0.5f;
    float semi_axis_y = ((float)window_height
            - (hud_globals->waypoint.bottom_offset + hud_globals->waypoint.top_offset)) * 0.5f;
    float ellipse_term_y = semi_axis_x * offset_y;
    float ellipse_term_x = semi_axis_y * offset_x;
    float ellipse_bound = semi_axis_y * semi_axis_x;

    if ( arrow_state == _waypoint_off_screen
      || ellipse_bound * ellipse_bound <= ellipse_term_x * ellipse_term_x + ellipse_term_y * ellipse_term_y )
    {
        arrow_state = _waypoint_off_screen;
        float clamp_scale = __fsqrts((ellipse_bound * ellipse_bound)
                / (ellipse_term_y * ellipse_term_y + ellipse_term_x * ellipse_term_x));
        offset_x = clamp_scale * offset_x;
        screen_point.n[0] = offset_x;
        offset_y = clamp_scale * offset_y;
        screen_point.n[1] = offset_y;
        if ( (arrow->flags & (1u << _hud_waypoint_dont_rotate_offscreen)) == 0 )
            /* DEVIATION: decompiler rendered this as atan2(x, (float)window_width); the disassembly
             * passes the clamped y offset in f2 — angle from the screen-up axis toward the target */
            rotation = (float)-atan2(offset_x, offset_y);
    }

    screen_point.n[0] = (float)((render.camera.viewport_bounds.__s1.x1 - render.camera.viewport_bounds.__s1.x0) / 2)
            + offset_x;
    screen_point.n[1] = (float)((render.camera.viewport_bounds.__s1.y1 - render.camera.viewport_bounds.__s1.y0) / 2)
            + offset_y;

    const bitmap_data *bitmap = nullptr;
    const real_rectangle2d *clip = nullptr;
    hud_retrieve_bitmap_and_bounding_rect(hud_globals->waypoint.arrow_bitmap.index,
            arrow->sequence_indices[arrow_state], 0, &bitmap, &clip);
    if ( !bitmap || !_texture_cache_bitmap_get_hardware_format((bitmap_data *)bitmap, 0, 1) )
        return;

    point2d point;
    point.__s1.x = (int16_t)(int)screen_point.n[0];
    point.__s1.y = (int16_t)(int)screen_point.n[1];

    int alpha = 255 * (int)arrow->opacity;
    if ( alpha < 0 )
        alpha = 0;
    else if ( alpha > 255 )
        alpha = 255;

    real_rgb_color color;
    pixel32_to_real_rgb_color(arrow->color, &color);
    /* the binary recomputes this clamp once per channel; the value is identical */
    float fade_factor = 1.0f - arrow->fade;
    if ( fade_factor < 0.0f )
        fade_factor = 0.0f;
    else if ( fade_factor > 1.0f )
        fade_factor = 1.0f;
    color.n[0] = fade_factor * color.n[0];
    color.n[1] = fade_factor * color.n[1];
    color.n[2] = fade_factor * color.n[2];
    if ( !render_distance )
    {
        /* shipped quirk: distance-less waypoints draw with the faded blue channel in the red slot */
        color.n[0] = color.n[2];
        color.n[1] = 0.0f;
        color.n[2] = 0.0f;
    }

    unsigned int color_argb = ((((((unsigned int)alpha << 8)
            | (uint8_t)(int64_t)(color.n[0] * 255.0f)) << 8)
            | (uint8_t)(int64_t)(color.n[1] * 255.0f)) << 8)
            | (uint8_t)(int64_t)(color.n[2] * 255.0f);
    hud_draw_bitmap_direct(bitmap, 4, &point, clip, arrow_scale, rotation, color_argb, 0);

    if ( arrow_state != 1 && render_distance )
    {
        hud_absolute_placement_definition placement;
        number_hud_element_definition numbers;
        memset(&placement, 0, sizeof(placement));
        memset(&numbers, 0, sizeof(numbers));

        float display_distance = distance * 3.0480001f;   /* world units -> meters */

        placement.corner = _hud_corner_top_left;
        numbers.digits = 3;
        numbers.number_flags = 5;
        numbers.fractional_digits = 1;
        numbers.colors.flash_color = color_argb;
        numbers.colors.color = color_argb;

        int16_t number_x = (int16_t)(int)((((clip->__s1.x1 - clip->__s1.x0) * (float)bitmap->width) * 0.5f)
                * arrow_scale * 0.33000001f + (float)point.__s1.x);
        numbers.placement.offset.__s1.x = number_x - render.camera.window_bounds.__s1.x0
                + render.camera.viewport_bounds.__s1.x0;
        int16_t number_y = (int16_t)(int)((((clip->__s1.y1 - clip->__s1.y0) * (float)bitmap->height) * 0.5f)
                * arrow_scale * 0.66000003f + (float)point.__s1.y);
        numbers.placement.offset.__s1.y = number_y - render.camera.window_bounds.__s1.y0
                + render.camera.viewport_bounds.__s1.y0;

        float decimal_modulus = (float)pow(10.0, 4.0);   /* computed at runtime in the binary */
        hud_calculate_point_no_safearea_fit = 1;
        hud_draw_numbers(local_player_index, &placement, &numbers,
                (int16_t)(int)display_distance,
                (int16_t)(int)(float)fmod(fabsf(decimal_modulus * display_distance), decimal_modulus),
                0, 0, 0.0f);
        hud_calculate_point_no_safearea_fit = 0;
    }
}
