/* weapon_build_weapon_interface_state @0x836D8C90 — snapshot a weapon object's live HUD readout into a
 * weapon_interface_state: overall heat/age/overheat plus, for each of the weapon definition's magazines,
 * the reload/fire state and the loaded/remaining round counts (current and maximum).
 *
 * The magazine state is 0 when ready to fire and 1 or 3 while reloading. */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/weapon_interface_state.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/weapon_magazine_state.h"
#include "headers/weapon_datum_flags.h"
#include "headers/blam_data_globals.h"


void weapon_build_weapon_interface_state(int weapon_index, weapon_interface_state *state)
{
    weapon_datum *weapon_object = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon_object->definition_index);

    state->heat = weapon_object->weapon.heat;
    state->age = weapon_object->weapon.age;
    state->overheated = weapon_object->weapon.flags & (1u << _weapon_overheated_bit);
    state->magazine_count = definition->weapon.magazines.count;

    weapon_magazine_definition *magazine_definitions = (weapon_magazine_definition *)definition->weapon.magazines.address;
    for (int i = 0; i < definition->weapon.magazines.count; ++i)
    {
        weapon_magazine_definition *magazine_definition = &magazine_definitions[i];
        weapon_magazine *object_magazine = &weapon_object->weapon.magazines[i];
        weapon_interface_magazine_state *magazine = &state->magazines[i];

        __int16 magazine_state = object_magazine->state; /* idle=ready; reloading/chambering = busy */
        magazine->reloading = (magazine_state == _weapon_magazine_reloading || magazine_state == _weapon_magazine_chambering);
        magazine->can_fire = (magazine_state == _weapon_magazine_idle);
        magazine->rounds_loaded = object_magazine->rounds_loaded;
        magazine->rounds_loaded_maximum = magazine_definition->rounds_loaded_maximum;
        magazine->rounds_remaining = object_magazine->rounds_total;
        magazine->rounds_remaining_maximum = magazine_definition->rounds_total_maximum;
    }
}
