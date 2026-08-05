/* get_local_player_input_blob @ 0x836DF628 — build one local player's digested control input for this
 * frame. It clears the output input_blob, then (if the player exists and owns a gamepad) reads the
 * abstracted input state and gamepad state and fills in:
 *   - throttle (forward/strafe), with circular clamping to the unit disc;
 *   - facing_delta (yaw/pitch look), via either the "fancy" look path (piecewise sensitivity curve,
 *     doubled-spin, look acceleration, zoom scaling, aim magnetism) or the simple linear path, depending
 *     on player_control_camera_control_is_active() and the abstraction's look_use_fancy flag;
 *   - primary_trigger and the unit/player control flag bit-vectors, derived from per-button press
 *     durations (gated by the per-player inhibited-button mask);
 *   - accept/back digital buttons.
 * Finally it runs the reset/action-test pass over the blob.
 *
 * NOTE on signature: the database prototype is
 *   void get_local_player_input_blob(__int16 local_player_index, float seconds_elapsed, input_blob *input);
 * The decompiler mis-split the float arg and invented a phantom 3rd pointer, using "a4" as the real
 * output. "input" below is that real output pointer. */

#include <stdint.h>
#include "headers/input_blob.h"
#include "headers/game_input_state.h"
#include "headers/gamepad_state.h"
#include "headers/player_control_globals.h"
#include "headers/game_globals_tag.h"
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/biped_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/game_globals_player_control.h"
#include "headers/game_globals_player_information.h"
#include "headers/unit_control_flags.h"
#include "headers/player_control_flags.h"
#include "headers/control_button.h"
#include "headers/biped_datum_flags.h"
#include "headers/object_type.h"
#include "headers/math_constants.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern uint8_t input_has_gamepad(int16_t gamepad_index);
extern gamepad_state *input_get_gamepad_state(int16_t gamepad_index);
extern game_input_state *input_abstraction_get_input_state(int16_t local_player_index);
extern uint8_t player_control_camera_control_is_active(void);
extern float evaluate_piecewise_linear_function(int16_t count, const float *function, float x);
extern float unit_get_zoom_magnification(int unit_index, int16_t zoom_level);
extern int local_player_aim_assist(int16_t local_player_index, float *autoaim_level, float *magnetism_level, real_euler_angles2d *target_angular_position, real_euler_angles2d *target_angular_velocity);
extern int local_player_compute_autoaim_level_and_target_object_index_only(int16_t local_player_index, float *autoaim_level);
extern float game_time_get_speed(void);
extern uint8_t game_players_are_double_speed(void);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void player_control_action_test_check_reset_input_blob(input_blob *input);
extern double __fabs(double x);
extern float  __fsqrts(float x);
extern void  *memset(void *dst, int value, unsigned int count);

/* object data pointer for an object handle (object header datum: data ptr at int element 3*idx+2) */
static unit_datum *object_data_ptr(int object_handle)
{
    return ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_handle)->datum);
}

void get_local_player_input_blob(int16_t local_player_index, float seconds_elapsed, input_blob *input)
{
    int player_index;

    player_index = local_player_get_player_index(local_player_index);

    /* clear the whole 32-byte blob */
    memset(input, 0, sizeof(input_blob));   /* recovered: 8-dword pun-clear loop -> memset */

    if ( player_index == -1 )
    {
        player_control_action_test_check_reset_input_blob(input);
        return;
    }

    {
        player_datum   *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        player_control *control = &player_control_globals->players[local_player_index];
        int16_t         machine_index = (uint16_t)player->local_player_index;
        int             unit_index = player->unit_index;

        if ( machine_index != (int16_t)0xFFFF && input_has_gamepad(machine_index) )
        {
            const gamepad_state *gamepad = input_get_gamepad_state(machine_index);
            /* player-control tuning constants for this player's control tag-block element */
            game_globals_player_control *control_definition = (game_globals_player_control *)global_game_globals->player_control.address;
            game_input_state *input_state = input_abstraction_get_input_state(machine_index);
            float look_yaw_rate = 0.0f;
            float look_pitch_rate = 0.0f;

            /* base look rates: weapon's per-trigger turn rate if held, else the player default */
            if ( unit_index != -1 )
            {
                unit_datum *unit_object = object_data_ptr(unit_index);
                int parent_object_index = unit_object->object.parent_object_index;

                look_yaw_rate   = player_look_yaw_rate[local_player_index]   * DEG_TO_RAD * SECONDS_PER_TICK;
                look_pitch_rate = player_look_pitch_rate[local_player_index] * DEG_TO_RAD * SECONDS_PER_TICK;

                if ( parent_object_index != -1 )
                {
                    int16_t seat_index = unit_object->unit.parent_seat_index;
                    if ( seat_index != -1 )
                    {
                        int parent_definition = object_data_ptr(parent_object_index)->definition_index;
                        unit_seat *seat = (unit_seat *)
                            (TAG_GET(unit_definition, parent_definition))->unit.seats.address + seat_index;
                        if ( seat->yaw_rate > 0.0f )
                            look_yaw_rate   = seat->yaw_rate * DEG_TO_RAD * SECONDS_PER_TICK;
                        if ( seat->pitch_rate > 0.0f )
                            look_pitch_rate = seat->pitch_rate * DEG_TO_RAD * SECONDS_PER_TICK;
                    }
                }
            }

            input->throttle.n[0] = input_state->forward_movement;
            input->throttle.n[1] = input_state->strafe;

            /* circular look normalization: scale so a diagonal stick reads as unit length */
            {
                float abs_pitch = (float)__fabs(input_state->pitch);
                float abs_yaw   = (float)__fabs(input_state->yaw);
                float look_scale = 1.0f;
                float clamped_yaw, clamped_pitch;

                if ( abs_pitch > 0.1f && abs_yaw > 0.1f )
                {
                    /* minor-axis ratio on one axis, 1.0 on the major axis */
                    float a, b;
                    if ( abs_pitch <= abs_yaw ) { a = abs_pitch / abs_yaw; b = 1.0f; }
                    else                        { a = 1.0f; b = abs_yaw / abs_pitch; }
                    look_scale = __fsqrts(a * a + b * b);
                }

                clamped_yaw = input_state->yaw * look_scale;
                if ( clamped_yaw < -1.0f ) clamped_yaw = -1.0f;
                else if ( clamped_yaw > 1.0f ) clamped_yaw = 1.0f;

                clamped_pitch = input_state->pitch * look_scale;
                if ( clamped_pitch < -1.0f ) clamped_pitch = -1.0f;
                else if ( clamped_pitch > 1.0f ) clamped_pitch = 1.0f;

                if ( player_control_camera_control_is_active() )
                {
                    if ( input_state->look_use_fancy )
                    {
                        /* doubled-spin: when enabled and the doubled-spin button is held, the swap state
                         * flips the per-axis spin multiplier (1 or 2). The yaw and pitch axes derive the
                         * multiplier through slightly different gates (faithful to the binary). */
                        int spin_swap = controls_swap_doubled_spin_state;
                        int spin_pitch, spin_yaw;
                        if ( input_state->buttons[_button_scope_zoom] && controls_enable_doubled_spin )
                            spin_pitch = (spin_swap == 0);
                        else
                            spin_pitch = spin_swap;
                        if ( input_state->buttons[_button_scope_zoom] == 0 || !controls_enable_doubled_spin )
                            spin_yaw = spin_swap;
                        else
                            spin_yaw = (spin_swap == 0);

                        {
                            real_euler_angles2d aim_assist_buffer[10];   /* aim-assist output scratch */
                            real_euler_angles2d aim_angles;              /* aim-assist angular delta */
                            int   point_count = control_definition->look_function.count;
                            const float *curve_points = (const float *)control_definition->look_function.address;
                            /* DEVIATION: decompiler left the pitch curve pointer/spin factor uninitialized
                             * (v45/v47); disasm shows both axes share curve_points and pitch uses
                             * (spin_pitch+1). */
                            float yaw_curve   = evaluate_piecewise_linear_function(point_count, curve_points, clamped_yaw);
                            float pitch_curve = evaluate_piecewise_linear_function(point_count, curve_points, clamped_pitch);
                            float yaw_delta   = yaw_curve   * (float)(spin_yaw + 1)   * look_yaw_rate;
                            float pitch_delta = pitch_curve * (float)(spin_pitch + 1) * look_pitch_rate;

                            /* zoom scaling: divide sensitivity by the active zoom magnification */
                            if ( unit_index != -1 )
                            {
                                int zoom_level = (uint16_t)control->desired_zoom_level;
                                if ( zoom_level != 0xFFFF )
                                {
                                    float inv_zoom = 1.0f / unit_get_zoom_magnification(unit_index, zoom_level);
                                    yaw_delta   *= inv_zoom;
                                    pitch_delta *= inv_zoom;
                                }
                            }

                            /* per-unit look-sensitivity scale from the unit's body stun and the
                             * player_information look-magnetism scale (element float[33]) */
                            if ( unit_index != -1 )
                            {
                                game_globals_player_information *player_info = (game_globals_player_information *)global_game_globals->player_information.address;
                                unit_datum *unit_object = object_data_ptr(unit_index);
                                float scale = -(unit_object->unit.body_stun * player_info->stun_turning_penalty - 1.0f);
                                yaw_delta   *= scale;
                                pitch_delta *= scale;
                            }

                            /* look acceleration: ramp up yaw sensitivity while the stick is held past the
                             * pegging threshold) */
                            if ( (float)__fabs(clamped_yaw) < control_definition->look_pegging_threshold )
                            {
                                control->look_acceleration_time = 0.0f;
                            }
                            else
                            {
                                float t = control->look_acceleration_time / control_definition->look_acceleration_time;
                                if ( t < 0.0f ) t = 0.0f;
                                else if ( t > 1.0f ) t = 1.0f;
                                control->look_acceleration_time += seconds_elapsed;
                                yaw_delta *= (control_definition->look_acceleration_scale - 1.0f) * t + 1.0f;
                            }

                            /* aim assist (autoaim/magnetism) */
                            control->target_object_index = local_player_aim_assist(
                                                               local_player_index,
                                                               &control->autoaim_level,
                                                               &control->magnetism_level,
                                                               aim_assist_buffer,
                                                               &aim_angles);

                            if ( player_magnetism_flag
                              && control->magnetism_level > 0.0f
                              && ( (float)__fabs(clamped_yaw) > 0.000099999997f
                                || (float)__fabs(clamped_pitch) > 0.000099999997f
                                || (float)__fabs(input->throttle.n[0]) > 0.000099999997f
                                || (float)__fabs(input->throttle.n[1]) > 0.000099999997f ) )
                            {
                                float game_speed = game_time_get_speed();
                                float friction = control_definition->magnetism_friction;
                                float range    = control_definition->magnetism_adhesion;
                                float keep, mag;
                                float mag_yaw, mag_pitch;

                                if ( friction < 0.0f ) friction = 0.0f;
                                else if ( friction > 1.0f ) friction = 1.0f;
                                keep = -(control->magnetism_level * friction - 1.0f);

                                if ( range < 0.0f ) range = 0.0f;
                                else if ( range > 1.0f ) range = 1.0f;
                                mag = control->magnetism_level * range;

                                if ( game_players_are_double_speed() )
                                    game_speed *= 0.5f;

                                mag_yaw   = aim_angles.n[0] * game_speed;
                                mag_pitch = aim_angles.n[1] * game_speed;
                                if ( mag_yaw < -0.10471976f ) mag_yaw = -0.10471976f;
                                else if ( mag_yaw > 0.10471976f ) mag_yaw = 0.10471976f;
                                if ( mag_pitch < -0.052359879f ) mag_pitch = -0.052359879f;
                                else if ( mag_pitch > 0.052359879f ) mag_pitch = 0.052359879f;

                                yaw_delta   = mag_yaw   * mag + keep * yaw_delta;
                                pitch_delta = mag_pitch * mag + keep * pitch_delta;
                            }

                            input->facing_delta.n[0] = (seconds_elapsed * 30.0f) * yaw_delta;
                            input->facing_delta.n[1] = (seconds_elapsed * 30.0f) * pitch_delta;
                        }
                    }
                    else
                    {
                        /* simple linear look path */
                        float inv_scale = 1.0f;
                        input->throttle.n[0] = input_state->forward_movement;
                        input->throttle.n[1] = input_state->strafe;

                        if ( unit_index != -1 )
                        {
                            int zoom_level = (uint16_t)control->desired_zoom_level;
                            if ( zoom_level != 0xFFFF )
                                inv_scale = 1.0f / unit_get_zoom_magnification(unit_index, zoom_level);
                        }
                        if ( unit_index != -1 )
                        {
                            unit_datum *unit_object = object_data_ptr(unit_index);
                            game_globals_player_information *player_info = (game_globals_player_information *)global_game_globals->player_information.address;
                            inv_scale = -(unit_object->unit.body_stun * player_info->stun_turning_penalty - 1.0f) * inv_scale;
                        }

                        {
                            float yaw_delta = inv_scale * input_state->yaw;
                            float pitch_delta = input_state->pitch * inv_scale;
                            if ( yaw_delta < -4.5f ) yaw_delta = -4.5f;
                            else if ( yaw_delta > 4.5f ) yaw_delta = 4.5f;
                            if ( pitch_delta < -2.3f ) pitch_delta = -2.3f;
                            else if ( pitch_delta > 2.3f ) pitch_delta = 2.3f;
                            input->facing_delta.n[0] = yaw_delta;
                            input->facing_delta.n[1] = pitch_delta;
                        }
                        control->target_object_index =
                            local_player_compute_autoaim_level_and_target_object_index_only(
                                local_player_index, &control->autoaim_level);
                    }
                }
                else
                {
                    input->facing_delta.n[0] = 0.0f;
                    input->facing_delta.n[1] = 0.0f;
                }
            }

            /* ---------------- button -> control-flag mapping ---------------- */
            {
                unsigned char effective_buttons[NUMBER_OF_ACTION_CONTROL_BUTTONS];
                unsigned int  bit;
                int           b;

                memset(effective_buttons, 0, NUMBER_OF_ACTION_CONTROL_BUTTONS);

                /* clear inhibited buttons that the player has now released (reset-when-released latch) */
                {
                    uint16_t reset_mask = control->reset_button_when_released_bit_vector;
                    uint16_t inhibited  = control->inhibited_button_bit_vector;
                    uint16_t pending    = (uint16_t)(inhibited & reset_mask);
                    if ( pending != 0 )
                    {
                        bit = 1;
                        for ( b = 0; b < NUMBER_OF_ACTION_CONTROL_BUTTONS; ++b )
                        {
                            if ( (pending & bit) != 0 && !input_state->buttons[b] )
                            {
                                control->inhibited_button_bit_vector &= ~(uint16_t)bit;
                                control->reset_button_when_released_bit_vector &= ~(uint16_t)bit;
                            }
                            bit = (bit << 1) | (bit >> 31);
                        }
                    }
                }

                /* copy through the button press durations that are not inhibited */
                {
                    uint16_t inhibited = control->inhibited_button_bit_vector;
                    bit = 1;
                    for ( b = 0; b < NUMBER_OF_ACTION_CONTROL_BUTTONS; ++b )
                    {
                        if ( (inhibited & bit) == 0 )
                            effective_buttons[b] = input_state->buttons[b];
                        bit = (bit << 1) | (bit >> 31);
                    }
                }

                /* crouch (only while not sprinting/at speed) */
                if ( unit_index != -1 )
                {
                    biped_datum *biped = object_try_and_get_and_verify_type(unit_index, object_mask_biped);
                    if ( biped )
                    {
                        if ( controls_enable_crouch
                          || (biped->biped.flags & (1u << _biped_airborne_bit)) != 0
                          || (input->throttle.n[1] * input->throttle.n[1]
                              + input->throttle.n[0] * input->throttle.n[0]) < 0.96040004f )
                        {
                            if ( effective_buttons[_button_crouch] ) input->unit_control_flags |= (1u << _unit_control_crouch_modifier_bit);
                            else                                     input->unit_control_flags &= ~(1u << _unit_control_crouch_modifier_bit);
                        }
                    }
                }

                input->primary_trigger = (float)input_state->buttons[_button_fire] * 0.0039215689f;

                if ( effective_buttons[_button_melee_attack] ) input->unit_control_flags |= (1u << _unit_control_weapon_primary_trigger_bit);
                else                                           input->unit_control_flags &= ~(1u << _unit_control_weapon_primary_trigger_bit);

                if ( effective_buttons[_button_throw_grenade] ) input->unit_control_flags |= (1u << _unit_control_throw_grenade_bit);
                else                                            input->unit_control_flags &= ~(1u << _unit_control_throw_grenade_bit);

                if ( effective_buttons[_button_throw_grenade] ) input->unit_control_flags |= (1u << _unit_control_weapon_secondary_trigger_bit);
                else                                            input->unit_control_flags &= ~(1u << _unit_control_weapon_secondary_trigger_bit);

                if ( effective_buttons[_button_start] == 1 ) input->player_control_flags |= (1u << _player_control_zoom_bit);
                else                                         input->player_control_flags &= ~(1u << _player_control_zoom_bit);

                if ( effective_buttons[_button_action_reload] ) input->unit_control_flags |= (1u << _unit_control_action_bit);
                else                                            input->unit_control_flags &= ~(1u << _unit_control_action_bit);

                /* held long enough (>= threshold ticks) sets the "held" action bit */
                if ( !(effective_buttons[_button_action_reload] < control_definition->minimum_weapon_swap_ticks) ) input->unit_control_flags |= (1u << _unit_control_swap_weapons_bit);
                else                                                     input->unit_control_flags &= ~(1u << _unit_control_swap_weapons_bit);

                if ( effective_buttons[_button_flashlight] ) input->unit_control_flags |= (1u << _unit_control_integrated_light_bit);
                else                                         input->unit_control_flags &= ~(1u << _unit_control_integrated_light_bit);

                if ( effective_buttons[_button_switch_weapon] ) input->unit_control_flags |= (1u << _unit_control_jump_bit);
                else                                            input->unit_control_flags &= ~(1u << _unit_control_jump_bit);

                if ( effective_buttons[_button_fire] ) input->unit_control_flags |= (1u << _unit_control_use_equipment_bit);
                else                                   input->unit_control_flags &= ~(1u << _unit_control_use_equipment_bit);

                if ( effective_buttons[_button_jump] == 1 ) input->player_control_flags |= (1u << _player_control_rotate_weapons_bit);
                else                                        input->player_control_flags &= ~(1u << _player_control_rotate_weapons_bit);

                if ( effective_buttons[_button_switch_grenade] == 1 ) input->player_control_flags |= (1u << _player_control_rotate_grenades_bit);
                else                                                  input->player_control_flags &= ~(1u << _player_control_rotate_grenades_bit);

                if ( (control->inhibited_button_bit_vector & (1u << _button_back)) == 0 )
                    input->back = gamepad->buttons[1];   /* physical gamepad button (gamepad_state order, not control_button) */
                if ( (control->inhibited_button_bit_vector & (1u << _button_action_reload)) == 0 )
                    input->accept = gamepad->buttons[0]; /* physical gamepad button (gamepad_state order, not control_button) */
            }
        }

        /* clamp throttle to the unit disc */
        {
            float strafe = input->throttle.n[1];
            float mag_sq = input->throttle.n[1] * input->throttle.n[1]
                         + input->throttle.n[0] * input->throttle.n[0];
            if ( mag_sq > 1.0f )
            {
                float inv = 1.0f / __fsqrts(mag_sq);
                input->throttle.n[0] = input->throttle.n[0] * inv;
                input->throttle.n[1] = strafe * inv;
            }
        }
    }

    player_control_action_test_check_reset_input_blob(input);
}
