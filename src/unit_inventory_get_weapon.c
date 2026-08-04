/* unit_inventory_get_weapon @0x836CECF8 — the object index of the weapon in a unit's inventory slot.
 * Returns -1 for an invalid slot. Reads unit.weapon_object_indices[index] (word index 190 from the
 * datum base). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


int unit_inventory_get_weapon(int unit_index, int16_t index)
{
    if ( index == -1 )
        return -1;

    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    return unit->unit.weapon_object_indices[index];
}
