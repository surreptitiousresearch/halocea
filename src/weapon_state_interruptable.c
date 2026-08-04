/* weapon_state_interruptable @0x836DA140 — true when a weapon animation state can be interrupted by a
 * proposed new state. The idle state (0) is always interruptable; the two recoil states (1,2) are
 * interruptable only by a state of equal-or-higher priority; all higher states are non-interruptable. */

#include <stdint.h>
#include "headers/weapon_datum_state.h"

int weapon_state_interruptable(int16_t old_state, int16_t new_state)
{
    if ( old_state == weapon_state_idle )
        return 1;
    if ( (unsigned int)old_state < 3 )
        return new_state >= (int)old_state;
    return 0;
}
