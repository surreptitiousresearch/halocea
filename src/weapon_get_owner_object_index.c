/* weapon_get_owner_object_index @0x836D9720 — the unit holding this weapon: the weapon's parent object
 * if it is a valid unit (type mask 0x3), else -1. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

int weapon_get_owner_object_index(int weapon_index)
{
    object_datum *weapon = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    int parent = weapon->object.parent_object_index;
    if ( parent == -1 || !object_try_and_get_and_verify_type(parent, object_mask_unit) )
        return -1;
    return weapon->object.parent_object_index;
}
