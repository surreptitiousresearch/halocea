/* action_flee_perform @0x83825920 — per-tick execution of an actor's flee behavior. Maintains the actor's
 * flee_state_data (which lives at actor+156): forces a fixed flee duration while panicking (state 9..12),
 * (re)selects a flee firing position when the current one is stale/invalid/exposed, marks the actor done
 * fleeing or unable to flee, emits the flee vocalization / ai_communication_event once per flee, and
 * returns whether the flee is resolved (done or unable). Sibling of action_flee_control /
 * action_flee_find_flee_position / action_flee_current_position_exposed.
 *
 * DEVIATIONS: (1) the DB prototype lists three params (actor_index, a2, a3) but the prologue reads only
 * r3 — a2/a3 are phantom (real signature is int action_flee_perform(int actor_index)); (2) the
 * action_flee_find_flee_position call is rendered with a spurious third arg — its real signature is
 * (actor_index, flee_state_data*), matching the committed extern. The prop fields use prop_datum's
 * modeled names. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/flee_state_data.h"
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/actor_panic_type.h"
#include "headers/ai_communication_type.h"
#include "headers/unit_scream_type.h"
#include "headers/ai_line_of_sight.h"
#include "headers/blam_data_globals.h"
#include "headers/ai_information_type.h"


#include "headers/ai_information_data.h"
extern int game_time_get(void);
extern unsigned __int8 action_flee_at_flee_position(int actor_index);
extern unsigned __int8 action_flee_current_position_exposed(unsigned __int16 actor_index,
        flee_state_data *flee_state_data);
extern void action_flee_find_flee_position(int actor_index, flee_state_data *state_data);
extern void actor_situation_update_target_status(uint16_t actor_index);
extern void actor_situation_combat_status_update(uint16_t actor_index);
extern uint8_t unit_is_speaking(uint16_t unit_index);
extern int unit_scream(int unit_index, int16_t scream_type);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);

int action_flee_perform(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    flee_state_data *flee = &actor->state.action_data.___u0.flee;

    if ( actor->meta.swarm )   /* actor inactive/dead — skip straight to the communication tail */
        goto communicate;

    /* Panicking (action state 9..12) forces a minimum flee duration. */
    /* recovered: *(__int16 *)(&actor->state.action_data + 12) -> flee->panic_type (offset 0x0C) */
    int action_state = flee->panic_type;
    if ( action_state >= _actor_panic_grenade_attached_to_us && action_state <= _actor_panic_burning_to_death )
        flee->forced_flee_ticks = 180;

    if ( flee->flee_stationary_ticks <= 0 )
    {
        if ( (unsigned __int16)flee->flee_firing_position_index == 0xFFFF )
        {
            flee->find_new_flee_position = 1;
        }
        else if ( actor->firing_positions.current_position_index == 0xFFFF )   /* actor lost its current firing position */
        {
            flee->flee_firing_position_index = -1;
            flee->find_new_flee_position = 1;
        }
        else if ( action_flee_at_flee_position(actor_index) )
        {
            if ( flee->forced_flee_ticks )
            {
                flee->find_new_flee_position = 1;
            }
            else
            {
                int flee_prop_index = flee->flee_prop_index;
                flee->flee_firing_position_index = actor->firing_positions.current_position_index;
                flee->flee_firing_position_found_randomly = actor->firing_positions.current_position_found_outside_range;
                flee->done_fleeing = 1;
                flee->find_new_flee_position = 0;
                if ( flee_prop_index != -1 )
                {
                    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, flee_prop_index);
                    __int16 audibility = prop->audibility;
                    __int16 ineffability = prop->ineffability;
                    prop->visibility = 0;
                    if ( audibility > ineffability )
                        ineffability = audibility;
                    prop->unopposable_enemy = 0;
                    prop->perception = ineffability;
                    prop->line_of_sight = _ai_line_of_sight_from_cover;
                    actor_situation_update_target_status(actor_index);
                    actor_situation_combat_status_update(actor_index);
                }
            }
        }
    }
    else
    {
        flee->flee_firing_position_index = -1;
    }

    /* Decide "done fleeing" for the terminal panic sub-states (grenade-attached..burning). */
    if ( flee->panic_type >= _actor_panic_grenade_attached_to_us
      && flee->panic_type <= _actor_panic_burning_to_death )
    {
        unsigned __int8 finished;
        if ( flee->panic_type == _actor_panic_delayed_projectile_attached_to_us || flee->panic_type == _actor_panic_grenade_attached_to_us )
        {
            finished = actor->input.delayed_attached_projectile_index == -1;   /* threat target cleared */
        }
        else if ( flee->panic_type == _actor_panic_melee_attached_to_us )
        {
            finished = actor->input.melee_attacker_attached == 0;
        }
        else   /* 12 */
        {
            finished = actor->input.burning_to_death == 0;
        }
        if ( finished )
            flee->done_fleeing = 1;
    }

    /* If actively threatened and not yet done, keep the flee position fresh. */
    if ( actor->meta.timeslice && !flee->done_fleeing )
    {
        if ( (unsigned __int16)flee->flee_firing_position_index != 0xFFFF
          && !flee->forced_flee_ticks
          && action_flee_current_position_exposed(actor_index, flee) )
        {
            flee->flee_firing_position_index = -1;
            flee->find_new_flee_position = 1;
        }

        if ( actor->input.vehicle_passenger )   /* actor disabled/blind */
        {
            flee->find_new_flee_position = 0;
            flee->unable_to_flee = 1;
            actor->emotions.last_flee_failed_time = game_time_get();
            goto communicate;
        }

        if ( flee->find_new_flee_position )
        {
            action_flee_find_flee_position(actor_index, flee);
            if ( (unsigned __int16)flee->flee_firing_position_index == 0xFFFF )
            {
                flee->unable_to_flee = 1;
                actor->emotions.last_flee_failed_time = game_time_get();
                goto communicate;
            }
        }
    }

communicate:
    {
        int panic_type = flee->panic_type;
        if ( panic_type >= _actor_panic_grenade_attached_to_us && panic_type <= _actor_panic_burning_to_death )
        {
            int unit_index = actor->meta.unit_index;
            if ( unit_index != -1 && !unit_is_speaking(unit_index) )
                flee->communicated_flee = 0;
        }
    }

    if ( flee->panic_type > _actor_panic_none
      && (unsigned __int16)flee->flee_firing_position_index != 0xFFFF
      && !flee->unable_to_flee
      && actor->meta.unit_index != -1 )
    {
        int now = game_time_get();
        if ( !flee->communicated_flee || flee->last_communication_time + 60 >= now )
        {
            __int16 panic_type = flee->panic_type;
            __int16 scream_type;
            if ( panic_type == _actor_panic_burning_to_death || panic_type == _actor_panic_melee_attached_to_us )
            {
                scream_type = _unit_scream_burning_to_death;
            }
            else if ( panic_type != _actor_panic_grenade_attached_to_us && panic_type != _actor_panic_delayed_projectile_attached_to_us )
            {
                int cause_unit_index = -1;
                if ( flee->flee_prop_index != -1 )
                    /* recovered: *((int *)prop_data->data + 78*idx + 6) -> prop.unit_index */
                    cause_unit_index = DATA_ARRAY_ELEMENT(prop_data, prop_datum, flee->flee_prop_index)->unit_index;
                int subject_unit_index = actor->meta.unit_index;
                if ( flee->communicated_flee )
                {
                    ai_communication_event(_ai_communication_flee_idle, subject_unit_index, cause_unit_index, -1, -1, -1, nullptr);
                }
                else
                {
                    ai_communication_event(panic_type == _actor_panic_friend_leader_type_killed ? _ai_communication_flee_leader_died : _ai_communication_flee, subject_unit_index, cause_unit_index,
                            -1, -1, _ai_information_flee, nullptr);
                    flee->communicated_flee = 1;
                }
                flee->last_communication_time = now;
                goto check_result;
            }
            else
            {
                scream_type = _unit_scream_grenade_attached_to_us;
            }
            unit_scream(actor->meta.unit_index, scream_type);
            flee->last_communication_time = now;
        }
    }

check_result:
    if ( flee->unable_to_flee )
        return 1;
    if ( flee->done_fleeing )
        return 1;
    return 0;
}