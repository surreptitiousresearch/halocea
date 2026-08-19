/* action_charge_begin @0x837F9010 — decrement the actor's remaining-charge counter (word +767) when the
 * actor is charging (charge.goal == charge_goal_vehicle_strafing) and its firing variant's
 * special_fire_situation is _actor_special_fire_situation_strafing.
 * `special_fire_situation` is reached through the typed actor_variant_definition->ranged_combat member. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/charge_state_data.h"
#include "headers/charge_goal.h"
#include "headers/actor_special_fire_situation.h"
#include "headers/blam_data_globals.h"

extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);

void action_charge_begin(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->state.action_data.___u0.charge.goal == charge_goal_vehicle_strafing
      /* recovered: *(__int16 *)((char *)variant + 342) -> ranged_combat.special_fire_situation */
      && actor_combat_get_firing_variant_definition(actor_index)->ranged_combat.special_fire_situation == _actor_special_fire_situation_strafing )
    {
        int16_t charge_remaining = actor->control.special_fire_deny_attempts;

        if ( charge_remaining > 0 )
            actor->control.special_fire_deny_attempts = charge_remaining - 1;
    }
}
