/* unit_overcharged @0x836CF350 — true when the weapon a unit is currently holding is in an overcharged
 * state. Resolves the held weapon from the unit's current weapon slot and inventory array, returning false
 * when the unit has no current weapon. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern int weapon_overcharged(int weapon_index);

uint8_t unit_overcharged(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int16_t weapon_slot = unit->unit.current_weapon_index;
    if (weapon_slot == -1)
        return 0;

    int weapon_object_index = unit->unit.weapon_object_indices[weapon_slot];
    if (weapon_object_index == -1)
        return 0;
    return weapon_overcharged(weapon_object_index);
}
