/* vibrate_update @ 0x837395C0 — accumulate one frame of controller rumble for each of the 2 local
 * controllers and drive the gamepad motors. Each controller has up to 8 concurrent impulse sources,
 * each contributing to a left and right frequency channel: the contribution is the impulse amplitude
 * scaled by a transition function of how far through its duration it is (1 at start, fading to 0). The
 * scripted-vibrate override is added on top, the result is clamped to the 16-bit motor range, the
 * per-impulse elapsed timers are advanced by one tick (1/30s), and — unless rumble is disabled for that
 * player — the motor state is sent to the gamepad.
 *
 * Deviation: the decompiler reads the fade function via HIWORD(); on big-endian PPC that selects the
 * low-addressed int16, i.e. vibrate_frequencies[channel].fade_function. Modeled directly. */

#include <stdint.h>
#include "headers/vibrate_global_data.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/vibrate_frequency.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"

extern float transition_function_evaluate(int16_t function_type, float value);
extern int local_player_get_player_index(int16_t local_player_index);
extern int16_t player_ui_get_single_player_local_player_from_controller(int16_t controller_index);
extern uint8_t player_ui_vibrate_disabled(int16_t local_player_index);
extern void input_set_gamepad_vibrater_state(int controller, uint16_t left, uint16_t right);

void vibrate_update(void)
{
    int16_t controller;

    for ( controller = 0; controller < 2; controller++ )
    {
        player_vibrate_datum *vib = &vibrate_globals->player_vibrate_data[controller];
        float channel[NUMBER_OF_vibrate_FREQUENCIES];
        int impulse_index;
        double left, right;
        double scaled;
        uint16_t left_motor, right_motor;
        int player_index, unit_player_index;

        channel[0] = vib->left_frequency_continuous;
        channel[1] = vib->right_frequency_continuous;

        for ( impulse_index = 0; impulse_index < 8; impulse_index++ )
        {
            float elapsed = vib->time_elapsed[impulse_index];
            int ch;
            for ( ch = 0; ch < NUMBER_OF_vibrate_FREQUENCIES; ch++ )
            {
                vibrate_frequency_definition *freq = &vib->impulse[impulse_index].vibrate_frequencies[ch];
                if ( freq->duration > elapsed )
                {
                    double fraction = (1.0f - (elapsed / freq->duration));
                    if ( fraction >= 0.0 )
                    {
                        if ( fraction > 1.0 )
                            fraction = 1.0;
                    }
                    else
                    {
                        fraction = 0.0;
                    }
                    channel[ch] = (transition_function_evaluate(freq->fade_function, fraction)
                                          * freq->frequency)
                                + channel[ch];
                }
            }
        }

        if ( vibrate_globals->scripted_vibrate.scale == 0.0f )
        {
            left  = channel[0];
            right = channel[1];
        }
        else
        {
            left  = ((vibrate_globals->scripted_vibrate.left_frequency
                                    * vibrate_globals->scripted_vibrate.scale) + channel[0]);
            right = ((vibrate_globals->scripted_vibrate.right_frequency
                                    * vibrate_globals->scripted_vibrate.scale) + channel[1]);
        }

        scaled = ((float)left * 65535.0f);
        if ( scaled >= 0.0 )       scaled = scaled <= 65535.0 ? scaled : 65535.0;
        else                       scaled = 0.0;
        left_motor = (int)scaled;

        scaled = ((float)right * 65535.0f);
        if ( scaled >= 0.0 )       scaled = scaled > 65535.0 ? 65535.0 : scaled;
        else                       scaled = 0.0;
        right_motor = (int)scaled;

        /* advance the 8 per-impulse elapsed timers by one tick */
        {
            int t;
            for ( t = 0; t < 8; t++ )
                vib->time_elapsed[t] = vib->time_elapsed[t] + SECONDS_PER_TICK;
        }

        player_index = local_player_get_player_index(controller);
        if ( player_index == -1 )
        {
            input_set_gamepad_vibrater_state(controller, 0, 0);
            continue;
        }

        unit_player_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index;
        if ( unit_player_index == -1 )
            continue;   /* no controller bound: leave motors untouched */

        if ( player_ui_vibrate_disabled(
                 player_ui_get_single_player_local_player_from_controller(unit_player_index)) )
            input_set_gamepad_vibrater_state(unit_player_index, 0, 0);
        else
            input_set_gamepad_vibrater_state(unit_player_index, left_motor, right_motor);
    }
}
