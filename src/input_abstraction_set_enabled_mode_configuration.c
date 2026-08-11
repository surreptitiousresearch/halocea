/* input_abstraction_set_enabled_mode_configuration @0x83724D20 — toggle the "controller configuration" input
 * mode. Enabling it sets the mode-active bit and snapshots all 4 gamepads' current state (zeroing the
 * snapshot slot for any disconnected controller); disabling it clears the bit and zeroes the whole snapshot
 * array. */

#include <stdint.h>
#include <string.h>
#include "headers/input_abstraction_globals.h"
#include "headers/input_abstraction_mode_flags.h"
#include "headers/gamepad_state.h"
#include "headers/blam_data_globals.h"


extern gamepad_state *input_get_gamepad_state(int16_t gamepad_index);
/* memset declared by <string.h> */

void input_abstraction_set_enabled_mode_configuration(uint8_t enabled)
{
    if ( enabled )
    {
        input_abstraction_globals.active_modes |= (1u << _input_abstraction_mode_configuration_bit);

        gamepad_state *snapshot = input_abstraction_globals.configuration_gamepad_snapshot;
        for ( int16_t i = 0; i < 4; i++ )
        {
            const gamepad_state *state = input_get_gamepad_state(i);
            if ( state )
                memcpy(&snapshot[i], state, sizeof(gamepad_state));
            else
                memset(&snapshot[i], 0, sizeof(gamepad_state));
        }
    }
    else
    {
        input_abstraction_globals.active_modes &= ~(1u << _input_abstraction_mode_configuration_bit);
        memset(input_abstraction_globals.configuration_gamepad_snapshot, 0,
            sizeof(input_abstraction_globals.configuration_gamepad_snapshot));
    }
}
