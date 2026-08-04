#include <stdint.h>
#include "headers/unit_animation_state.h"
/* unit_abort_animation @0x836CE780 — cancel a unit's active animation by forcing its animation state to 0.
 * A one-line tail-call wrapper over unit_animation_set_state. */

extern uint8_t unit_animation_set_state(int unit_index, int16_t new_state);

void unit_abort_animation(int unit_index)
{
    unit_animation_set_state(unit_index, _unit_state_idle);
}
