/* weapon_trigger_overload @0x836DD0D0 — put a weapon trigger into the overloaded state: if the weapon
 * definition has a next trigger (definition triggers count dword +1276, records at dword +1280, 276-byte
 * stride), fire it (chain-fire on overload, e.g. plasma weapons venting); then set this trigger's state
 * byte (weapon+40*trigger+609) to 1 (overloaded) and its tick timer word (+610) to the definition's
 * overload time (trigger definition float +196, seconds → 30Hz ticks). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/weapon_trigger_state.h"
#include "headers/blam_data_globals.h"


extern void weapon_trigger_fire(int weapon_index, int16_t trigger_index);

void weapon_trigger_overload(int weapon_index, int trigger_index)
{
    weapon_datum *weapon =
        (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    weapon_trigger_definition *trigger_definition =
        &((weapon_trigger_definition *)definition->weapon.triggers.address)[trigger_index];

    if ( trigger_index + 1 < definition->weapon.triggers.count )
        weapon_trigger_fire(weapon_index, trigger_index + 1);

    /* re-fetch: weapon_trigger_fire can move object data (matches compiled reload of the header slot) */
    weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_trigger *trigger = &weapon->weapon.triggers[(int16_t)trigger_index];
    trigger->state = _weapon_trigger_firing;
    trigger->state_timer = (int16_t)(int)(trigger_definition->overloading_time * 30.0f);
}
