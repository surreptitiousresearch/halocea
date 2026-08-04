/* input_abstraction_get_enabled_mode_events @0x83724C50 — tests bit 0x2 of
 * input_abstraction_globals.active_modes. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/input_abstraction_mode_flags.h"

uint8_t input_abstraction_get_enabled_mode_events(void)
{
    return (input_abstraction_globals.active_modes >> _input_abstraction_mode_events_bit) & 1;
}
