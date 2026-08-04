/* input_mode_handle_events @0x83725008 — translate raw controller input into UI navigation events for each
 * of the four controllers. The d-pad and the left thumbstick both drive directional (up/down/left/right)
 * events; each direction repeats no faster than every 0x15E ms (350 ms). A thumbstick direction only fires on
 * a fresh deflection (the stick was centred last frame) and, when it fires, the stick axis is snapped to its
 * extreme and the prior magnitude latched so the repeat-throttle and edge-detect work. Face/menu buttons map
 * to accept/back/create/delete on a fresh press (duration == 1).
 *
 * abs16(x) is the inlined signed-abs idiom (srawi/xor/subf), restored as a branchful absolute value. The
 * function writes snapped values back through the returned gamepad_state, so the const is cast away (faithful
 * to the original). The `(|prev| >= 0) + (|prev| >= 0x7332)` & 1 idiom reduces to `|prev| < ANALOG_STICK_RECOGNITION_THRESHOLD`
 * (stick centred last frame), since the abs is always non-negative. */

#include <stdint.h>
#include "headers/gamepad_state.h"
#include "headers/gamepad_button.h"
#include "headers/analog_stick_event_constants.h"
#include "headers/blam_data_globals.h"


extern uint8_t input_has_gamepad(int16_t gamepad_index);
extern gamepad_state *input_get_gamepad_state(int16_t gamepad_index);
extern uint32_t main_get_mseconds(void);
extern void event_manager_queue_left_event(uint8_t ticks, int16_t player_number);
extern void event_manager_queue_right_event(uint8_t ticks, int16_t player_number);
extern void event_manager_queue_up_event(uint8_t ticks, int16_t player_number);
extern void event_manager_queue_down_event(uint8_t ticks, int16_t player_number);
extern void event_manager_queue_accept_event(uint8_t ticks, int16_t player_number);
extern void event_manager_queue_back_event(uint8_t ticks, int16_t player_number);
extern void event_manager_queue_create_event(uint8_t ticks, int16_t player_number);
extern void event_manager_queue_delete_event(uint8_t ticks, int16_t player_number);

void input_mode_handle_events(void)
{
    for (int controller = 0; controller < 4; controller = (__int16)(controller + 1))
    {
        char up_event = 0;
        char down_event = 0;
        char left_event = 0;
        char right_event = 0;

        if (!input_has_gamepad(controller))
            continue;

        gamepad_state *state = input_get_gamepad_state(controller);
        unsigned int mseconds = main_get_mseconds();
        if (!state)
            continue;

        /* d-pad: each held direction repeats no faster than every 350 ms; releasing clears the throttle */
        if (state->buttons[_gamepad_binary_button_dpad_up])
        {
            if (mseconds - last_dpad_up[controller] >= MILLISECOND_DELAY_BETWEEN_SUBSEQUENT_ANALOG_STICK_EVENTS)
            {
                up_event = 1;
                last_dpad_up[controller] = mseconds;
            }
        }
        else
            last_dpad_up[controller] = 0;

        if (state->buttons[_gamepad_binary_button_dpad_down])
        {
            if (mseconds - last_dpad_down[controller] >= MILLISECOND_DELAY_BETWEEN_SUBSEQUENT_ANALOG_STICK_EVENTS)
            {
                down_event = 1;
                last_dpad_down[controller] = mseconds;
            }
        }
        else
            last_dpad_down[controller] = 0;

        if (state->buttons[_gamepad_binary_button_dpad_left])
        {
            if (mseconds - last_dpad_left[controller] >= MILLISECOND_DELAY_BETWEEN_SUBSEQUENT_ANALOG_STICK_EVENTS)
            {
                left_event = 1;
                last_dpad_left[controller] = mseconds;
            }
        }
        else
            last_dpad_left[controller] = 0;

        if (state->buttons[_gamepad_binary_button_dpad_right])
        {
            if (mseconds - last_dpad_right[controller] >= MILLISECOND_DELAY_BETWEEN_SUBSEQUENT_ANALOG_STICK_EVENTS)
            {
                right_event = 1;
                last_dpad_right[controller] = mseconds;
            }
        }
        else
            last_dpad_right[controller] = 0;

        /* left thumbstick: fire a direction on a fresh deflection past the threshold (0x7332) */
        int stick_x = state->sticks[0].n[0];
        int stick_y = state->sticks[0].n[1];
        int abs_x = stick_x < 0 ? -stick_x : stick_x;
        int abs_y = stick_y < 0 ? -stick_y : stick_y;
        int new_x = stick_x;
        int new_y = stick_y;

        if (abs_x >= ANALOG_STICK_RECOGNITION_THRESHOLD || abs_y >= ANALOG_STICK_RECOGNITION_THRESHOLD)
        {
            int prev_abs_x = last_left_analog_stick_x[controller];
            if (prev_abs_x < 0)
                prev_abs_x = -prev_abs_x;
            int prev_abs_y = last_left_analog_stick_y[controller];
            if (prev_abs_y < 0)
                prev_abs_y = -prev_abs_y;

            int x_was_centered = prev_abs_x < ANALOG_STICK_RECOGNITION_THRESHOLD;
            int y_was_centered = prev_abs_y < ANALOG_STICK_RECOGNITION_THRESHOLD;

            if (mseconds - left_analog_stick_timer[controller] >= MILLISECOND_DELAY_BETWEEN_SUBSEQUENT_ANALOG_STICK_EVENTS
                && ((abs_x >= ANALOG_STICK_RECOGNITION_THRESHOLD && x_was_centered) || (abs_y >= ANALOG_STICK_RECOGNITION_THRESHOLD && y_was_centered)))
            {
                left_analog_stick_timer[controller] = mseconds;
                if (abs_x >= ANALOG_STICK_RECOGNITION_THRESHOLD)
                {
                    if (stick_x < 0)
                    {
                        new_x = -32768;
                        state->sticks[0].n[0] = (short)0x8000;
                        left_event = 1;
                    }
                    else
                    {
                        new_x = 0x7FFF;
                        state->sticks[0].n[0] = 0x7FFF;
                        right_event = 1;
                    }
                }
                if (abs_y >= ANALOG_STICK_RECOGNITION_THRESHOLD)
                {
                    if (stick_y < 0)
                    {
                        new_y = -32768;
                        state->sticks[0].n[1] = (short)0x8000;
                        down_event = 1;
                    }
                    else
                    {
                        new_y = 0x7FFF;
                        state->sticks[0].n[1] = 0x7FFF;
                        up_event = 1;
                    }
                }
            }
            else
            {
                new_x = 0;
                new_y = 0;
            }
        }
        else
        {
            new_x = 0;
            new_y = 0;
        }

        last_left_analog_stick_x[controller] = new_x;
        last_left_analog_stick_y[controller] = new_y;

        if (left_event)
            event_manager_queue_left_event(1, controller);
        if (right_event)
            event_manager_queue_right_event(1, controller);
        if (up_event)
            event_manager_queue_up_event(1, controller);
        if (down_event)
            event_manager_queue_down_event(1, controller);
        if (state->buttons[_gamepad_analog_button_a] == 1 || state->buttons[_gamepad_binary_button_start] == 1)
            event_manager_queue_accept_event(1, controller);
        if (state->buttons[_gamepad_analog_button_b] == 1 || state->buttons[_gamepad_binary_button_back] == 1)
            event_manager_queue_back_event(1, controller);
        if (state->buttons[_gamepad_analog_button_y] == 1)
            event_manager_queue_create_event(1, controller);
        if (state->buttons[_gamepad_analog_button_x] == 1)
            event_manager_queue_delete_event(1, controller);
    }
}
