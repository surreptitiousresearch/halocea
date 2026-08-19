/* weapon_magazine_state_change_ok @0x836D9680 — true if a weapon's magazine is allowed to change state
 * right now: neither trigger is firing (weapon.triggers[0].state / triggers[1].state) and the weapon is
 * not in a busy animation state (weapon.state). This is the same guard triple that
 * weapon_magazine_start_reload / weapon_magazine_start_chamber test before proceeding.
 *
 * DEVIATION: the decompiler emits raw datum-block byte reads at +609/+649/+568; DB-verified those are
 * weapon.triggers[0].state (0x22C+0x35=609), weapon.triggers[1].state (0x22C+0x5D=649) and
 * weapon.state (0x22C+0x0C=568), so they are recovered as typed member accesses. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_datum_state.h"
#include "headers/blam_data_globals.h"


/* Attested uint8_t return: callee clrlwi r3,24 @0x836D96D0; caller clrlwi r3,24 @0x836DBE24. */
uint8_t weapon_magazine_state_change_ok(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;

    if ( weapon->weapon.triggers[0].state )
        return 0;
    if ( weapon->weapon.triggers[1].state )
        return 0;
    if ( weapon->weapon.state != weapon_state_idle )
        return 0;
    return 1;
}
