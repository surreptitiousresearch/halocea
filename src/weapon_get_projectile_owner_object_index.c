/* weapon_get_projectile_owner_object_index @0x836D97A0 — the object to credit for the weapon's projectiles: the
 * holding unit's gunner object (the unit operating the weapon) when set, otherwise the holding unit itself.
 * The weapon's parent must be a valid unit (type mask 0x3). Returns -1 if there is no unit owner. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

int weapon_get_projectile_owner_object_index(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    int parent = weapon->object.parent_object_index;
    if ( parent == -1 )
        return -1;
    unit_datum *unit = object_try_and_get_and_verify_type(parent, object_mask_unit);
    if ( !unit )
        return -1;
    int gunner_object_index = unit->unit.gunner_object_index;
    if ( gunner_object_index != -1 )
        return gunner_object_index;
    return weapon->object.parent_object_index;
}
