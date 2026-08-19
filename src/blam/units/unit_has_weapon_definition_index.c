/* unit_has_weapon_definition_index @0x836CB3B8 — true if any of a unit's 4 weapon slots (object dwords
 * +760 + 4*slot, i.e. dword index 190+slot) holds a weapon object whose definition tag index equals
 * weapon_definition_index. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/maximum_weapons_per_unit.h"
#include "headers/blam_data_globals.h"


uint8_t unit_has_weapon_definition_index(int unit_index, int weapon_definition_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    for ( int weapon_slot = 0; weapon_slot < MAXIMUM_WEAPONS_PER_UNIT; weapon_slot = (short)(weapon_slot + 1) )
    {
        /* DEVIATION: raw dword 190+slot is unit.weapon_object_indices[slot] (unit@500 + 260 + 4*slot) */
        int weapon_index = unit->unit.weapon_object_indices[weapon_slot];
        if ( weapon_index != -1
          && DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum->definition_index == weapon_definition_index )
            return 1;
    }
    return 0;
}
