/* actor_emotion_update @ 0x837D9C98 — drive the actor's emotional/aggression state and its cover-fire
 * ("suppression") desire. Steps:
 *   1. Cancel berserk when the actor no longer qualifies (no combat status, low action category, or pinned).
 *   2. Emit a berserk start/stop communication event when the berserk flag transitions.
 *   3. Recompute the aggression target from the highest occupied threat-histogram bucket (actor+494..+503,
 *      filled by actor_situation_update) and low-pass the current aggression toward it.
 *   4. When the character is a "friendly-fire aware" type, scan friend_prop props to see whether the actor is in a
 *      friend_prop's line of fire (sets actor+860..+863), optionally requesting a firing-position discard.
 *   5. Evaluate the character-tag-driven suppression desire (mode at character+760 with thresholds at +764/
 *      +768) and run it through an on/off-delay hysteresis (actor+856 state, actor+858 timer).
 *
 * The character definition is resolved through TAG_INSTANCE to the DB actor_definition struct; actor fields
 * use named actor_datum members; prop fields use the prop_datum database layout. */

#include <stdint.h>
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include <math.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/prop_datum.h"
#include "headers/prop_perception_state.h" /* _prop_state_* enum */
#include "headers/prop_iterator.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/ai_communication_type.h"
#include "headers/defensive_crouch_type.h"
#include "headers/blam_data_globals.h"
#include "headers/ai_information_data.h"
extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);


extern void actor_berserk(uint16_t actor_index, uint8_t berserk);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern uint8_t actor_perception_friend_prop_is_attacking(uint16_t actor_index, uint16_t friend_prop_index, real_vector3d *attack_vector);
extern int16_t actor_perception_aiming_vector_test_blockage(const real_point3d *source_position, const real_vector3d *source_vector, const real_point3d *friend_position, real_vector3d *friend_direction_to_aiming_vector);
extern float normalize3d(real_vector3d *v);
extern void actor_discard_firing_position(uint16_t actor_index, int16_t firing_position_index, uint8_t temporary);
extern uint8_t actor_type_flood_desire_shamble(int actor_index);
extern void actor_emotion_unopposable_retreat(uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);

void actor_emotion_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);
    unsigned int character_flags = character->flags;

    /* 1. cancel berserk when no longer justified */
    if ( actor->emotions.berserk
      && (!actor->state.combat_status
          || actor->state.mode < _actor_mode_combat
          || (actor->input.shield_vitality == 1.0f && actor->state.combat_status < _actor_combat_status_definite)) )
        actor_berserk(actor_index, 0);

    /* 2. berserk transition -> communication event (22 = stopped, 23 = started) */
    unsigned int berserk = actor->external_orders.defending;
    if ( actor->emotions.currently_defending != berserk )
    {
        int unit_index = actor->meta.unit_index;
        actor->emotions.currently_defending = berserk;
        if ( unit_index != -1 )
            /* decompiler computed the event type via cntlzw(berserk); it reduces to 23 when set, 22 when cleared */
            ai_communication_event(berserk ? _ai_communication_retreat : _ai_communication_advance, unit_index, -1, -1, -1, -1, NULL);
    }

    /* "forced to charge" flag from berserk or the character's always-charge flag */
    char panic = (actor->emotions.berserk || (character_flags & (1u << _actor_definition_always_charge_bit)) != 0) ? 1 : 0;
    actor->emotions.forced_to_charge = panic;
    if ( actor->input.vehicle_index == -1 )
    {
        if ( (character_flags & (1u << _actor_definition_charge_in_attacking_mode_bit)) != 0 && !actor->emotions.currently_defending )
            actor->emotions.forced_to_charge = 1;
    }
    else
    {
        actor->emotions.forced_to_charge = 0;
    }

    /* 3. aggression target from the highest occupied threat-histogram bucket (actor+494..+503) */
    __int16 bucket = 9;
    while ( bucket > 0 && actor->situation.specific_threats[bucket] <= 0 )
        bucket = (__int16)(bucket - 1);

    if ( bucket >= 8 )       actor->emotions.instantaneous_danger = 2.0f;
    else if ( bucket >= 7 )  actor->emotions.instantaneous_danger = 1.8f;
    else if ( bucket >= 6 )  actor->emotions.instantaneous_danger = 1.6f;
    else if ( bucket >= 5 )  actor->emotions.instantaneous_danger = 1.2f;
    else if ( bucket >= 3 )  actor->emotions.instantaneous_danger = 0.69999999f;
    else                     actor->emotions.instantaneous_danger = 0.0f;

    /* low-pass current aggression (actor+852) toward the target (actor+848) */
    double smoothing_exponent;
    *(unsigned long long *)&smoothing_exponent = 0xBFA7A8D000000000ULL;
    double decay = exp(smoothing_exponent);
    actor->emotions.perceived_danger += (actor->emotions.instantaneous_danger - actor->emotions.perceived_danger)
                             * (float)(1.0 - decay);
    if ( actor->external_orders.stand_down )
        actor->emotions.original_body_vitality = actor->input.body_vitality;

    /* 4. friendly-fire awareness scan (crouch-in / avoid-friend line-of-fire flags) */
    if ( (character_flags & (1u << _actor_definition_crouch_in_line_of_fire_bit)) != 0
      || (character_flags & (1u << _actor_definition_avoid_friend_line_of_fire_bit)) != 0 )
    {
        if ( actor->input.vehicle_index == -1 && actor->state.combat_status >= _actor_combat_status_definite )
        {
            __int16 target_marker = actor->target.target_type;
            actor->emotions.crouch_blocking_player_line_of_fire = 0;
            actor->emotions.crouch_blocking_line_of_fire = 0;
            actor->emotions.crouch_friends_in_line_of_fire = 0;
            actor->emotions.moving_into_player_line_of_fire = 0;
            /* Left raw: decompiler bit-pun for the range test 0 <= target_marker <= 8 (a validity/bucket
             * gate over actor_target_type, not a single named enumerator); kept byte-exact. */
            int marker_in_range = ((target_marker >= 0) + ((unsigned int)target_marker <= 8)) & 1;

            real_vector3d actor_to_target;
            if ( marker_in_range )
            {
                prop_datum *target_prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index);
                actor_to_target = target_prop->actor_to_prop;
            }

            prop_iterator iterator;
            prop_iterator_new(&iterator, actor_index);
            for ( prop_datum *friend_prop = prop_iterator_next(&iterator); friend_prop; friend_prop = prop_iterator_next(&iterator) )
            {
                int state = friend_prop->state;
                if ( state < _prop_state_becoming_unacknowledged || state > _prop_state_acknowledged || friend_prop->enemy || friend_prop->dead || friend_prop->swarm
                  || !(friend_prop->player || friend_prop->vehicle_index == -1) )
                    continue;

                real_vector3d friend_prop_attack_vector;
                if ( actor_perception_friend_prop_is_attacking(actor_index, iterator.index, &friend_prop_attack_vector) )
                {
                    real_vector3d friend_prop_dir_to_aim;
                    __int16 blockage = actor_perception_aiming_vector_test_blockage(
                                           &friend_prop->body_position, &friend_prop_attack_vector,
                                           &actor->input.position.body_position, &friend_prop_dir_to_aim);
                    if ( blockage >= 1 )
                    {
                        actor->emotions.crouch_blocking_line_of_fire = 1;
                        if ( friend_prop->player )
                            actor->emotions.crouch_blocking_player_line_of_fire = 1;
                    }

                    if ( (character_flags & (1u << _actor_definition_avoid_friend_line_of_fire_bit)) != 0 && friend_prop->player && friend_prop->shooting )
                    {
                        float dist_sq = friend_prop_dir_to_aim.n[0] * friend_prop_dir_to_aim.n[0]
                                      + friend_prop_dir_to_aim.n[1] * friend_prop_dir_to_aim.n[1]
                                      + friend_prop_dir_to_aim.n[2] * friend_prop_dir_to_aim.n[2];
                        if ( dist_sq < 1.0f && (actor->control.moving || actor->emotions.moving_into_fire_timer > 0) )
                        {
                            real_vector3d move_dir;
                            move_dir.n[0] = actor->control.moving_towards_vector.n[0];
                            move_dir.n[1] = actor->control.moving_towards_vector.n[1];
                            move_dir.n[2] = actor->control.moving_towards_vector.n[2];
                            if ( normalize3d(&move_dir) > 0.0f )
                            {
                                real_point3d probe;
                                probe.n[0] = move_dir.n[0] * 0.40000001f + actor->input.position.body_position.x;
                                probe.n[1] = move_dir.n[1] * 0.40000001f + actor->input.position.body_position.y;
                                probe.n[2] = move_dir.n[2] * 0.40000001f + actor->input.position.body_position.z;
                                __int16 probe_blockage = actor_perception_aiming_vector_test_blockage(
                                                             &friend_prop->body_position, &friend_prop_attack_vector,
                                                             &probe, NULL);
                                if ( probe_blockage <= blockage )
                                    probe_blockage = blockage;
                                if ( probe_blockage >= 1 )
                                {
                                    float dot_threshold = dist_sq >= 0.25f ? 0.86602539f : 0.0f;
                                    float alignment = move_dir.n[1] * friend_prop_dir_to_aim.n[1]
                                                    + move_dir.n[2] * friend_prop_dir_to_aim.n[2]
                                                    + move_dir.n[0] * friend_prop_dir_to_aim.n[0];
                                    if ( alignment > dot_threshold )
                                        actor->emotions.moving_into_player_line_of_fire = 1;
                                }
                            }
                        }
                    }
                }

                if ( marker_in_range
                  && actor_perception_aiming_vector_test_blockage(&actor->input.position.body_position,
                         &actor_to_target, &friend_prop->body_position, NULL) >= 2 )
                    actor->emotions.crouch_friends_in_line_of_fire = 1;
            }
        }
        else
        {
            actor->emotions.crouch_blocking_player_line_of_fire = 0;
            actor->emotions.crouch_blocking_line_of_fire = 0;
            actor->emotions.crouch_friends_in_line_of_fire = 0;
            actor->emotions.moving_into_player_line_of_fire = 0;
        }
    }

    /* discard the current firing position when the actor decided it endangers a friend_prop (actor+863) */
    if ( actor->emotions.moving_into_player_line_of_fire )
    {
        actor_discard_firing_position(actor_index, actor->firing_positions.current_position_index, 1u);
        actor->emotions.moving_into_fire_timer = 22;
    }
    else
    {
        __int16 suppress_timer = actor->emotions.moving_into_fire_timer;
        if ( suppress_timer > 0 )
            actor->emotions.moving_into_fire_timer = suppress_timer - 1;
    }

    /* 5. suppression desire with on/off-delay hysteresis */
    __int16 desire_timer = actor->emotions.defensive_crouch_timer;
    if ( desire_timer > 0 )
    {
        actor->emotions.defensive_crouch_timer = desire_timer - 1;
        goto retreat;                                          /* delay still running: leave state unchanged */
    }

    float desire_threshold = (!actor->emotions.currently_defending || actor->emotions.berserk) ? character->defensive.defensive_threshold_attacking
                                                         : character->defensive.defensive_threshold_defending;
    unsigned __int8 desire;
    unsigned __int16 desire_mode = (unsigned __int16)character->defensive.defensive_crouch_type;

    if ( desire_mode < _defensive_crouch_danger || desire_mode > _defensive_crouch_flood_shamble )   /* _defensive_crouch_none or out of range */
    {
        desire = 0;
    }
    else if ( desire_mode == _defensive_crouch_shield_low )
    {
        desire = (actor->input.shield_vitality < desire_threshold);
    }
    else if ( desire_mode == _defensive_crouch_hide_behind_shield )
    {
        desire = (actor->input.shield_vitality > desire_threshold && actor->situation.cumulative_threats[2] > 0) ? 1 : 0;
    }
    else if ( desire_mode == _defensive_crouch_any_target )
    {
        desire = ((-actor->state.combat_status) & (unsigned int)~actor->state.combat_status) >> 31;
    }
    else if ( desire_mode == _defensive_crouch_danger )
    {
        desire = (actor->emotions.perceived_danger > desire_threshold);
    }
    else   /* _defensive_crouch_flood_shamble */
    {
        desire = actor_type_flood_desire_shamble(actor_index);
    }

    /* friendly-fire override: a friend_prop in the line of fire forces or clears the desire */
    if ( (character_flags & (1u << _actor_definition_crouch_in_line_of_fire_bit)) != 0 )
    {
        if ( actor->emotions.crouch_blocking_player_line_of_fire )
            desire = 1;
        else if ( actor->emotions.crouch_friends_in_line_of_fire )
            desire = 0;
        else if ( actor->emotions.crouch_blocking_line_of_fire )
            desire = 1;
    }

    if ( actor->emotions.defensive_crouch )                                    /* desire currently active */
    {
        if ( !desire )
        {
            actor->emotions.defensive_crouch = 0;
            if ( character->defensive.defensive_crouch_min_stand_time > 0.0f )          /* off-delay */
            {
                actor->emotions.defensive_crouch_timer = (int)(character->defensive.defensive_crouch_min_stand_time * 30.0f);
                goto retreat;
            }
            actor->emotions.defensive_crouch_timer = 45;
            goto retreat;
        }
        goto retreat;                                          /* still desired: keep active */
    }

    if ( !desire )
        goto retreat;

    actor->emotions.defensive_crouch = 1;                                      /* desire becomes active */
    if ( character->defensive.defensive_crouch_min_crouch_time > 0.0f )                  /* on-delay */
    {
        actor->emotions.defensive_crouch_timer = (int)(character->defensive.defensive_crouch_min_crouch_time * 30.0f);
        goto retreat;
    }
    actor->emotions.defensive_crouch_timer = 45;

retreat:
    {
        __int16 retreat_timer = actor->emotions.evasion_delay_timer;
        if ( retreat_timer > 0 )
            actor->emotions.evasion_delay_timer = retreat_timer - 1;
    }
    actor_emotion_unopposable_retreat(actor_index);
}