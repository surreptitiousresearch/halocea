/* actor_action_handle_grenade_throwing @0x837F4960 — per-tick check of whether an actor should
 * consider (and then attempt) throwing a grenade. Requires target.target_type (the graded target-knowledge
 * scale) to be at least 5; if the actor's current action is a shoot action (state.action == actor_action_flee), additionally
 * requires that action's ammo/shot counter to be non-positive. If not ready, clears the
 * want-to-throw-grenade flag and bails. Otherwise reads the actor variant definition's grenade type
 * (tag word at index 194): type 1 always considers a grenade if the actor hasn't already decided to hold
 * fire (state.combat_status < _actor_combat_status_clear_los -- read as combat_status here, matching actor_state_data.combat_status);
 * type 2 considers a grenade if the actor's current attack-of-opportunity prop is swarming, or if it's
 * mid-shoot-action with a zero shot counter. actor_action_consider_grenade sets the want-to-throw flag on
 * success; if set, actually attempts the throw via actor_action_try_to_throw_grenade.
 *
 * DEVIATION: offsets 616 and 624 resolve to actor->target.target_type and actor->target.target_prop_index
 * (actor_target_data); the "grenade-readiness timer" reading was a prior guess. The remaining fields are resolved:
 * the action-state counter to the flee arm's panic_type (action_data+0xC) and the want-to-throw-grenade
 * flag to actor->control.grenade_trying_to_throw (+1696). The
 * "v4 = (~(sign_bit) + (x!=0)) & 1" bit-twiddle the decompiler emits for the ammo/shot-counter check is
 * algebraically just "x <= 0" (verified by cases: x>0 -> 0, x==0 -> 1, x<0 -> 1) and is simplified here. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_action.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_panic_type.h"
#include "headers/actor_target_type.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_grenade_stimulus.h"
#include "headers/blam_data_globals.h"


extern unsigned __int8 actor_action_consider_grenade(int actor_index);
extern int actor_action_try_to_throw_grenade(int actor_index, uint8_t known_trajectory);

uint8_t actor_action_handle_grenade_throwing(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    unsigned __int8 result = 0;

    /* recovered: raw word offset 616 -> actor->target.target_type — the graded target-knowledge scale
     * (actor_action_can_stop_conversing gates on >=6 alerted / >=9 visible enemy); the prior
     * "grenade-readiness timer" name was a guess. */
    __int16 target_knowledge_level = actor->target.target_type;

    unsigned __int8 ready_to_throw;
    if ( target_knowledge_level < actor_target_uninspected_orphan )
    {
        ready_to_throw = 0;
    }
    else
    {
        ready_to_throw = 1;
        if ( actor->state.action == actor_action_flee )
        {
            __int16 shoot_action_shot_counter = actor->state.action_data.___u0.flee.panic_type;
            ready_to_throw = (shoot_action_shot_counter <= 0);
        }
    }

    if ( !ready_to_throw )
    {
        /* recovered: raw byte offset 1696 -> actor->control.grenade_trying_to_throw */
        actor->control.grenade_trying_to_throw = 0;
        return result;
    }

    /* [194] = abs byte 0x184 = variant grenade_combat.stimulus_type (NOT grenade_type: that was the old
     * "==2 trap" — grenade_type has only 0/1, but stimulus_type legitimately has visibletarget/seekcover). */
    actor_variant_definition *variant_definition =
        TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);
    __int16 grenade_stimulus = variant_definition->grenade_combat.stimulus_type;

    if ( grenade_stimulus == actor_grenade_stimulus_visibletarget )
    {
        if ( actor->state.combat_status >= _actor_combat_status_clear_los )
            result = actor_action_consider_grenade(actor_index);
    }
    else if ( grenade_stimulus == actor_grenade_stimulus_seekcover )
    {
        /* recovered: raw dword offset 624 -> actor->target.target_prop_index */
        int prop_index = actor->target.target_prop_index;
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

        if ( prop->swarm || (actor->state.action == actor_action_flee && actor->state.action_data.___u0.flee.panic_type == _actor_panic_none) )
            result = actor_action_consider_grenade(actor_index);
    }

    /* recovered: raw byte offset 1696 -> actor->control.grenade_trying_to_throw (set by actor_action_consider_grenade) */
    if ( actor->control.grenade_trying_to_throw )
        actor_action_try_to_throw_grenade(actor_index, 0);

    return result;
}
