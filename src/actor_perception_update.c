/* actor_perception_update @ 0x837DBDD8 — advance the actor's perception of the world for the frame. First (when
 * awake) it refreshes perception and the danger zone, and runs the danger-zone acknowledgement logic (deciding
 * whether to notice/react to a hazard, with a random gate from the character tag). Then it walks every prop
 * (the actor's per-unit knowledge), ages all the per-prop timers, recomputes each prop's "in use" status and
 * service timer, runs the prop perception/awareness state machine (prop_datum.state, values per
 * prop_perception_state: 0 unacknowledged, 1 becoming_acknowledged, 2 becoming_unacknowledged, 3 acknowledged,
 * 4 uninspected_orphan, 5 inspected_orphan), and fires the resulting stimulus/communication events (sighting,
 * allegiance change, entering combat, calls for help). Finally it picks the nearest orphan prop, ages the
 * "definitely located" tracking, and publishes the highest prop service timer back to the actor.
 *
 * A couple of PPC sign tests ((-x & ~x) >> 31) are reconstructed as the (x > 0) comparisons they compute. */

#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/actor_position_data.h"
#include "headers/ai_information_data.h"
#include "headers/real_point3d.h"
#include "headers/actor_action.h"
#include "headers/actor_danger_zone_type.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_target_type.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_mode.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/ai_information_type.h"
#include "headers/direction_specification_type.h"
#include "headers/secondary_look_type.h"
#include "headers/blam_data_globals.h"
#include "headers/real_point3d.h"
extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);


extern const __int16 global_acknowledgement_speeds[4][4];

extern void actor_perception_refresh(int actor_index);
extern void actor_perception_refresh_danger_zone(int actor_index);
extern void actor_stimulus_noticed_danger_zone(uint16_t actor_index, int16_t danger_type, int16_t danger_hostility, int danger_object_index, const real_point3d *position);
extern void prop_position_refresh(int actor_index, int prop_index, actor_position_data *sense_position, uint8_t forced_position_update, uint8_t full_status_update);
extern void prop_status_refresh(int actor_index, int prop_index, actor_position_data *sense_position);
extern uint8_t actor_perception_desire_prop(uint16_t actor_index, int16_t desired_target_state, int unit_index, int unit_actor_index, uint8_t in_use, uint8_t player, uint8_t enemy, uint8_t dead, int16_t dead_ticks, float suicide_radius, float distance_squared, int16_t required_ticks, uint8_t *optional_reference);
extern void actor_perception_find_prop_pathfinding_location(uint16_t actor_index, uint16_t prop_index);
extern int prop_orphan_transition(int actor_index, int parent_prop_index);
extern void actor_switch_props(uint16_t actor_index, int prop_index, int replacement_prop_index);
extern int16_t actor_get_perception_knowledge(uint16_t actor_index, int prop_index);
extern uint8_t actor_perception_become_acknowledged(int actor_index, int prop_index, uint8_t *expected_reference);
extern uint8_t actor_compute_prop_unopposable(uint16_t actor_index, int prop_index);
extern float actor_compute_prop_target_weight(int actor_index, int prop_index);
extern void prop_delete(int actor_index, int prop_index);
extern void actor_stimulus_prop_just_killed(uint16_t actor_index, uint16_t prop_index);
extern void actor_stimulus_prop_sighted(uint16_t actor_index, uint16_t prop_index, uint8_t new_enemy);
extern void actor_stimulus_enter_combat_perceived_enemy(uint16_t actor_index, uint16_t prop_index);
extern void actor_stimulus_enter_combat_found_body(uint16_t actor_index, uint16_t prop_index);
extern uint8_t actor_in_combat(uint16_t actor_index);
extern uint8_t actor_is_fighting(uint16_t actor_index);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern int game_time_get(void);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);

void actor_perception_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    unsigned __int8 dormant = actor->meta.dormant;
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);

    double closest_orphan_distance = 3.4028235e38;
    int    best_orphan_index = -1;
    char   serviced_a_prop = 0;
    __int16 highest_timer = 1;
    int    nearest_index = -1;

    if ( !dormant )
    {
        if ( actor->meta.timeslice )
            actor_perception_refresh(actor_index);
        actor_perception_refresh_danger_zone(actor_index);

        __int16 danger_type = actor->danger_zone.danger_type;
        if ( danger_type != actor_danger_zone_none )
        {
            unsigned __int8 danger_acknowledge = 0;
            if ( actor->danger_zone.attached_to_us || actor->danger_zone.hostility )
            {
                actor->danger_zone.noticed_danger = 1;
                danger_acknowledge = actor->danger_zone.acknowledgement_timer > 0;   /* (-x & ~x) >> 31 == (x > 0) */
                actor->danger_zone.acknowledgement_timer = 0;
            }
            else
            {
                __int16 timer = actor->danger_zone.acknowledgement_timer;
                if ( timer > 0 && actor->danger_zone.currently_perceived )
                {
                    int last_danger_time = actor->state.uncertain_combat_timer;
                    if ( last_danger_time == -1 || last_danger_time >= 60 )
                    {
                        __int16 next = (__int16)(timer - 1);
                        actor->danger_zone.acknowledgement_timer = next;
                        danger_acknowledge = (next == 0);
                    }
                    else
                    {
                        danger_acknowledge = 1;
                        actor->danger_zone.acknowledgement_timer = 0;
                    }
                }
            }

            if ( danger_acknowledge )
            {
                if ( danger_type == actor_danger_zone_suicide )
                {
                    actor->danger_zone.noticed_danger = 1;
                }
                else if ( danger_type == actor_danger_zone_projectile || danger_type == actor_danger_zone_vehicle )
                {
                    float chance = (danger_type == actor_danger_zone_projectile)
                                 ? actor_def->perception.notice_projectile_chance
                                 : actor_def->perception.notice_vehicle_chance;
                    if ( chance > 0.0f )
                    {
                        unsigned int *seed = get_global_random_seed_address();
                        if ( real_seed_random(seed) < chance )
                            actor->danger_zone.noticed_danger = 1;
                    }
                }

                if ( actor->danger_zone.noticed_danger )
                {
                    if ( actor->danger_zone.attached_to_us )
                        actor->danger_zone.allow_dive_evasion = 0;
                    else if ( actor->danger_zone.hostility || actor->danger_zone.danger_type == actor_danger_zone_vehicle
                           || actor->danger_zone.danger_type == actor_danger_zone_suicide )
                        actor->danger_zone.allow_dive_evasion = 1;
                    else
                    {
                        unsigned int *seed = get_global_random_seed_address();
                        actor->danger_zone.allow_dive_evasion = real_seed_random(seed) < actor_def->moving.grenade_dive_chance;
                    }
                    actor_stimulus_noticed_danger_zone(actor_index, actor->danger_zone.danger_type,
                                                       actor->danger_zone.hostility, actor->danger_zone.object_index,
                                                       &actor->danger_zone.position);
                }
            }

            if ( !actor->danger_zone.acknowledgement_timer )
            {
                if ( actor->control.secondary_look_type == _secondary_look_dangerous_object )
                {
                    __int16 priority = actor->control.secondary_look_priority;
                    if ( priority > 5 )
                        priority = 5;
                    actor->control.secondary_look_priority = priority;
                }
                if ( actor->danger_zone.attached_to_us )
                {
                    actor->danger_zone.noticed_danger = 1;
                    actor->danger_zone.allow_dive_evasion = 0;
                }
            }
        }
    }

    prop_iterator iterator;
    actor_position_data sense_position;
    prop_iterator_new(&iterator, actor_index);

    for ( prop_datum *prop = prop_iterator_next(&iterator); prop; prop = prop_iterator_next(&iterator) )
    {
        __int16 new_state = -1;
        unsigned __int8 acknowledged_expected[2] = { 0, 0 };
        char expired_orphan = 0;
        char needs_position_refresh = 0;
        unsigned __int8 serviced = 0;
        unsigned __int8 became_acknowledged = 0;

        /* ---- age the per-prop timers ---- */
        if ( prop->unit_effect_decay_ticks > 0 )
        {
            __int16 d = prop->unit_effect_decay_ticks - 1;
            prop->unit_effect_decay_ticks = d;
            if ( !d )
                prop->unit_effect = -1;
        }
        if ( prop->ticks_since_damage != -1 )
        {
            __int16 d = prop->ticks_since_damage + 1;
            prop->ticks_since_damage = d;
            if ( d >= 45 )
                prop->currently_damaging_me = 0;
        }
        if ( prop->ticks_since_definitely_located != -1 )
        {
            __int16 d = prop->ticks_since_definitely_located + 1;
            prop->ticks_since_definitely_located = d;
            if ( d >= 60 )
            {
                prop->definitely_located = 0;
                prop->definite_knowledge_source_actor = -1;
            }
        }
        if ( prop->dead )
            ++prop->dead_ticks;
        else
            prop->dead_ticks = 0;
        if ( prop->ticks_until_orphan > 0 )
            prop->ticks_until_orphan = prop->ticks_until_orphan - 1;
        if ( prop->required_ticks > 0 && !prop->delay_requirement_decision )
            prop->required_ticks = prop->required_ticks - 1;
        if ( prop->unreachable_ticks > 0 && prop->unreachable_ticks != 0x7FFF )
            prop->unreachable_ticks = prop->unreachable_ticks + 1;
        if ( prop->unopposable_casualty_decay_timer > 0 )
        {
            __int16 d = prop->unopposable_casualty_decay_timer - 1;
            prop->unopposable_casualty_decay_timer = d;
            if ( !d )
            {
                __int16 c = (__int16)(prop->unopposable_casualties_inflicted - 1);
                prop->unopposable_casualties_inflicted = c;
                if ( c > 0 )
                    prop->unopposable_casualty_decay_timer = 750;
            }
        }
        if ( prop->visibility < 2 )
            prop->visible_ticks = 0;
        else if ( prop->visible_ticks != 0x7FFF )
            ++prop->visible_ticks;

        /* ---- service timer + in-use status ---- */
        if ( dormant )
        {
            prop->timer = 0;
            prop->in_use = 0;
        }
        else
        {
            unsigned __int8 enemy = prop->enemy;
            __int16 weighted = ++prop->timer;
            if ( !enemy )
                weighted >>= 3;
            if ( prop->quantized_distance >= 3 )
                weighted >>= 1;
            if ( !serviced_a_prop && weighted >= actor->meta.highest_prop_timer )
            {
                serviced = 1;
                prop->timer = 0;
                needs_position_refresh = 1;
                weighted = 0;
                serviced_a_prop = 1;
            }
            if ( weighted > highest_timer )
                highest_timer = weighted;

            int state = prop->state;
            if ( (unsigned int)state >= _prop_state_becoming_unacknowledged || prop->___u3.orphan_prop_index == -1 )
            {
                if ( actor->meta.swarm )
                {
                    prop->in_use = 0;
                }
                else
                {
                    unsigned __int8 in_use =
                           actor->target.target_prop_index == iterator.index
                        || actor->meta.interesting_orphan_index  == iterator.index
                        || actor->emotions.unopposable_retreat_prop_index == iterator.index
                        || actor->external_orders.pursuit_group_prop_index == iterator.index
                        || (actor->control.secondary_look_type && actor->control.secondary_look_direction.type == _direction_specification_prop
                            && actor->control.secondary_look_direction.___u1.prop_index == iterator.index)
                        || (actor->control.idle_major_active && actor->control.idle_major_direction.type == _direction_specification_prop && actor->control.idle_major_direction.___u1.prop_index == iterator.index)
                        || (actor->control.idle_minor_active && actor->control.idle_minor_direction.type == _direction_specification_prop && actor->control.idle_minor_direction.___u1.prop_index == iterator.index);
                    prop->in_use = in_use;
                    if ( state >= _prop_state_uninspected_orphan && state <= _prop_state_inspected_orphan )
                        DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop->___u3.orphan_prop_index)->in_use = in_use;
                }
            }
            if ( prop->in_use && (unsigned int)prop->state >= _prop_state_becoming_unacknowledged )
                needs_position_refresh = 1;
            if ( needs_position_refresh )
                prop_position_refresh(actor_index, iterator.index, &sense_position, 0, serviced);
            if ( serviced )
                prop_status_refresh(actor_index, iterator.index, &sense_position);
        }

        /* ---- perception/awareness state machine ---- */
        unsigned __int16 state = (unsigned __int16)prop->state;
        if ( state <= _prop_state_inspected_orphan )
        {
            if ( state == _prop_state_becoming_acknowledged )
                goto accumulate_awareness;
            if ( state == _prop_state_becoming_unacknowledged )
            {
                if ( prop->perception <= 0 )
                {
                    float moved_x = prop->body_position.n[0] - prop->last_perceived_body_position.n[0];
                    float moved_y = prop->body_position.n[1] - prop->last_perceived_body_position.n[1];
                    if ( !prop->ticks_until_orphan || (moved_x * moved_x + moved_y * moved_y) > 1.0f )
                    {
                        int replacement = -1;
                        if ( actor_perception_desire_prop(actor_index, _prop_state_uninspected_orphan, prop->unit_index, prop->actor_index,
                                                          prop->in_use, prop->player, prop->enemy, prop->dead,
                                                          prop->dead_ticks, prop->suicide_radius,
                                                          prop->distance * prop->distance, prop->required_ticks,
                                                          NULL) )
                        {
                            unsigned __int8 orphan_serviced = 0;  /* orphan-transition refresh is never the serviced prop */
                            prop_position_refresh(actor_index, iterator.index, &sense_position, 0, orphan_serviced);
                            actor_perception_find_prop_pathfinding_location(actor_index, iterator.index);
                            replacement = prop_orphan_transition(actor_index, iterator.index);
                        }
                        actor_switch_props(actor_index, iterator.index, replacement);
                        new_state = _prop_state_unacknowledged;
                    }
                }
                else
                {
                    new_state = _prop_state_acknowledged;
                }
            }
            else if ( state == _prop_state_acknowledged && prop->state )
            {
                if ( !prop->perception )
                {
                    if ( actor_perception_desire_prop(actor_index, _prop_state_uninspected_orphan, prop->unit_index, prop->actor_index,
                                                      prop->in_use, prop->player, prop->enemy, prop->dead,
                                                      prop->dead_ticks, prop->suicide_radius,
                                                      prop->distance * prop->distance, prop->required_ticks,
                                                      NULL) )
                    {
                        new_state = _prop_state_becoming_unacknowledged;
                    }
                    else
                    {
                        actor_switch_props(actor_index, iterator.index, -1);
                        new_state = _prop_state_unacknowledged;
                    }
                }
            }
            else
            {
                if ( prop->state == _prop_state_unacknowledged )
                {
                    if ( prop->perception <= 0 )
                        goto apply_state;
                    prop->awareness = 0.0f;
                    new_state = _prop_state_becoming_acknowledged;
accumulate_awareness:
                    if ( prop->perception )
                    {
                        __int16 knowledge = actor_get_perception_knowledge(actor_index, iterator.index);
                        __int16 speed = global_acknowledgement_speeds[knowledge][prop->perception];
                        float gain;
                        if ( speed == 1 )      gain = actor_def->perception.runtime_awareness_delta_non_combat;
                        else if ( speed == 2 ) gain = actor_def->perception.runtime_awareness_delta_guard;
                        else if ( speed == 3 ) gain = actor_def->perception.runtime_awareness_delta_combat;
                        else if ( speed )      gain = 1.0f;
                        else                   gain = 0.0f;
                        prop->awareness = gain + prop->awareness;
                        if ( prop->awareness >= 1.0f )
                            new_state = _prop_state_acknowledged;
                    }
                    else
                    {
                        prop->awareness = 0.0f;
                        new_state = _prop_state_unacknowledged;
                    }
                    goto apply_state;
                }

                /* uninspected/inspected orphan handling */
                if ( state == _prop_state_uninspected_orphan )
                {
                    char inspecting = 0;
                    __int16 inspection_threshold = actor->input.vehicle_gunner_bombardment ? 300 : 45;
                    /* Both reads are faithful decompiler type-puns over the DB control layout:
                     * weapon_maximum_range (float @412) read as __int16, current_fire_target_type
                     * (__int16 @416) read as int. Kept byte-exact — the reinterpretation is the
                     * decompiler's; do not "simplify" to a float/short compare. */
                    if ( prop->visibility >= 2
                      || (*((__int16 *)&actor->control.weapon_maximum_range) == 1 && *((int *)&actor->control.current_fire_target_type) == iterator.index
                          && game_time_get() % 3 == 0) )
                        inspecting = 1;
                    if ( inspecting )
                    {
                        __int16 ticks = (__int16)(prop->orphan_inspection_ticks + 1);
                        prop->orphan_inspection_ticks = ticks;
                        if ( ticks >= inspection_threshold )
                            new_state = _prop_state_inspected_orphan;
                    }
                }

                /* orphan lifespan decay, rate depends on how the prop is referenced */
                __int16 decay;
                if ( iterator.index == actor->emotions.unopposable_retreat_prop_index
                  || (actor->state.action == actor_action_flee && actor->state.action_data.___u0.flee.flee_prop_index == iterator.index) )
                    decay = 0;
                else if ( iterator.index == actor->target.target_prop_index )
                    decay = prop->abandoned_search != 0;
                else if ( iterator.index == actor->meta.interesting_orphan_index )
                    decay = (actor->state.combat_status < _actor_combat_status_certain) ? 1 : 6;
                else
                    decay = 10;
                __int16 lifespan = (__int16)(prop->orphan_lifespan_ticks - decay);
                prop->orphan_lifespan_ticks = lifespan;
                if ( lifespan < 0 )
                    expired_orphan = 1;
            }
        }

apply_state:
        if ( new_state != -1 )
        {
            if ( (unsigned int)new_state <= _prop_state_inspected_orphan )
            {
                if ( new_state == _prop_state_unacknowledged )
                    goto clear_definite;
                if ( new_state != _prop_state_becoming_acknowledged )
                {
                    if ( new_state == _prop_state_becoming_unacknowledged )
                    {
                        __int16 t = (prop->visibility < 2) ? 10 : 60;
                        prop->ticks_until_orphan = t;
                    }
                    else if ( new_state == _prop_state_acknowledged )
                    {
                        became_acknowledged = actor_perception_become_acknowledged(actor_index, iterator.index,
                                                                                   acknowledged_expected);
                        iterator.next_index = prop->next_prop_index;
                    }
                    else
                    {
clear_definite:
                        prop->definite_knowledge_source_actor = -1;
                        prop->definitely_located = 0;
                    }
                }
            }
            prop->state = new_state;
            prop->unopposable_enemy = actor_compute_prop_unopposable(actor_index, iterator.index);
            prop->target_weight = actor_compute_prop_target_weight(actor_index, iterator.index);
        }

        /* ---- stimulus / communication events ---- */
        if ( expired_orphan )
        {
            DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop->___u3.orphan_prop_index)->___u3.orphan_prop_index = -1;
            actor_switch_props(actor_index, iterator.index, -1);
            prop_delete(actor_index, iterator.index);
        }
        else if ( prop->refresh_stimuli && prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged )
        {
            if ( prop->just_killed )
            {
                actor_stimulus_prop_just_killed(actor_index, iterator.index);
                prop->just_killed = 0;
            }
            if ( prop->just_became_visible || (became_acknowledged && prop->visibility > 0) )
            {
                unsigned __int8 new_enemy = (became_acknowledged && !acknowledged_expected[0]) ? 1 : 0;
                actor_stimulus_prop_sighted(actor_index, iterator.index, new_enemy);
                prop->just_became_visible = 0;
            }
            if ( !actor->emotions.sighted_friendly_player && !prop->enemy && prop->player && prop->visibility >= 2
              && prop->quantized_facing <= 2 && prop->distance < 7.0f )
            {
                unsigned __int8 new_enemy = 0;  /* friendly player sighting — never a new enemy */
                actor->emotions.sighted_friendly_player = 1;
                ai_communication_event(_ai_communication_sighted_friend_player, actor->meta.unit_index, prop->unit_index, _comm_hostility_friend, -1, -1, NULL);
                actor_stimulus_prop_sighted(actor_index, iterator.index, new_enemy);
            }
            if ( actor->meta.unit_index != -1 && !prop->dead && prop->ally && prop->ally_status_changed )
            {
                unsigned __int8 is_enemy = game_team_is_enemy(actor->meta.team_index, prop->team_index);
                float radius = is_enemy ? 15.0f : (prop->quantized_facing > 2 ? 3.0f : 10.0f);
                unsigned __int8 in_radius = prop->distance < radius;
                if ( is_enemy && prop->currently_damaging_me )
                    in_radius = 1;
                if ( in_radius )
                {
                    ai_information_data info;
                    info.___u0.allegiance.broken = is_enemy;
                    info.___u0.allegiance.team1_index = actor->meta.team_index;
                    info.___u0.allegiance.team2_index = prop->team_index;
                    ai_communication_event(_ai_communication_allegiance_changed, actor->meta.unit_index, prop->unit_index, is_enemy == 0 ? _comm_hostility_friend : _comm_hostility_traitor,
                                           -1, _ai_information_allegiance, &info);
                }
            }
            if ( actor->state.mode < _actor_mode_combat )
            {
                if ( !prop->dead || prop->enemy )
                {
                    if ( prop->enemy )
                        actor_stimulus_enter_combat_perceived_enemy(actor_index, iterator.index);
                }
                else
                {
                    actor_stimulus_enter_combat_found_body(actor_index, iterator.index);
                }
            }
            if ( !prop->enemy && !prop->dead && !prop->player )
            {
                char report = 0;
                if ( (actor->target.target_prop_index != -1
                      && (actor->target.since_any_target_visible_timer == -1 || actor->target.since_any_target_visible_timer >= 180))
                  || (actor->meta.encounter_index != -1
                      && (DATUM_GET(encounter_data, encounter_datum, actor->meta.encounter_index)->enemy_visible_timer == -1
                          || (DATUM_GET(encounter_data, encounter_datum, actor->meta.encounter_index)->enemy_visible_timer >= 180
                              && DATUM_GET(encounter_data, encounter_datum, actor->meta.encounter_index)->enemy_alive))) )
                    report = 1;
                if ( report )
                {
                    int unit = actor->meta.unit_index;
                    if ( unit != -1 )
                    {
                        if ( actor->state.mode >= _actor_mode_combat )
                        {
                            if ( actor_in_combat(actor_index) && !actor_is_fighting(actor_index)
                              && prop->noncombat && prop->visibility >= 2 )
                                ai_communication_event(_ai_communication_alert_noncombat, actor->meta.unit_index, prop->unit_index, _comm_hostility_friend, -1, _ai_information_combat_stimulus, NULL);
                        }
                        else if ( prop->in_combat )
                        {
                            ai_communication_event(_ai_communication_alert_noncombat, prop->unit_index, unit, _comm_hostility_friend, -1, _ai_information_combat_stimulus, NULL);
                        }
                    }
                }
            }
        }
        else
        {
            int state2 = prop->state;
            if ( state2 >= _prop_state_uninspected_orphan && state2 <= _prop_state_inspected_orphan && prop->distance < closest_orphan_distance )
            {
                closest_orphan_distance = prop->distance;
                nearest_index = iterator.index;
            }
        }
    }

    best_orphan_index = nearest_index;
    if ( actor->target.target_prop_index != -1 )
    {
        __int16 active_state = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index)->state;
        if ( active_state >= _prop_state_uninspected_orphan && active_state <= _prop_state_inspected_orphan )
            best_orphan_index = -1;
    }

    __int16 target_marker = actor->target.target_type;
    if ( target_marker >= actor_target_definite_orphan )
        actor->target.any_target_ever = 1;
    if ( target_marker < actor_target_visible_enemy )
    {
        if ( actor->external_orders.stand_down )
            actor->target.since_any_target_visible_timer = -1;
        else
        {
            int located_ticks = actor->target.since_any_target_visible_timer;
            if ( located_ticks != -1 )
                actor->target.since_any_target_visible_timer = located_ticks + 1;
        }
    }
    else
    {
        actor->target.since_any_target_visible_timer = 0;
    }

    actor->meta.interesting_orphan_index = best_orphan_index;
    actor->meta.highest_prop_timer = highest_timer;
}
