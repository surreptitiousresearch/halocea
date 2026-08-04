/* player_control_get_desired_weapon @0x836DE970 — resolve the weapon object a local player wants
 * to be holding. If the player is not currently controlling this unit, fall back to the unit's
 * current weapon. Otherwise honour the player's desired_weapon_index; if that yields no weapon
 * (-1), fall back to the unit's current weapon. Returns the weapon object index (or -1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_control_globals.h"
#include "headers/blam_data_globals.h"

extern int unit_inventory_get_weapon(int unit_index, int16_t index);

int player_control_get_desired_weapon(__int16 local_player_index, int unit_index)
{
    player_control *control = &player_control_globals->players[local_player_index];
    unit_datum *unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if (control->unit_index != unit_index)
        return unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index);

    int result = unit_inventory_get_weapon(unit_index, control->desired_weapon_index);
    if (result == -1)
        return unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index);
    return result;
}
