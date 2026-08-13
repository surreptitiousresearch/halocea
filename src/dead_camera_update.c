/* dead_camera_update @ 0x837C3400 — camera proc for the death/respawn (spectator) camera. It focuses on
 * the watched unit (or the last frozen position if that unit is gone), orbiting at the random distance
 * and orientation chosen in dead_camera_new. The death-cam timer and the spectator switch_timer count
 * down; when switch_timer elapses (and the game is not paused) it advances to the next player that has a
 * unit (preferring a teammate), retargets that player's unit, and rearms switch_timer (15s in a running
 * game engine, else 3s). */

#include <stdint.h>
#include "headers/dead_camera.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
#include "headers/real_vector3d.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_euler_angles2d.h"
#include "headers/real_vector3d.h"
#include "headers/observer_time_flags.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern void observer_up_from_forward(const real_vector3d *forward, real_vector3d *up);
extern uint8_t game_time_get_paused(void);
extern int player_get_next_player_with_a_unit(int player_index, int old_player_index, uint8_t match_team);
extern void *datum_try_and_get(const data_array *data, int index);
extern uint8_t game_engine_running(void);

void dead_camera_update(dead_camera *camera, const camera_control *controls, observer_command *result)
{
    float *unit_object;
    double switch_timer;

    /* focus on the watched unit's camera position, or the last frozen position if it's gone */
    if ( camera->unit_index == -1
      || (unit_object = object_try_and_get_and_verify_type(camera->unit_index, object_mask_all)) == 0 )
    {
        result->___u1.__s0.focus_position.n[0] = camera->position.n[0];
        result->___u1.__s0.focus_position.n[1] = camera->position.n[1];
        result->___u1.__s0.focus_position.n[2] = camera->position.n[2];
    }
    else
    {
        result->___u1.__s0.focus_position.n[0] = unit_object[40];
        result->___u1.__s0.focus_position.n[1] = unit_object[41];
        result->___u1.__s0.focus_position.n[2] = unit_object[42];
    }

    result->___u1.__s0.focus_distance = camera->distance;
    vector3d_from_euler_angles2d(&result->___u1.__s0.forward, &camera->orientation);
    observer_up_from_forward(&result->___u1.__s0.forward, &result->___u1.__s0.up);
    result->___u1.__s0.field_of_view = camera->field_of_view;
    result->___u1.__s0.focus_offset = *global_zero_vector3d;
    result->focus_velocity = *global_zero_vector3d;
    result->flags = 1;

    result->___u4.__s0.position_flags = ((1u << _observer_time_valid_bit) | (1u << _observer_time_force_bit));
    result->timer = camera->timer >= 0.0f ? camera->timer : 0.0f;   /* fsel: max(timer, 0) */
    result->___u5.__s0.position_timer = 0.0f;
    if ( camera->timer == 3.0f )
    {
        result->___u5.__s0.distance_timer = 0.0f;
        result->___u4.__s0.distance_flags = ((1u << _observer_time_valid_bit) | (1u << _observer_time_force_bit));
        result->___u1.__s0.focus_distance = 0.5f;
    }

    switch_timer = camera->switch_timer;
    camera->timer = camera->timer - controls->seconds_elapsed;
    if ( ((float)switch_timer - controls->seconds_elapsed) >= 0.0f )
        camera->switch_timer = ((float)switch_timer - controls->seconds_elapsed);
    else
        camera->switch_timer = 0.0f;

    if ( camera->switch_timer == 0.0f && !game_time_get_paused() )
    {
        int player_index = camera->player_index;
        int next_unit = -1;
        uint8_t found_teammate = 0;
        int team = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->team_index;
        data_iterator iter;
        player_datum *player;
        int next_player;

        /* is there another player on the same team (so we prefer to spectate a teammate)? */
        data_iterator_new(&iter, player_data);
        for ( player = data_iterator_next(&iter); player; player = data_iterator_next(&iter) )
        {
            if ( iter.index != player_index && player->team_index == team )
            {
                found_teammate = 1;
                break;
            }
        }

        next_player = player_get_next_player_with_a_unit(camera->player_index, camera->current_player_index,
                                                         found_teammate);
        camera->current_player_index = next_player;
        if ( next_player != -1 )
        {
            if ( !datum_try_and_get(player_data, next_player) )
                camera->current_player_index = camera->player_index;
            next_unit = DATA_ARRAY_ELEMENT(player_data, player_datum, camera->current_player_index)->unit_index;
        }

        if ( next_unit != camera->unit_index && next_unit != -1 )
        {
            camera->unit_index = next_unit;
            camera->timer = 3.0f;
        }

        camera->switch_timer = game_engine_running() ? 15.0f : 3.0f;
    }
}
