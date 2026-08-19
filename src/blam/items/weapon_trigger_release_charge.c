/* weapon_trigger_release_charge @0x836DD198 — release a charging weapon trigger. If its definition has no
 * release-decay time (spew_time <= 0), the charge is dropped immediately: on a multi-trigger weapon this
 * also fires trigger 1, then the trigger's state/state_timer are cleared. Otherwise the trigger enters the
 * "releasing" state (6) with its timer set from spew_time (seconds -> 30Hz ticks). Either way, the
 * held-charge fraction (stored in triggers[i].rate_of_fire) is zeroed and the HCEX bridge is told the
 * weapon stopped charging.
 *
 * DEVIATION preserved as-is: the held-charge-fraction write at the end uses the trigger pointer computed
 * before weapon_trigger_fire() may run (unlike the 608/609/610 writes above it, which re-fetch afterward,
 * matching weapon_trigger_overload.c's documented re-fetch-after-fire precaution) — reproduced verbatim
 * rather than "fixed" to also re-fetch, since that's what the compiled code actually does.
 */

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
extern void hcex_obj_set_state(int object_index, const char *state_name);

void weapon_trigger_release_charge(int weapon_index, int16_t trigger_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_trigger *original_trigger = &weapon->weapon.triggers[trigger_index];

    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    weapon_trigger_definition *trigger_definition =
        &((weapon_trigger_definition *)definition->weapon.triggers.address)[trigger_index];
    float release_decay_time = trigger_definition->spew_time;

    if ( release_decay_time <= 0.0f )
    {
        if ( definition->weapon.triggers.count > 1 )
        {
            weapon_trigger_fire(weapon_index, 1);
            weapon = (weapon_datum *)
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
        }

        weapon_trigger *trigger = &weapon->weapon.triggers[trigger_index];
        trigger->state = _weapon_trigger_idle;
        trigger->idle_ticks = 0;
        trigger->state_timer = 0;
    }
    else
    {
        original_trigger->state = _weapon_trigger_releasing;
        /* DEVIATION: fmuls @0x836DD22C rounds the 30Hz product to single precision before
         * fctiwz @0x836DD234 truncates — explicit float local pins that rounding step. */
        float release_ticks = release_decay_time * 30.0f;
        original_trigger->state_timer = (int)release_ticks;
    }

    original_trigger->rate_of_fire = 0.0f;  /* held-charge fraction (DB member name kept) */
    hcex_obj_set_state(weapon_index, "charging_stop");
}
