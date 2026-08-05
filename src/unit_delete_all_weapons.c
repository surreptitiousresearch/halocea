/* unit_delete_all_weapons @0x836CB328 — delete every weapon a unit is carrying except the one currently
 * held. Each non-held, valid inventory entry is deleted and cleared, and the current/desired weapon slot
 * fields are reset when they referenced a deleted slot. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/maximum_weapons_per_unit.h"
#include "headers/blam_data_globals.h"

extern void object_delete(int object_index);

void unit_delete_all_weapons(int unit_index)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    for (int slot = 0; slot < MAXIMUM_WEAPONS_PER_UNIT; slot = (int16_t)(slot + 1))
    {
        int weapon_object_index = unit->unit.weapon_object_indices[slot];
        if (weapon_object_index != -1 && slot != unit->unit.current_weapon_index)
        {
            object_delete(weapon_object_index);
            unit->unit.weapon_object_indices[slot] = -1;
            if (slot == unit->unit.desired_weapon_index)
                unit->unit.desired_weapon_index = -1;
            if (slot == unit->unit.current_weapon_index)
                unit->unit.current_weapon_index = -1;
        }
    }
}
