#include <stdint.h>
#include "headers/actor_default_state.h"
/* actor_action_get_default_state @0x837F3F58 — map an actor initial-state enum to its default action state via
 * the global_default_states table; out-of-range (<0 or >=12) maps to 0. */

extern const uint16_t global_default_states[12];

uint16_t actor_action_get_default_state(int16_t initial_state)
{
    if ( initial_state >= actor_default_state_none && initial_state < number_of_actor_default_states )
        return global_default_states[initial_state];
    return 0;
}
