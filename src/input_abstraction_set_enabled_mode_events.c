/* input_abstraction_set_enabled_mode_events @0x83724CA8 — enable/disable the "events" input mode
 * (bit 0x2 of the active-modes mask). */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/input_abstraction_mode_flags.h"

void input_abstraction_set_enabled_mode_events(uint8_t enabled)
{
    if ( enabled )
        input_abstraction_globals.active_modes |= (1u << _input_abstraction_mode_events_bit);
    else
        input_abstraction_globals.active_modes &= ~(1u << _input_abstraction_mode_events_bit);
}
