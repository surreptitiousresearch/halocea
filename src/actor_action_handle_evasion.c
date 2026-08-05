/* actor_action_handle_evasion @0x837F4A98 — per-tick decision for whether an actor should take evasive
 * action (dive for cover / evade a threat / throw a grenade defensively). Runs three escalating checks:
 *   1. If the actor is tracking a swarm/orphan prop (+936 count > 0, no active vehicle at +344) whose
 *      unopposable_enemy flag is set and whose line_of_sight <= 1, and at least 30 ticks since the last
 *      attempt, try to seek cover (and optionally panic if the behavior tag allows it).
 *   2. If the evasion urgency (+852) exceeds the tag's dive threshold (choosing the crouched/standing
 *      threshold at +784/+788, clamped to 1.1 when the actor is airborne and a positive dive-chance is
 *      set), reseed the RNG (once), and if the variant's grenade type is 2, consider a defensive grenade.
 *   3. Compute two gates (v12 = "prop still dangerous", v13 = "not already fleeing"), then optionally
 *      seek cover again (RNG-gated by the tag's cover-seek chance at +792) firing an ai_communication_event,
 *      and finally fall back to a plain evade (setting the evasion cooldown timer at +872 from the tag's
 *      evade-duration at +796 * 30).
 * Returns whether any evasive action was taken.
 *
 * Sibling of actor_action_handle_active_cover_seeking / actor_action_handle_grenade_throwing.
 *
 * DEVIATIONS: (1) the two actor tags are read at raw meta offsets — the behavior tag at meta+0x58 (the
 * header's variant_definition_index) and the grenade/variant tag at meta+0x5C (unlabeled in the partial
 * actor_meta_data header; disasm-confirmed r30 = TAG_INSTANCE(*(int*)(actor+0x5C)), the same offset
 * actor_action_handle_grenade_throwing actually reads even though its source names it
 * variant_definition_index). (2) `(-x & ~x) >> 31` on the swarm count is the branchless `x > 0` on a
 * signed short. Actor fields are accessed through typed actor_datum members (emotions.*, input.*,
 * external_orders.*, target.*); the dive thresholds at +784/+788 are behavior_tag->defensive fields on
 * the actor_definition tag. */

#include <stdint.h>
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_action.h"
#include "headers/actor_grenade_stimulus.h"
#include "headers/actor_panic_type.h"
#include "headers/charge_goal.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_line_of_sight.h"
#include "headers/blam_data_globals.h"


#include "headers/ai_information_data.h"
extern int game_time_get(void);
extern uint8_t actor_action_allow_cover_seeking(int actor_index, uint8_t unopposable);
extern uint8_t actor_action_try_to_seek_cover(int actor_index, uint8_t cover_from_last_visible_location, uint8_t allow_occluded_points);
extern uint8_t actor_action_try_to_panic(int actor_index, int16_t panic_type, int panic_prop_index,
        uint8_t force_flee_transition);
extern uint8_t actor_action_consider_grenade(int actor_index);
extern uint8_t actor_action_try_to_evade(int actor_index);
extern int actor_target_unit_index(uint16_t actor_index);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);

uint8_t actor_action_handle_evasion(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    /* Grenade/variant tag at meta+0x5C, behavior tag at meta+0x58 (see header note). */
    const actor_variant_definition *variant_tag = TAG_GET(const actor_variant_definition, actor->meta.variant_definition_index);
    const actor_definition *behavior_tag = TAG_GET(const actor_definition, actor->meta.definition_index);

    int current_time = game_time_get();
    uint8_t acted = 0;

    /* +936 = emotions.unopposable_retreat_timer, +344 = input.vehicle_index,
     * +940 = emotions.unopposable_retreat_prop_index, +876 = emotions.last_defensive_cover_seeking_time. */
    if ( actor->emotions.unopposable_retreat_timer > 0 && actor->input.vehicle_index == -1 )
    {
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->emotions.unopposable_retreat_prop_index);
        if ( prop->unopposable_enemy && (unsigned int)prop->line_of_sight <= _ai_line_of_sight_occluded )
        {
            int last_attempt = actor->emotions.last_defensive_cover_seeking_time;
            if ( last_attempt == -1 || last_attempt + 30 <= current_time )
            {
                actor->emotions.last_defensive_cover_seeking_time = current_time;
                if ( actor_action_allow_cover_seeking(actor_index, 1u)
                  && (actor_action_try_to_seek_cover(actor_index, 0, 1u)
                      || ((behavior_tag->flags & (1u << _actor_definition_panicked_by_unopposable_enemy_bit)) != 0
                          && actor_action_try_to_panic(actor_index, _actor_panic_unopposable_enemy, actor->emotions.unopposable_retreat_prop_index, 0))) )
                {
                    acted = 1;
                }
            }
        }
    }

    if ( acted )
        return acted;

    /* Dive threshold: crouched (+788) if actor is stationary and not suppressed, else standing (+784);
     * both are behavior_tag->defensive fields on the actor_definition tag. Actor fields:
     * +884 = emotions.currently_defending, +888 = emotions.berserk, +458 = external_orders.playfighting,
     * +852 = emotions.perceived_danger (evasion urgency). */
    float dive_threshold;
    if ( !actor->emotions.currently_defending || actor->emotions.berserk )
        dive_threshold = behavior_tag->defensive.evasion_danger_threshold_attacking;
    else
        dive_threshold = behavior_tag->defensive.evasion_danger_threshold_defending;

    if ( actor->external_orders.playfighting && behavior_tag->defensive.evasion_seek_cover_chance > 0.0f && dive_threshold > 1.1f )
        dive_threshold = 1.1f;

    if ( actor->emotions.perceived_danger <= dive_threshold )
        return acted;

    /* +1284: RNG-already-seeded-this-tick flag. */
    if ( !actor->control.moving )
        real_seed_random(get_global_random_seed_address());

    /* Variant grenade stimulus "seek cover" → consider a defensive grenade. */
    if ( variant_tag->grenade_combat.stimulus_type == actor_grenade_stimulus_seekcover && actor_action_consider_grenade(actor_index) )
    {
        actor->emotions.perceived_danger = 0.0f;
        acted = 1;
    }

    /* v12 = "tracked prop still dangerous"; v13 = "not already committed to a fleeing action". */
    uint8_t prop_dangerous = 1;
    uint8_t not_fleeing = 1;

    if ( actor->emotions.defensive_crouch )   /* has a secondary tracked prop */
    {
        if ( (behavior_tag->flags & (1u << _actor_definition_crouch_try_not_to_move_bit)) != 0 )
        {
            prop_dangerous = 0;
            if ( actor->target.target_prop_index != -1 )
            {
                prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index);
                /* prop+0x122 (quantized_facing) <= 2 and prop+0x121 (quantized_distance) <= 1 */
                if ( prop->quantized_facing <= 2 )
                    prop_dangerous = prop->quantized_distance <= 1;
            }
        }
    }

    /* state.action == actor_action_charge with a melee/melee-leaping charge goal → fleeing. */
    if ( actor->state.action == actor_action_charge )
    {
        int16_t charge_goal = actor->state.action_data.___u0.charge.goal;
        if ( charge_goal == charge_goal_melee || charge_goal == charge_goal_melee_leaping )
            not_fleeing = 0;
    }

    if ( !acted && prop_dangerous )
    {
        int last_attempt = actor->emotions.last_defensive_cover_seeking_time;
        if ( last_attempt == -1 || last_attempt + 30 <= current_time )
        {
            actor->emotions.last_defensive_cover_seeking_time = current_time;
            if ( actor_action_allow_cover_seeking(actor_index, 0)
              && real_seed_random(get_global_random_seed_address()) < (double)behavior_tag->defensive.evasion_seek_cover_chance
              && actor_action_try_to_seek_cover(actor_index, 0, 1u) )
            {
                int target_unit = actor_target_unit_index(actor_index);
                ai_communication_event(_ai_communication_cover, actor->meta.unit_index, target_unit, -1, -1, -1, nullptr);
                actor->emotions.perceived_danger = 0.0f;
                acted = 1;
            }
        }
    }

    /* +872 evasion cooldown timer (word); non-zero means still evading. */
    if ( acted || !not_fleeing || actor->emotions.evasion_delay_timer || !actor_action_try_to_evade(actor_index) )
        return acted;

    actor->emotions.perceived_danger = 0.0f;
    actor->firing_positions.moved_away_from_firing_position = 1;
    actor->emotions.evasion_delay_timer = (int)(behavior_tag->defensive.evasion_delay_timer * 30.0f);
    return 1;
}