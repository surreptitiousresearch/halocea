/* create_weapon @0x836A9148 — spawn a weapon object from a starting-weapon spec, owned by a unit, and
 * preset its magazine-0 ammo. Returns the new object index, or -1 if the spec has no
 * weapon or object creation fails. */

#include "headers/data_array.h"
#include "headers/object_placement_data.h"
#include "headers/starting_weapon_info.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/blam_data_globals.h"

extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);

int create_weapon(const starting_weapon_info *weapon_info, int unit_index)
{
    int definition_index = weapon_info->weapon.index;
    if (definition_index == -1)
        return -1;

    object_placement_data placement;
    object_placement_data_new(&placement, definition_index, unit_index);
    int weapon_object_index = object_new(&placement);
    if (weapon_object_index != -1)
    {
        weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_object_index)->datum;
        weapon->weapon.magazines[0].rounds_total = weapon_info->rounds_total;
        weapon->weapon.magazines[0].rounds_loaded = weapon_info->rounds_loaded;
    }
    return weapon_object_index;
}
