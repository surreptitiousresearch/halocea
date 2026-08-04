/* weapon_trigger_fully_charged @0x836DB368 — transition one weapon trigger into the fully-charged state
 * (state byte weapon+40*trigger+609 = 3) with its hold timer set from the definition's charged time
 * (trigger definition float +76, seconds → 30Hz ticks, into the timer word +610), push the weapon into the
 * per-trigger charged weapon state (7 + trigger_index), and notify the first-person weapon layer
 * (message 14 = charged). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/weapon_trigger_state.h"
#include "headers/first_person_weapon_message_type.h"
#include "headers/blam_data_globals.h"


extern uint8_t weapon_set_state(int weapon_index, int16_t new_state, uint8_t immediate);
extern void first_person_weapon_message_from_weapon(int weapon_index, int16_t message_type);

void weapon_trigger_fully_charged(int weapon_index, int16_t trigger_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_trigger *trigger = &weapon->weapon.triggers[trigger_index];

    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    float charged_time =
        ((weapon_trigger_definition *)definition->weapon.triggers.address)[trigger_index].charged_time;
    trigger->state = _weapon_trigger_charged;
    trigger->state_timer = (__int16)(int)(charged_time * 30.0f);

    weapon_set_state(weapon_index, trigger_index + 7, 1u);
    first_person_weapon_message_from_weapon(weapon_index, _first_person_weapon_message_charged);
}
