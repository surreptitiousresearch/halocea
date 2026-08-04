/* action_search_update @0x83823208 — per-tick update for an actor's "search" action (searching for a lost
 * target). Runs only while the search is unresolved (actor->state.action_data[0] == 0). It first decides
 * whether the actor is "closing in" (actor->state.action_data[3]): forced when the actor variant's search
 * style is 4, otherwise when the variant allows it, the actor has no target, its state is 5, and its search
 * prop is close (prop quantized_distance <= 2). If a search timer is active (actor->state.action_data[2]) it counts the
 * timer (actor->state.action_data[36]) down, flags the search failed (actor->state.action_data[0]) when it
 * expires, and — when the actor has a communication target — issues a "found nothing" (18) call once the
 * timer expires if it has a target, or a one-shot "still searching" (13) call otherwise. If no search timer
 * is active and the actor is idle, an idle counter (actor->state.action_data[40]) runs up to 120 ticks
 * before flagging the search over.
 *
 * DEVIATION: the decompiler embeds `actor->state.action_data[3] = 0` inside the short-circuit of the
 * closing-in test; hoisted here to before the test (the final value is identical, no intervening read). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_definition_flags.h"
#include "headers/actor_definition.h"
#include "headers/defensive_crouch_type.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_target_type.h"
#include "headers/search_state_data.h"
#include "headers/prop_datum.h"
#include "headers/ai_communication_type.h"
#include "headers/blam_data_globals.h"


#include "headers/ai_information_data.h"
extern int actor_target_unit_index(uint16_t actor_index);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);

void action_search_update(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    search_state_data *search_resolved = &actor->state.action_data.___u0.search;

    if ( actor->state.action_data.___u0.search.search_done )
        return;

    actor_definition *actor_variant = TAG_GET(actor_definition, actor->meta.definition_index);

    actor->state.action_data.___u0.search.sneaking = 0;
    if ( actor_variant->defensive.defensive_crouch_type == _defensive_crouch_any_target
      || ((actor_variant->flags & (1u << _actor_definition_sneak_uncovering_target_bit)) != 0
          && !actor->state.action_data.___u0.search.pursuit_location.type
          && actor->target.target_type == actor_target_uninspected_orphan
          && (DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index))->quantized_distance <= 2) )
    {
        actor->state.action_data.___u0.search.sneaking = 1;
    }

    if ( actor->state.action_data.___u0.search.at_destination )
    {
        int search_timer = actor->state.action_data.___u0.search.search_remaining_time;
        if ( search_timer > 0 )
            actor->state.action_data.___u0.search.search_remaining_time = search_timer - 1;
        int remaining = actor->state.action_data.___u0.search.search_remaining_time;
        if ( !remaining )
            search_resolved->search_done = 1;
        if ( actor->meta.unit_index != -1 )
        {
            if ( actor->state.action_data.___u0.search.pursuit_location.type )
            {
                if ( !remaining )
                {
                    int unit = actor_target_unit_index(actor_index);
                    ai_communication_event(_ai_communication_search_report, actor->meta.unit_index, unit, -1, -1, -1, nullptr);
                }
            }
            else if ( !actor->firing_positions.pursuit_communicated_lost_contact && (search_resolved->search_done || remaining + 90 < actor->state.action_data.___u0.search.search_desired_time) )
            {
                int unit = actor_target_unit_index(actor_index);
                ai_communication_event(_ai_communication_lost_contact, actor->meta.unit_index, unit, -1, -1, -1, nullptr);
                actor->firing_positions.pursuit_communicated_lost_contact = 1;
            }
        }
    }
    else if ( !actor->control.moving && !actor->meta.swarm )
    {
        int idle_ticks = actor->state.action_data.___u0.search.search_failure_timer + 1;
        actor->state.action_data.___u0.search.search_failure_timer = idle_ticks;
        if ( idle_ticks > 120 )
        {
            actor->state.action_data.___u0.search.search_failed = 1;
            search_resolved->search_done = 1;
        }
    }
}
