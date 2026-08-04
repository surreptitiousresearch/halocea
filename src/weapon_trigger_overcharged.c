/* weapon_trigger_overcharged @0x836DD2B8 — resolve a fully-charged trigger's overcharge behaviour.
 * Reads the trigger definition's overcharged_action (weapon_trigger_definition +0x50): action 1 detonates
 * the weapon (e.g. overcharged plasma pistol blowing up in hand), action 2 releases the held charge. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/weapon_overcharged_action.h"
#include "headers/blam_data_globals.h"


extern void weapon_detonate(int weapon_index);
extern void weapon_trigger_release_charge(int weapon_index, int16_t trigger_index);

void weapon_trigger_overcharged(int weapon_index, int16_t trigger_index)
{
    weapon_datum *weapon =
        (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    weapon_trigger_definition *trigger_definition =
        &((weapon_trigger_definition *)definition->weapon.triggers.address)[trigger_index];

    switch ( (unsigned __int16)trigger_definition->overcharged_action )
    {
        case _trigger_overcharged_explodes:
            weapon_detonate(weapon_index);
            break;
        case _trigger_overcharged_fire:
            weapon_trigger_release_charge(weapon_index, trigger_index);
            break;
    }
}
