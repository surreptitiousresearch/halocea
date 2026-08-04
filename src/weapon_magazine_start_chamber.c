/* weapon_magazine_start_chamber @0x836DB1E8 — start chambering a round into a weapon's magazine: only
 * only from an idle-class magazine state (_weapon_magazine_idle or _weapon_magazine_reloaded), and only
 * while neither trigger is mid-fire nor the weapon state byte is set. Sets the state to chambering,
 * plays the magazine definition's chambering_effect, and sets the chamber timer from chamber_time
 * (seconds -> 30Hz ticks). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/weapon_magazine_state.h"
#include "headers/weapon_datum_state.h"
#include "headers/blam_data_globals.h"


extern uint8_t weapon_set_state(int weapon_index, int16_t new_state, uint8_t immediate);
extern int weapon_effect_new(int weapon_index, int effect_index, float effect_scale, float effect_error);

void weapon_magazine_start_chamber(int weapon_index, int16_t magazine_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_magazine *magazine = &weapon->weapon.magazines[magazine_index];

    __int16 magazine_state = magazine->state;
    if ( magazine_state != _weapon_magazine_idle && magazine_state != _weapon_magazine_reloaded )
        return;

    if ( weapon->weapon.triggers[0].state || weapon->weapon.triggers[1].state || weapon->weapon.state )
        return;

    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    weapon_magazine_definition *magazine_definition =
        &((weapon_magazine_definition *)definition->weapon.magazines.address)[magazine_index];

    /* weapon_state_primary_chamber (3) for magazine 0, secondary_chamber (4) for magazine 1 */
    weapon_set_state(weapon_index, weapon_state_primary_chamber + magazine_index, 0);
    weapon_effect_new(weapon_index, magazine_definition->chambering_effect.index, 0.0f, 0.0f);

    magazine->state = _weapon_magazine_chambering;
    magazine->state_timer = (int)(magazine_definition->chamber_time * 30.0f);
}
