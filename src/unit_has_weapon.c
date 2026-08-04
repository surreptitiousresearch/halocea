/* unit_has_weapon @0x836CD2E8 — true if any of a unit's 4 weapon inventory slots holds the object
 * whose object index equals weapon_index. Scans unit.weapon_object_indices[0..3] for an exact match. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/maximum_weapons_per_unit.h"
#include "headers/blam_data_globals.h"


uint8_t unit_has_weapon(int unit_index, int weapon_index)
{
    unit_datum *unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int *weapon_slot = &unit->unit.weapon_object_indices[0];
    for ( int slot_count = 0; *weapon_slot != weapon_index; ++weapon_slot )
    {
        if ( ++slot_count >= MAXIMUM_WEAPONS_PER_UNIT )
            return 0;
    }
    return 1;
}
