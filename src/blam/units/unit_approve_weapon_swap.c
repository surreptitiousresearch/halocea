/* unit_approve_weapon_swap @0x836CF668 — decide whether a unit should be allowed to swap to (pick up) a
 * weapon it is offered. The unit holds up to four inventory weapons, with the currently equipped slot index in
 * current_weapon_index. The swap is rejected outright if the unit isn't actually holding a weapon. Otherwise
 * every occupied inventory slot is scanned for one whose weapon definition matches the offered weapon: if such
 * a same-type weapon is found in a slot that isn't the held slot, or whose remaining charge/age has run out,
 * or whose charge is no greater than the offered weapon's, the swap is disapproved. In effect the swap is only
 * approved when it would replace the held weapon with a fresher same-type one. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_datum.h"
#include "headers/maximum_weapons_per_unit.h"
#include "headers/blam_data_globals.h"


uint8_t unit_approve_weapon_swap(int unit_index, int new_weapon_index)
{
    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    weapon_datum *new_weapon_object = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, new_weapon_index)->datum;

    int equipped_slot = unit_object->unit.current_weapon_index;
    int equipped_weapon = -1;
    if (equipped_slot != -1)
        equipped_weapon = unit_object->unit.weapon_object_indices[equipped_slot];

    if (equipped_weapon == -1)
        return 0;

    uint8_t result = 1;
    for (int slot = 0; slot < MAXIMUM_WEAPONS_PER_UNIT; slot = (int16_t)(slot + 1))
    {
        int inventory_weapon = unit_object->unit.weapon_object_indices[slot];
        if (inventory_weapon == -1)
            continue;

        weapon_datum *inventory_weapon_object =
            (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, inventory_weapon)->datum;
        if (new_weapon_object->definition_index == inventory_weapon_object->definition_index)
        {
            float held_charge = inventory_weapon_object->weapon.age;
            if (slot != unit_object->unit.current_weapon_index
                || held_charge <= 0.0f
                || new_weapon_object->weapon.age >= held_charge)
            {
                result = 0;
            }
        }
    }
    return result;
}
