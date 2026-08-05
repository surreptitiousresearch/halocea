/* first_person_weapon_update @ 0x836A0508 — advance one local player's first-person viewmodel one tick:
 * validate the held weapon, drive the primary weapon-animation state machine (firing it forward through
 * animation_update_internal, spawning the triggered sound, advancing to the next state at end), drive the
 * "moving" overlay animation (on/off from unit speed and airborne state), drive the overcharged-jitter
 * overlay (state 4), run the sway/turn/firing-push-back springs while rendered, advance interpolation, and
 * run the idle-pose timer (randomly enter the "look at weapon" pose state 5 when idle long enough).
 * Finally counts down the predict timer and predicts when it elapses.
 *
 * Weapon/unit object data are reached via the object-header datum. The weapon definition is
 * TAG_INSTANCE(object def index); its first_person_animations reference names the animation graph.
 * "Unit speed" is the magnitude of the unit's throttle vector.
 *
 * Deviations:
 *  - The overcharged-jitter frame advance came through as long-double/fmod ABI artifacts (v18/v20/v26/v27);
 *    reconstructed from disasm 0x836A0840-0x836A085C as a single-precision fmod with a (double)frame_count
 *    divisor. frame_count = graph animations[index].frame_count.
 *  - accelerate_to_position's final circular_position argument (v19/v28/v29/v34) is an uninitialized stale
 *    register at every call site; passed 0 (all these springs are linear / pre-wrapped). */

#include <stdint.h>
#include <math.h>
#include "headers/weapon_datum_flags.h"
#include <stddef.h>
#include "headers/first_person_weapon.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/game_globals_tag.h"
#include "headers/game_globals_player_information.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_first_person_weapon_animations.h"
#include "headers/animation.h"


#include "headers/real_point3d.h"
#include "headers/animation_state.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void first_person_weapon_set_state(int16_t local_player_index, int16_t new_state, uint8_t reset_sounds);
extern void first_person_weapon_next_state(int16_t local_player_index);
extern void first_person_weapon_start_interpolation(int16_t local_player_index, int16_t frame_count);
extern void first_person_weapon_predict(int16_t local_player_index);
#include "headers/animation_update_kind.h"
extern int16_t animation_update_internal(animation_update_kind render_or_affects_game_state, int animation_graph_index, animation_state *state, int *triggered_sound_index);
extern int16_t director_get_perspective(int16_t local_player_index);
extern int object_impulse_sound_new(int object_index, int definition_index, int16_t node_index, const real_point3d *position, const real_vector3d *forward, float scale, uint8_t is_local_player);
extern uint8_t unit_flying_through_air(int unit_index);
extern uint8_t accelerate_to_position(float *position_reference, float *velocity_reference, float position_desired, float acceleration_maximum, float velocity_maximum, float position_lower_bound, float position_upper_bound, uint8_t circular_position);
extern float player_control_get_autoaim_level(int16_t local_player_index);
extern int16_t player_control_get_zoom_level(int16_t local_player_index);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern float real_seed_random(uint32_t *seed);


#include "headers/first_person_weapon_state.h"
#include "headers/first_person_weapon_animation.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"

void first_person_weapon_update(int16_t local_player_index)
{
    first_person_weapon *fpw = &first_person_weapons[local_player_index];

    int weapon_index = fpw->weapon_index;
    if ( weapon_index != -1 && !object_try_and_get_and_verify_type(weapon_index, object_mask_weapon) )
        fpw->weapon_index = -1;

    if ( fpw->unit_index != -1 && fpw->weapon_index != -1 )
    {
        int   state = fpw->state;
        weapon_datum *weapon_data = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, fpw->weapon_index)->datum;
        unit_datum   *unit_data   = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, fpw->unit_index)->datum;
        weapon_definition *definition = TAG_GET(weapon_definition, weapon_data->definition_index);
        int   animation_graph_index = definition->weapon.interface_definition.first_person_animations.index;
        animation_graph *graph = TAG_GET(animation_graph, animation_graph_index);

        if ( state == _first_person_weapon_state_overheated || state == _first_person_weapon_state_overheating )
        {
            if ( (weapon_data->weapon.flags & (1u << _weapon_overheated_exit_bit)) != 0 )
                first_person_weapon_set_state(local_player_index, _first_person_weapon_state_overheated_exit, 1u);
            if ( (weapon_data->weapon.flags & (1u << _weapon_overheated_bit)) == 0 )
                first_person_weapon_set_state(local_player_index, _first_person_weapon_state_idle, 1u);
        }

        int triggered_sound_index;
        if ( animation_update_internal(animation_update_kind_render_only, animation_graph_index,
                                       &fpw->state_animation, &triggered_sound_index) == 2 )
            first_person_weapon_next_state(local_player_index);

        if ( triggered_sound_index != -1 && !director_get_perspective(local_player_index) )
        {
            fpw->current_sound_index = object_impulse_sound_new(fpw->weapon_index, triggered_sound_index, -1,
                                                                global_origin3d, global_forward3d, 1.0f,
                                                                local_player_index);
            fpw->current_sound_state = fpw->state;
        }

        real_vector3d *throttle = &unit_data->unit.throttle;
        float speed_sq = throttle->__s1.k * throttle->__s1.k + (throttle->__s1.i * throttle->__s1.i + throttle->__s1.j * throttle->__s1.j);
        uint8_t moving = (sqrtf(speed_sq) > 0.1f) && (unit_flying_through_air(fpw->unit_index) == 0);

        if ( (uint16_t)fpw->moving_animation.index == 0xFFFF )
        {
            if ( moving )
            {
                animation_graph_first_person_weapon_animations *animation_set = graph->first_person_weapon_animations.count
                    ? (animation_graph_first_person_weapon_animations *)graph->first_person_weapon_animations.address
                    : 0;
                fpw->moving_animation.frame_index = 0;
                if ( animation_set->animations.count <= _first_person_weapon_animation_moving )
                    fpw->moving_animation.index = -1;
                else
                    fpw->moving_animation.index = ((int16_t *)animation_set->animations.address)[_first_person_weapon_animation_moving];
            }
        }
        else
        {
            animation_update_internal(animation_update_kind_render_only, animation_graph_index,
                                      &fpw->moving_animation, NULL);
            if ( !moving )
            {
                if ( !fpw->state )
                    first_person_weapon_start_interpolation(local_player_index, 6);
                fpw->moving_animation.index = -1;
            }
        }

        int   jitter_index = fpw->overcharged_jitter_animation.index;
        int16_t state16 = fpw->state;
        if ( jitter_index == -1 )
        {
            if ( state16 == _first_person_weapon_state_charged )
            {
                animation_graph_first_person_weapon_animations *animation_set = graph->first_person_weapon_animations.count
                    ? (animation_graph_first_person_weapon_animations *)graph->first_person_weapon_animations.address
                    : 0;
                fpw->overcharged_jitter_animation.frame_index = 0.0f;
                if ( animation_set->animations.count <= _first_person_weapon_animation_overcharged_jitter )
                    fpw->overcharged_jitter_animation.index = -1;
                else
                    fpw->overcharged_jitter_animation.index = ((int16_t *)animation_set->animations.address)[_first_person_weapon_animation_overcharged_jitter];
            }
        }
        else if ( state16 == _first_person_weapon_state_charged )
        {
            int16_t frame_count = ((animation *)graph->animations.address)[jitter_index].frame_count;
            double dividend = (double)(((weapon_data->weapon.overcharged + 1.0f) * 2.0f)
                                       + fpw->overcharged_jitter_animation.frame_index);
            fpw->overcharged_jitter_animation.frame_index = (float)fmod(dividend, (double)frame_count);
        }
        else
        {
            fpw->overcharged_jitter_animation.index = -1;
        }

        if ( fpw->rendered )
        {
            accelerate_to_position(fpw->position.n, fpw->position_velocity.n, throttle->__s1.i,
                                   0.079999998f, 0.5f, -1.0f, 1.0f, 0);
            accelerate_to_position(&fpw->position.n[1], &fpw->position_velocity.n[1], throttle->__s1.j,
                                   0.079999998f, 0.5f, -1.0f, 1.0f, 0);

            float yaw_delta = fpw->render_facing.n[0] - fpw->last_render_facing.n[0];
            if ( yaw_delta >= PI )
                yaw_delta -= TWO_PI;
            if ( yaw_delta <= -PI )
                yaw_delta += TWO_PI;
            float turn_x = yaw_delta * 30.0f;

            float pitch_delta = fpw->render_facing.n[1] - fpw->last_render_facing.n[1];
            if ( pitch_delta >= PI )
                pitch_delta -= TWO_PI;
            if ( pitch_delta <= -PI )
                pitch_delta += TWO_PI;
            float turn_y = pitch_delta * -30.0f;

            if ( turn_x >= -1.0f ) { if ( turn_x > 1.0f ) turn_x = 1.0f; } else turn_x = -1.0f;
            if ( turn_y >= -1.0f ) { if ( turn_y > 1.0f ) turn_y = 1.0f; } else turn_y = -1.0f;

            accelerate_to_position(fpw->turning.n, fpw->turning_velocity.n, turn_x,
                                   0.029999999f, 0.2f, -1.0f, 1.0f, 0);
            accelerate_to_position(&fpw->turning.n[1], &fpw->turning_velocity.n[1], turn_y,
                                   0.029999999f, 0.2f, -1.0f, 1.0f, 0);
        }

        accelerate_to_position(&fpw->firing_push_back, &fpw->firing_push_back_velocity, 0.0f,
                               0.0099999998f, 0.2f, 0.0f, 1.0f, 0);
        if ( fpw->firing_push_back == 1.0f )
            fpw->firing_push_back_velocity = 0.0f;

        if ( fpw->interpolation_frame_count > 0 )
        {
            int16_t next = (int16_t)(fpw->interpolation_frame_index + 1);
            fpw->interpolation_frame_index = next;
            if ( next >= fpw->interpolation_frame_count )
                fpw->interpolation_frame_count = 0;
        }

        if ( player_control_get_autoaim_level(local_player_index) != 0.0f
          || player_control_get_zoom_level(local_player_index) != -1
          || fpw->firing_push_back != 0.0f
          || fpw->position.n[0] != 0.0f
          || fpw->position.n[1] != 0.0f
          || fpw->turning.n[0] != 0.0f
          || fpw->turning.n[1] != 0.0f )
        {
            fpw->ticks_idle = 0;
            if ( (uint16_t)fpw->state == _first_person_weapon_state_posing )
                first_person_weapon_set_state(local_player_index, _first_person_weapon_state_idle, 1u);
        }
        else if ( fpw->state )
        {
            fpw->ticks_idle = 0;
        }
        else
        {
            game_globals_player_information *player_information = (game_globals_player_information *)global_game_globals->player_information.address;
            if ( !fpw->ticks_until_pose )
            {
                unsigned int *seed = get_global_local_random_seed_address();
                fpw->ticks_until_pose = (int16_t)(int)(real_seed_random_range(seed, player_information->first_person_idle_time_lower_bound,
                                                                              player_information->first_person_idle_time_upper_bound) * 30.0f);
            }
            int idle = (int16_t)(fpw->ticks_idle + 1);
            fpw->ticks_idle = idle;
            if ( idle > fpw->ticks_until_pose )
            {
                fpw->ticks_until_pose = 0;
                if ( real_seed_random(get_global_local_random_seed_address()) >= player_information->first_person_idle_skip_fraction )
                    first_person_weapon_set_state(local_player_index, _first_person_weapon_state_posing, 1u);
            }
        }
    }

    int16_t predict = (int16_t)(fpw->ticks_until_predict - 1);
    fpw->ticks_until_predict = predict;
    if ( predict <= 0 )
        first_person_weapon_predict(local_player_index);
}
