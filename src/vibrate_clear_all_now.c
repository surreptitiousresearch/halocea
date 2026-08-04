/* vibrate_clear_all_now @0x83739320 — clear all controller vibration immediately: zero the vibrate
 * globals and force every connected gamepad's rumble motors off. */

#include <stdint.h>
#include "headers/vibrate_global_data.h"
#include <string.h>

extern uint8_t input_has_gamepad(int16_t gamepad_index);
extern void input_set_gamepad_vibrater_state(int gamepad_index, int low_frequency, int high_frequency);

void vibrate_clear_all_now(void)
{
    memset(vibrate_globals, 0, sizeof(vibrate_global_data));
    for ( int gamepad_index = 0; gamepad_index < 2; ++gamepad_index )
    {
        if ( input_has_gamepad(gamepad_index) )
            input_set_gamepad_vibrater_state(gamepad_index, 0, 0);
    }
}
