/* weapon_detonate @0x836D9E50 — spawn the weapon's detonation effect at the weapon and then delete the
 * weapon object (e.g. an overheated/discarded weapon cooking off). */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/blam_data_globals.h"

extern int weapon_effect_new(int weapon_index, int effect_index, float effect_scale, float effect_error);
extern void object_delete(int object_index);

void weapon_detonate(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);

    weapon_effect_new(weapon_index, definition->weapon.detonation_effect.index, 0.0f, 0.0f);
    object_delete(weapon_index);
}
