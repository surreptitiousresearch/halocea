/* unit_approve_weapon_pickup @0x836CB550 — approve picking up a weapon only if the unit isn't already carrying
 * one of the same type. Each of the unit's four inventory slots (unit+760) is checked; if any holds a weapon
 * whose definition tag matches the offered weapon's, the pickup is disapproved (returns 0). Otherwise it is
 * approved (returns 1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/maximum_weapons_per_unit.h"
#include "headers/blam_data_globals.h"


uint8_t unit_approve_weapon_pickup(int unit_index, int new_weapon_index)
{
    unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    object_datum *new_weapon_object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, new_weapon_index)->datum);

    uint8_t result = 1;
    for (int slot = 0; slot < MAXIMUM_WEAPONS_PER_UNIT; slot = (int16_t)(slot + 1))
    {
        int weapon = unit_object->unit.weapon_object_indices[slot];
        if (weapon != -1)
        {
            object_datum *weapon_object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon)->datum);
            if (weapon_object->definition_index == new_weapon_object->definition_index) /* same weapon definition tag */
                result = 0;
        }
    }
    return result;
}
