/* unit_first_free_weapon_index @0x836CB440 — return the index (0..3) of the unit's first empty weapon
 * inventory slot, or -1 if all four slots are occupied. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

int unit_first_free_weapon_index(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int slot = 0;

    while (unit->unit.weapon_object_indices[slot] != -1)
    {
        slot = (int16_t)(slot + 1);
        if (slot >= 4)
            return -1;
    }

    return slot;
}
