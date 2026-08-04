/* weapon_compute_movement_penalty @0x836D90B8 — return the movement-speed penalty a held weapon imposes,
 * selecting the forward or sideways penalty from the weapon definition. Per the definition's
 * movement_penalty_mode (weapon_movement_penalty_mode enum), the penalty is suppressed (0) while not
 * zoomed: when_zoomed always suppresses when un-zoomed, when_zoomed_or_reloading only while a
 * magazine is reloading. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_state.h"
#include "headers/weapon_movement_penalty_mode.h"
#include "headers/blam_data_globals.h"

float weapon_compute_movement_penalty(int weapon_index, uint8_t forward, uint8_t zoomed)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);

    float penalty = forward ? definition->weapon.forward_movement_penalty
                            : definition->weapon.sideways_movement_penalty;

    __int16 mode = definition->weapon.movement_penalty_mode;
    if ( (mode == _weapon_movement_penalty_when_zoomed
          || (mode == _weapon_movement_penalty_when_zoomed_or_reloading
              && (weapon->weapon.magazines[0].state == _magazine_reloading
                  || weapon->weapon.magazines[1].state == _magazine_reloading)))
         && !zoomed )
    {
        penalty = 0.0f;
    }
    return penalty;
}
