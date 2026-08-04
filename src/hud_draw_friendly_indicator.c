/* hud_draw_friendly_indicator @0x837089C0 — draw a single teammate marker on the HUD for `player_index`'s
 * unit. Single-player counterpart of hud_draw_players.c (which gathers and loops over every teammate); the
 * projection/fade/draw math is identical. */

#include <stdint.h>
#include "headers/render_globals.h"
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/bitmap_data.h"
#include "headers/real_point3d.h"
#include "headers/real_point2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/point2d.h"
#include "headers/real_matrix4x3.h"
#include "headers/interface_tag_index.h"
#include "headers/blam_data_globals.h"


#include "headers/render_camera.h"
#include "headers/render_frustum.h"
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern uint8_t render_camera_view_to_screen(const render_camera *camera, const render_frustum *frustum, const real_point3d *view_point, real_point2d *screen_point);
extern int interface_get_tag_index(int16_t interface_tag_index);
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern void interface_draw_bitmap(const bitmap_data *bitmap, const point2d *point, const real_rectangle2d *clip,
                                  float scale, float theta, float fade);

void hud_draw_friendly_indicator(int player_index)
{
    int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    real_point3d head_position;
    unit_get_head_position(unit_index, &head_position);
    head_position.n[2] = head_position.n[2] + 0.30000001f;

    real_point3d view_position[2];
    matrix4x3_transform_point(&render.frustum.world_to_view, &head_position, view_position);
    real_point2d screen_position;
    if ( render_camera_view_to_screen(&render.camera, &render.frustum, view_position, &screen_position) )
    {
        bitmap_data *bitmap = bitmap_group_get_bitmap_from_sequence(
                interface_get_tag_index(_interface_bitmap_multiplayer_hud), 0, 0);
        if ( _texture_cache_bitmap_get_hardware_format(bitmap, 0, 1u) )
        {
            point2d position;
            position.__s1.x = (int)screen_position.n[0] - render.camera.viewport_bounds.n[1];
            position.__s1.y = (int)screen_position.n[1] - render.camera.viewport_bounds.n[0];

            /* depth fade factor, clamped to [0.075, 1] */
            float depth_factor = 1.0f - ((-view_position[0].n[2] - render.frustum.z_near) * 100.0f)
                                        / (render.frustum.z_far - render.frustum.z_near);
            float fade;
            if ( depth_factor >= 0.075000003f )
                fade = depth_factor <= 1.0f ? depth_factor : 1.0f;
            else
                fade = 0.075000003f;

            real_rectangle2d clip;
            clip.n[0] = 0.0f;
            clip.n[1] = 1.0f;
            clip.n[2] = (1.0f - fade) * 0.5f;
            clip.n[3] = 1.0f;
            interface_draw_bitmap(bitmap, &position, &clip, 1.0f, 0.0f, 1.0f);
        }
    }
}
