/* hud_draw_players @0x83708B68 */
/* hud_draw_players 0x83708B94 — draw teammate markers on the local player's HUD. Collects every other player on
 * the local player's team that has a live unit, then for each projects the teammate's head position (raised
 * 0.3 wu) to screen space and, if on-screen, draws the team interface bitmap there. The marker's clip rectangle
 * height is faded with view depth: a distance factor 1 - ((depth - z_near) * 100) / (z_far - z_near) clamped to
 * [0.075, 1] feeds clip = {0, 1, (1 - factor) * 0.5, 1}. */

#include <stdint.h>
#include "headers/render_globals.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
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
extern int local_player_get_player_index(int16_t local_player_index);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern uint8_t render_camera_view_to_screen(const render_camera *camera, const render_frustum *frustum, const real_point3d *view_point, real_point2d *screen_point);
extern int interface_get_tag_index(int16_t interface_tag_index);
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern void interface_draw_bitmap(const bitmap_data *bitmap, const point2d *point, const real_rectangle2d *clip, float scale, float theta, float fade);

void hud_draw_players(void)
{
    int player_index = local_player_get_player_index(render.local_player_index);
    int local_team = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->team_index;
    if ( player_index == -1 )
        return;

    /* Gather every other player on the local player's team that currently has a unit. */
    int teammate_indices[16];
    int teammate_count = 0;
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for ( player_datum *player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator) )
    {
        if ( player_index != iterator.index && local_team == player->team_index && player->unit_index != -1 )
            teammate_indices[teammate_count++] = iterator.index;
    }
    if ( teammate_count <= 0 )
        return;

    int *teammate = teammate_indices;
    do
    {
        int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, *teammate)->unit_index;
        real_point3d head_position;
        unit_get_head_position(unit_index, &head_position);
        head_position.n[2] = head_position.n[2] + 0.30000001f;

        real_point3d view_position[2];
        matrix4x3_transform_point(&render.frustum.world_to_view, &head_position, view_position);
        real_point2d screen_position;
        if ( render_camera_view_to_screen(&render.camera, &render.frustum, view_position, &screen_position) )
        {
            bitmap_data *bitmap =
                bitmap_group_get_bitmap_from_sequence(
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
        ++teammate;
    }
    while ( --teammate_count );
}
