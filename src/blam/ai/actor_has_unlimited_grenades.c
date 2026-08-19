/* actor_has_unlimited_grenades @0x8371E370 — whether the given actor's grenade supply is unlimited.
 * In this build the check is stubbed to always grant unlimited grenades to AI actors. */

#include <stdint.h>

/* actor_index unread by the stub; caller (unit_throw_grenade_move_to_hand @0x836CB6F0) loads a
 * full-word datum handle into r3 before the call, so the interface keeps `int actor_index`. */
uint8_t actor_has_unlimited_grenades(int actor_index)
{
    return 1;
}
