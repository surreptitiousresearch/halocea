/* weapon_state_is_depleted @0x83707D08 */
#include "headers/weapon_interface_state.h"

int weapon_state_is_depleted(const weapon_interface_state *state)
{
    if (state->magazines[0].rounds_loaded_maximum
        && !state->magazines[0].rounds_loaded
        && !state->magazines[0].rounds_remaining)
    {
        return 1;
    }

    if (state->age == 1.0f)
        return 1;

    return 0;
}
