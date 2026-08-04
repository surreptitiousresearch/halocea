/* gamepad_button_is_down @0x83688A80 — true if the given button is currently held on the first
 * connected gamepad (scans machine indices 0..3 for the first present gamepad). */

#include <stdint.h>
#include "headers/gamepad_state.h"

extern uint8_t input_has_gamepad(int16_t gamepad_index);
extern gamepad_state *input_get_gamepad_state(int16_t gamepad_index);

int gamepad_button_is_down(int16_t button_index)
{
    __int16 machine_index = 0;
    do
    {
        if ( input_has_gamepad(machine_index) )
            break;
        machine_index = (__int16)(machine_index + 1);
    }
    while ( machine_index < 4 );

    return machine_index < 4 && input_get_gamepad_state(machine_index)->buttons[button_index] != 0;
}
