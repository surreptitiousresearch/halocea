#include <stdint.h>
#include "headers/unit_animation_state.h"
/* unit_close @0x836CF318 — set a unit's animation state to "close" (state 38). */

extern uint8_t unit_animation_set_state(int unit_index, int16_t new_state);

void unit_close(int unit_index)
{
    if ( unit_index != -1 )
        unit_animation_set_state(unit_index, _unit_state_closing);
}
