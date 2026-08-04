/* unit_scripting_has_weapon_readied @0x83800BF0 — return whether the unit's currently-readied weapon (the
 * weapon object indexed by the unit's current-weapon slot at +0x2F2) is an instance of the given weapon
 * definition. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern int unit_inventory_get_weapon(int unit_index, int16_t index);

uint8_t unit_scripting_has_weapon_readied(int unit_index, int weapon_definition_index)
{
    if ( unit_index == -1 || weapon_definition_index == -1 )
        return 0;

    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int weapon = unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index);
    if ( weapon == -1 )
        return 0;
    object_datum *weapon_data =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon)->datum;
    return weapon_definition_index == weapon_data->definition_index;
}
