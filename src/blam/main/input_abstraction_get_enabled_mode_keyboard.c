/* input_abstraction_get_enabled_mode_keyboard @0x83724C68 — tests bit 0x4 of
 * input_abstraction_globals.active_modes. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/input_abstraction_mode_flags.h"

uint8_t input_abstraction_get_enabled_mode_keyboard(void)
{
    return (input_abstraction_globals.active_modes >> _input_abstraction_mode_keyboard_bit) & 1;
}
