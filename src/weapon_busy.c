/* weapon_busy @0x836D9610 — true when a weapon object is mid-action and must not be interrupted:
 * either trigger is in a non-idle state, either magazine is mid-reload, or the weapon's own animation
 * state machine is non-idle. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/blam_data_globals.h"

int weapon_busy(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;

    if ( weapon->weapon.triggers[0].state )
        return 1;
    if ( weapon->weapon.triggers[1].state )
        return 1;
    if ( weapon->weapon.magazines[0].state )
        return 1;
    if ( weapon->weapon.magazines[1].state )
        return 1;
    if ( weapon->weapon.state )
        return 1;
    return 0;
}
