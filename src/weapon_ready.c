/* weapon_ready @0x836DCCC0 — bring a weapon to the ready/equipped state: reset it, set weapon state 9
 * (ready), send the first-person "ready" message (12), spawn the definition's ready_effect, and record
 * the first-person ready-animation duration in weapon.state_timer. If the weapon is locally simulated
 * (datum_role == 0 — the old comment's "no parent" was wrong), set flag 0x4000000 so it shows
 * immediately. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_datum_state.h"
#include "headers/first_person_weapon_message_type.h"
#include "headers/object_flags.h"
#include "headers/first_person_weapon_animation.h"
#include "headers/blam_data_globals.h"

extern void weapon_reset(int weapon_index);
extern uint8_t weapon_set_state(int weapon_index, int16_t new_state, uint8_t immediate);
extern void first_person_weapon_message_from_weapon(int weapon_index, int16_t message_type);
extern int weapon_effect_new(int weapon_index, int effect_index, float effect_scale, float effect_error);
extern uint16_t weapon_get_first_person_animation_time(int weapon_index, int16_t mode, int16_t animation_type, int16_t shotgun_reload_type);

void weapon_ready(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);

    weapon_reset(weapon_index);
    weapon_set_state(weapon_index, weapon_state_ready, 1);
    first_person_weapon_message_from_weapon(weapon_index, _first_person_weapon_message_ready);
    weapon_effect_new(weapon_index, definition->weapon.ready_effect.index, 0.0f, 0.0f);

    int16_t animation_time = weapon_get_first_person_animation_time(weapon_index, 0,
                                 _first_person_weapon_animation_ready, -1);
    weapon->weapon.state_timer = animation_time;
    if ( !weapon->object.datum_role )
        weapon->object.flags |= (1u << _object_force_baseline_update_bit);
}
