/* ai_get_responsible_unit @0x836E8904 — map an object to the unit accountable for it. When
 * responsible_for_weapon_fire is set and the object records a weapon-fire controller, that controller is
 * returned; otherwise the object's general responsible unit is used. If neither is recorded the object is
 * its own responsible unit. Returns -1 for a missing object or a non-unit. */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

int ai_get_responsible_unit(int object_index, uint8_t responsible_for_weapon_fire)
{
    if (object_index == -1)
        return -1;

    unit_datum *unit = object_try_and_get_and_verify_type(object_index, object_mask_unit);
    if (!unit)
        return -1;

    if (responsible_for_weapon_fire)
    {
        int weapon_fire_responsible_unit = unit->unit.gunner_object_index; /* +808 */
        if (weapon_fire_responsible_unit != -1)
            return weapon_fire_responsible_unit;
    }

    int responsible_unit = unit->unit.driver_object_index; /* +804 */
    if (responsible_unit == -1)
        return object_index;
    return responsible_unit;
}
