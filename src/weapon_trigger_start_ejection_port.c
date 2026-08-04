/* weapon_trigger_start_ejection_port @0x836DA018 — kick off a trigger's ejection-port animation. Only
 * acts if the trigger definition has an ejection-port recovery time (> 0) and the trigger's
 * "ejection port during chamber animation" flag (bit 0x7) agrees with the caller's `chamber` phase:
 * flag set requires chamber != 0, flag clear requires chamber == 0. When it fires, the runtime trigger's
 * ejection_port_position is set to 1.0 (fully open). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/weapon_trigger_definition_flags.h"
#include "headers/blam_data_globals.h"


void weapon_trigger_start_ejection_port(int weapon_index, int16_t trigger_index, uint8_t chamber)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_trigger *trigger = &weapon->weapon.triggers[trigger_index];

    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    weapon_trigger_definition *trigger_definition =
        &((weapon_trigger_definition *)definition->weapon.triggers.address)[trigger_index];

    if ( trigger_definition->ejection_port_recovery_time > 0.0 )
    {
        if ( (trigger_definition->flags & (1u << _weapon_trigger_ejection_port_during_chamber_animation_bit)) != 0 )
        {
            if ( !chamber )
                return;
        }
        else if ( chamber )
        {
            return;
        }
        trigger->ejection_port_position = 1.0f;
    }
}
