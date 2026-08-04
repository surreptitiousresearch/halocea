/* input_abstraction_get_enabled_mode_game @0x83724C38 — tests bit 0x1 of input_abstraction_globals.active_modes. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/input_abstraction_mode_flags.h"

uint8_t input_abstraction_get_enabled_mode_game(void)
{
    return (input_abstraction_globals.active_modes >> _input_abstraction_mode_game_bit) & 1;
}
