/* unit_inventory_get_must_be_readied_weapon @0x836CD1E0 — return the slot (0-3) of the first weapon in
 * a unit's inventory that must be readied (e.g. picked up but not yet drawn), or -1 if none. Weapon
 * slots are object dwords +760 + 4*slot (dword index 190+slot). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/maximum_weapons_per_unit.h"
#include "headers/blam_data_globals.h"

extern uint8_t weapon_must_be_readied(int weapon_index);

int16_t unit_inventory_get_must_be_readied_weapon(int unit_index)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    for ( int weapon_slot = 0; weapon_slot < MAXIMUM_WEAPONS_PER_UNIT; weapon_slot = (short)(weapon_slot + 1) )
    {
        /* DEVIATION: raw dword 190+slot is unit.weapon_object_indices[slot] (unit@500 + 260 + 4*slot) */
        int weapon_index = unit->unit.weapon_object_indices[weapon_slot];
        if ( weapon_index != -1 && weapon_must_be_readied(weapon_index) )
            return weapon_slot;
    }
    return -1;
}
