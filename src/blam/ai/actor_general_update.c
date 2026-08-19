/* actor_general_update @ 0x837210E8 — per-tick liveness/dormancy governor, run first in actor_update; returns
 * 0 to skip the rest of the actor's update (when the actor was erased), 1 otherwise. A swarm actor with no
 * swarm cache is erased outright. General countdown timers are aged. The actor is forced awake when it has no
 * dormant desire or its encounter is force-active; otherwise, if not already dormant and nothing keeps it
 * "interesting" (in combat, holding an active prop of the right kind, or actively pathing toward something),
 * its become-dormant timer climbs and it goes dormant after 60 ticks.
 *
 * Reads are resolved to named actor_datum members (verified against the decompiler). Named
 * meta fields: swarm @+6, force_active @+10, dormant_desire @+18, dormant @+19, encounter_index @+52,
 * swarm_cache_index @+40, become_dormant_timer @+20. Prop state is read by raw offset into prop_data (312-byte
 * stride). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/encounter_datum.h"
#include "headers/prop_datum.h"
#include "headers/actor_action.h"
#include "headers/action_class.h"
#include "headers/actor_combat_status.h"
#include "headers/prop_perception_state.h"
#include "headers/destination_type.h"
#include "headers/ai_follow_target_type.h"
#include "headers/blam_data_globals.h"


extern void actor_erase(int actor_index, uint8_t immediate);
extern void actor_set_dormant(int actor_index, uint8_t dormant);
extern int16_t actor_action_class(int actor_index);
extern uint8_t actor_path_has_path(int actor_index);

uint8_t actor_general_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->meta.swarm && actor->meta.swarm_cache_index == -1 )      /* swarm actor with no swarm cache */
    {
        actor_erase(actor_index, 0);
        return 0;
    }

    actor->control.path.refreshed_this_tick = 0;

    int general_timer = actor->state.suspicion_timer;
    if ( general_timer > 0 )
    {
        actor->state.suspicion_timer = general_timer - 1;
        if ( !(general_timer - 1) )
            actor->state.suspicion_combat_status = _actor_combat_status_none;
    }

    int16_t short_timer = actor->state.command_list_delay_timer;
    if ( short_timer > 0 )
        actor->state.command_list_delay_timer = short_timer - 1;

    int encounter_index = actor->meta.encounter_index;        /* meta.encounter_index */
    encounter_datum *encounter = (encounter_index == -1)
                    ? NULL
                    : DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);

    uint8_t force_active = actor->meta.force_active;           /* meta.force_active */
    if ( encounter )
        force_active |= encounter->force_active;

    if ( !actor->meta.dormant_desire || force_active )                   /* no dormant desire, or forced active */
    {
        actor_set_dormant(actor_index, 0);
        return 1;
    }

    if ( actor->meta.dormant )                                    /* already dormant */
        return 1;

    int16_t action_class = actor_action_class(actor_index);
    char can_become_dormant = 1;

    int active_prop_index = actor->target.target_prop_index;
    if ( action_class == _action_class_transitory )
    {
        can_become_dormant = 0;
    }
    else if ( active_prop_index != -1 )
    {
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, active_prop_index);
        if ( prop->player && prop->enemy && !prop->dead )
        {
            int16_t prop_state = prop->state;
            if ( (prop_state >= _prop_state_becoming_unacknowledged && prop_state <= _prop_state_acknowledged)
              || (prop_state >= _prop_state_uninspected_orphan && prop_state <= _prop_state_inspected_orphan && action_class == _action_class_pursuit) )
                can_become_dormant = 0;
        }
    }

    if ( can_become_dormant && actor_path_has_path(actor_index) )
    {
        uint16_t pathing_kind = actor->control.path.destination_orders.destination_type;
        if ( pathing_kind == _destination_firing_position )
        {
            if ( actor->state.action == actor_action_guard )       /* action type */
                can_become_dormant = (encounter && encounter->follow_target_type == _follow_target_players) ? 0 : can_become_dormant;
        }
        else if ( pathing_kind == _destination_prop )
        {
            prop_datum *path_prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->control.path.destination_orders.___u3.prop.prop_index);
            if ( path_prop->player )
                can_become_dormant = 0;
        }
    }

    if ( can_become_dormant )
    {
        int16_t become_dormant_timer = (int16_t)(actor->meta.become_dormant_timer + 1);
        actor->meta.become_dormant_timer = become_dormant_timer;
        if ( become_dormant_timer >= 60 )
            actor_set_dormant(actor_index, 1u);
    }

    return 1;
}
