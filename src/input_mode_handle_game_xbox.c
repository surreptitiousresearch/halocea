/* input_mode_handle_game_xbox @0x83725748 — per-frame gamepad -> game-input mapping for the four local
 * controllers. For each controller with a live gamepad it: latches the look yaw/pitch rate preferences,
 * normalizes both thumbsticks through an octagonal gate (each axis divided by the larger of |cos a|/|sin a|
 * so the dominant axis reaches full scale, then scaled by 1/32767), optionally snaps the sticks to the
 * cardinal/diagonal references when the "digital-ish" control schemes (joystick_controls 2 or 3) are
 * selected, maps the 13 game buttons through the per-player control map, and applies one of four
 * control schemes (joystick_controls 0..3) to forward/strafe/yaw/pitch. When a controller that was
 * available last frame loses its gamepad it raises the appropriate deferred controller-disconnect error.
 *
 * The database decompile is reg-alloc-failed and renders every stick int->float conversion and the paired
 * atan2/sin/cos arguments as long-double/OVERLAPPED puns; those are restored here to plain float math from
 * the disassembly (atan2 arg order verified: atan2f((float)n[1], (float)n[0])). The stick normalization and
 * the snap-to-reference step each appear twice, identically, in the codegen (once per stick); reconstructed
 * as the shared inline helpers the original source almost certainly used. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/gamepad_state.h"
#include "headers/gamepad_button.h"
#include "headers/button_preset_flags.h"
#include "headers/error_type.h"
#include "headers/blam_data_globals.h"

#include "headers/network_game_client.h"
extern double atan2(double y, double x);
extern double sin(double x);
extern double cos(double x);
extern double __fabs(double x);
extern float __fsqrts(float x);

extern gamepad_state *input_get_gamepad_state(int16_t gamepad_index);
extern uint8_t input_abstraction_input_state_idle(const game_input_state *new_state, const game_input_state *old_state);
extern uint8_t local_player_is_piloting_aircraft(int16_t controller_index);
extern uint8_t main_menu_is_active(void);
extern int16_t player_ui_get_single_player_local_player_controller(int16_t local_player_index);
extern uint8_t player_ui_local_player_wants_to_play_multiplayer(int16_t local_player_index);
extern network_game_client *global_network_game_client_get(void);
extern uint8_t local_player_exists(int local_player_index);
extern uint8_t virtual_keyboard_active(void);
extern uint8_t virtual_keyboard_close(void);
extern void display_error_deferred(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);

/* per-controller look rates published for the camera/turning code */

/* per-quadrant diagonal reference angles: {pi/4, 3pi/4, -pi/4, -3pi/4} */
extern const float reference_values[4];

#define SHORT_TO_UNIT 0.000030518509 /* 1/32767 */

/* Normalize one thumbstick through the octagonal gate. Divides both raw axes by the larger of
 * |cos angle| / |sin angle| so the dominant axis saturates at full scale, scales by 1/32767, and clamps
 * to [-1, 1]. Also returns the raw stick angle for the snap step. */
static void input_normalize_stick(const point2d *stick, float *out_x, float *out_y, float *out_angle)
{
    float angle = (float)atan2((float)stick->n[1], (float)stick->n[0]);
    float sine = (float)sin(angle);
    float cosine = (float)cos(angle);
    float dominant = (__fabs(sine) <= __fabs(cosine)) ? (float)cos(angle) : (float)sin(angle);
    float inv_dominant = (float)(1.0 / __fabs(dominant));

    float x = (float)(((float)stick->n[0] * inv_dominant) * SHORT_TO_UNIT);
    if (x < -1.0f)
        x = -1.0f;
    else if (x > 1.0f)
        x = 1.0f;
    *out_x = x;

    float y = (float)(((float)stick->n[1] * inv_dominant) * SHORT_TO_UNIT);
    if (y < -1.0f)
        y = -1.0f;
    else if (y > 1.0f)
        y = 1.0f;
    *out_y = y;

    *out_angle = angle;
}

/* Snap a normalized stick toward the nearest cardinal/diagonal for the digital control schemes.
 * ref_angle is (raw_angle - reference_values[quadrant]); snap_threshold gates the hard snap. */
static void input_snap_stick(float raw_angle, float ref_angle, float snap_threshold,
                             float *axis_x, float *axis_y)
{
    float magnitude = __fsqrts(((*axis_y * *axis_y) + (*axis_x * *axis_x)));
    float sign;

    if (__fabs(ref_angle) >= snap_threshold)
    {
        if (__fabs(*axis_x) <= __fabs(*axis_y))
        {
            sign = (*axis_y >= 0.0f) ? 1.0f : -1.0f;
            *axis_x = 0.0f;
            *axis_y = (sign * magnitude);
        }
        else
        {
            sign = (*axis_x >= 0.0f) ? 1.0f : -1.0f;
            *axis_y = 0.0f;
            *axis_x = (sign * magnitude);
        }
    }
    else
    {
        float blend = -(float)(__fabs(ref_angle) * 1.637022199626223 - 1.0);
        float a = __fabs(raw_angle);
        if (a < 0.78539819f || a > 2.3561945f) /* horizontal-dominant octant */
        {
            sign = (*axis_x >= 0.0f) ? 1.0f : -1.0f;
            *axis_x = (sign * magnitude);
            sign = (*axis_y >= 0.0f) ? 1.0f : -1.0f;
            *axis_y = (blend * (sign * magnitude));
        }
        else /* vertical-dominant octant */
        {
            sign = (*axis_y >= 0.0f) ? 1.0f : -1.0f;
            *axis_y = (sign * magnitude);
            sign = (*axis_x >= 0.0f) ? 1.0f : -1.0f;
            *axis_x = (blend * (sign * magnitude));
        }
    }
}

void input_mode_handle_game_xbox(void)
{
    for (int controller = 0; controller < 4; ++controller)
    {
        input_abstraction_globals.input_state[controller].look_use_fancy = 1;

        const gamepad_state *gamepad = input_get_gamepad_state(controller);
        if (gamepad)
        {
            game_input_state *new_state = &input_abstraction_globals.input_state[controller];
            game_input_preferences *prefs = &input_abstraction_globals.player_control_settings[controller];

            /* DEVIATION: decompiler renders this as a 9-dword copy loop from
             * &player_control_settings[controller+3].invert_look+4, which resolves to input_state[controller]. */
            game_input_state previous_state = input_abstraction_globals.input_state[controller];

            player_look_yaw_rate[controller] = prefs->yaw_rate;
            player_look_pitch_rate[controller] = prefs->pitch_rate;

            float movement_x, movement_y, movement_angle;
            float look_x, look_y, look_angle;
            input_normalize_stick(&gamepad->sticks[0], &movement_x, &movement_y, &movement_angle);
            input_normalize_stick(&gamepad->sticks[1], &look_x, &look_y, &look_angle);

            /* map the 13 abstract game controls to physical gamepad buttons for this player */
            for (int button = 0; button < 13; ++button)
                new_state->buttons[button] = gamepad->buttons[prefs->game_control_to_xbox_buttons[button]];

            int joystick_controls = prefs->joystick_controls;
            if (joystick_controls == _joystick_preset_legacy || joystick_controls == _joystick_preset_legacy_south_paw)
            {
                int move_quadrant = (movement_y < 0.0f ? 2 : 0) | (movement_x < 0.0f);
                int look_quadrant = (look_y < 0.0f ? 2 : 0) | (look_x < 0.0f);
                input_snap_stick(movement_angle, (movement_angle - reference_values[move_quadrant]),
                                 0.6108652651309967f, &movement_x, &movement_y);
                input_snap_stick(look_angle, (look_angle - reference_values[look_quadrant]),
                                 0.17453292f, &look_x, &look_y);
            }

            unsigned __int8 invert_pitch = prefs->invert_look;
            if (!invert_pitch && prefs->invert_look_aircraft_control)
                invert_pitch = local_player_is_piloting_aircraft((__int16)controller);

            unsigned int control_mode = (unsigned __int16)prefs->joystick_controls;
            if (control_mode <= 3)
            {
                if (control_mode == _joystick_preset_standard)
                {
                    if (gamepad->buttons[_gamepad_binary_button_dpad_left])
                        new_state->strafe = 1.0f;
                    else if (gamepad->buttons[_gamepad_binary_button_dpad_right])
                        new_state->strafe = -1.0f;
                    else
                        new_state->strafe = -movement_x;

                    if (gamepad->buttons[_gamepad_binary_button_dpad_up])
                        new_state->forward_movement = 1.0f;
                    else if (gamepad->buttons[_gamepad_binary_button_dpad_down])
                        new_state->forward_movement = -1.0f;
                    else
                        new_state->forward_movement = movement_y;

                    new_state->yaw = -look_x;
                    new_state->pitch = invert_pitch ? (-1.0f * look_y) : (1.0f * look_y);
                }
                else if (control_mode == _joystick_preset_south_paw)
                {
                    if (gamepad->buttons[_gamepad_binary_button_dpad_left])
                        new_state->yaw = 1.0f;
                    else if (gamepad->buttons[_gamepad_binary_button_dpad_right])
                        new_state->yaw = -1.0f;
                    else
                        new_state->yaw = -movement_x;

                    /* pitch: d-pad up/down force full deflection, otherwise the movement stick drives it */
                    if (gamepad->buttons[_gamepad_binary_button_dpad_up])
                        new_state->pitch = invert_pitch ? -1.0f : 1.0f;
                    else if (!gamepad->buttons[_gamepad_binary_button_dpad_down])
                        new_state->pitch = invert_pitch ? (-1.0f * movement_y) : (1.0f * movement_y);
                    else
                        new_state->pitch = invert_pitch ? 1.0f : -1.0f;

                    new_state->strafe = -look_x;
                    new_state->forward_movement = look_y;
                }
                else if (control_mode == _joystick_preset_legacy)
                {
                    if (gamepad->buttons[_gamepad_binary_button_dpad_left])
                        new_state->yaw = 1.0f;
                    else if (gamepad->buttons[_gamepad_binary_button_dpad_right])
                        new_state->yaw = -1.0f;
                    else
                        new_state->yaw = -movement_x;

                    if (gamepad->buttons[_gamepad_binary_button_dpad_up])
                        new_state->forward_movement = 1.0f;
                    else if (gamepad->buttons[_gamepad_binary_button_dpad_down])
                        new_state->forward_movement = -1.0f;
                    else
                        new_state->forward_movement = movement_y;

                    new_state->strafe = -look_x;
                    new_state->pitch = invert_pitch ? (-1.0f * look_y) : (1.0f * look_y);
                }
                else /* control_mode == _joystick_preset_legacy_south_paw */
                {
                    if (gamepad->buttons[_gamepad_binary_button_dpad_left])
                        new_state->strafe = 1.0f;
                    else if (gamepad->buttons[_gamepad_binary_button_dpad_right])
                        new_state->strafe = -1.0f;
                    else
                        new_state->strafe = -movement_x;

                    float pitch_value;
                    if (gamepad->buttons[_gamepad_binary_button_dpad_up])
                        pitch_value = invert_pitch ? -1.0f : 1.0f;
                    else if (!gamepad->buttons[_gamepad_binary_button_dpad_down])
                        pitch_value = ((invert_pitch ? -1.0f : 1.0f) * movement_y);
                    else
                        pitch_value = invert_pitch ? 1.0f : -1.0f;

                    new_state->pitch = pitch_value;
                    new_state->yaw = -look_x;
                    new_state->forward_movement = look_y;
                }
            }

            input_abstraction_globals.controller_available[controller] = 1;
            if (!input_abstraction_input_state_idle(new_state, &previous_state))
                input_abstraction_globals.all_idle = 0;
        }
        else if (input_abstraction_globals.controller_available[controller])
        {
            /* controller was present last frame but its gamepad is gone: raise a disconnect error */
            __int16 error_local_player = (__int16)controller;
            unsigned __int8 modal = 0;
            __int16 error_code = _error_controller_unplugged;
            unsigned __int8 show_error = 1;

            if (main_menu_is_active())
            {
                int available_count = 0;
                for (int other = 0; other < 4; ++other)
                    if (input_abstraction_globals.controller_available[other])
                        ++available_count;

                unsigned __int8 controls_no_single_player =
                    player_ui_get_single_player_local_player_controller(0) != controller
                    && player_ui_get_single_player_local_player_controller(1) != controller
                    && player_ui_get_single_player_local_player_controller(2) != controller
                    && player_ui_get_single_player_local_player_controller(3) != controller
                    && !player_ui_local_player_wants_to_play_multiplayer((__int16)controller);

                if (available_count < 2)
                {
                    if (controls_no_single_player)
                        error_local_player = -1;
                }
                else if (controls_no_single_player)
                {
                    show_error = 0;
                }
            }
            else
            {
                if (global_network_game_client_get())
                {
                    modal = 0;
                    error_code = _error_controller_unplugged;
                }
                else
                {
                    modal = 1;
                    error_code = _error_controller_unplugged_start_to_continue;
                }
                show_error = local_player_exists(controller);
            }

            if (show_error == 1)
            {
                if (virtual_keyboard_active())
                    virtual_keyboard_close();
                display_error_deferred(error_code, error_local_player, modal, modal);
            }

            input_abstraction_globals.controller_available[controller] = 0;
        }
        else
        {
            input_abstraction_globals.controller_available[controller] = 0;
        }
    }
}
