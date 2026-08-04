/* weapon_magazine_state_interruptable @0x836D9628 — true when a magazine's current reload state can be
 * interrupted by a new state change: only the idle-class states (idle / reloaded) are interruptable; the
 * committed states (reloading / chambering) are not. The proposed new_state is unused. */

#include <stdint.h>
#include "headers/weapon_magazine_state.h"

int weapon_magazine_state_interruptable(int16_t old_state, int16_t new_state)
{
    return old_state == _weapon_magazine_idle || old_state == _weapon_magazine_reloaded;
}
