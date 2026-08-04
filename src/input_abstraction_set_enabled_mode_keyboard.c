/* input_abstraction_set_enabled_mode_keyboard @0x83724CE0 — enable/disable the "keyboard" input mode
 * (bit 0x4 of the active-modes mask). */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/input_abstraction_mode_flags.h"

void input_abstraction_set_enabled_mode_keyboard(uint8_t enabled)
{
    if ( enabled )
        input_abstraction_globals.active_modes |= (1u << _input_abstraction_mode_keyboard_bit);
    else
        input_abstraction_globals.active_modes &= ~(1u << _input_abstraction_mode_keyboard_bit);
}
