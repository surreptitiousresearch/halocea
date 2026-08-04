/* actor_type_flood_desire_shamble @0x8381F1F0 — decide whether a (flood combat-form) actor wants to "shamble":
 * crouch-switch side to side while holding its ground during ranged combat, and if so, which side.
 *
 * First establishes whether the actor is even a candidate this tick (busy/pathing, mode, combat status,
 * a specific "dying" animation state, target range vs. the firing variant's max/berserk range, a unit damage
 * flag, berserk, a specific melee-wind-up action, no ranged weapon, underwater, or the variant's movement
 * type disabling switching all veto it outright, returning a fixed 0/1; movement_type==1 forces candidacy;
 * otherwise being within the variant's combat range does). If not a candidate, clears crouch_switching_active
 * and returns the veto result.
 *
 * If a candidate and not already decided this bout (`!crouch_switching_active`): blends the variant's
 * initial_crouch_chance toward the local group's current crouch/stand split among nearby same-type,
 * non-hostile props (more standing neighbors pulls the chance down, more crouching neighbors pulls it up),
 * rolls against it, and latches both crouch_switching_active and crouch_switching_current_value (which
 * side/pose to take).
 *
 * Otherwise (already decided): counts down a short re-roll cooldown (crouch_switching_change_timer); once it
 * elapses (gated by the variant's flags bit 0x8 and having any close friends), classifies nearby same-type,
 * non-hostile, non-dead, close (<15u) props already mid-switch by which side of the actor-to-target axis
 * they're on (dot of the target's actor_to_prop vector against the neighbor-relative-position vector: >1.4 =
 * clearly one side, <-1.4 = clearly the other, in between = ambiguous), and flips to the opposite side if the
 * majority of unambiguous neighbors are on the SAME side as the actor currently is (so the group spreads
 * across both flanks instead of stacking). Independently also flips once the ongoing
 * crouch_switching_stay_timer expires. Either trigger re-arms both timers with a random 31-60 tick stay
 * duration (seeded from the variant's crouch_time or run_time bounds depending on berserk state).
 * Always returns the (possibly just-updated) crouch_switching_current_value once past the candidacy gate.
 *
 * DEVIATIONS ("local variable allocation has failed"; disasm-resolved, 0x8381F224-0x8381F818):
 *  - The group crouch/stand blend is rendered as a `__int64`/pointer-arithmetic mess reading 4 bytes before
 *    a local (`*(__int64*)((char*)&v26 - 4)`); disasm (0x8381F6EC-0x8381F764) shows it is plainly
 *    `chance -= 0.5*((1-chance)*standing_count - chance*crouching_count)`, using the two loop counters
 *    directly (no 64-bit value ever existed).
 *  - `v2[867] = (_cntlzw((unsigned __int8)v2[867]) & 0x20) != 0` is the standard "x == 0" idiom applied to
 *    the byte's own current value — i.e. a plain logical-NOT flip; simplified to `!crouch_switching_current_value`.
 *  - `actor_variant_definition` was previously a fully opaque forward declaration (every other reconstructed
 *    caller reads its fields at raw offsets). This session fleshed it out from `types_members`: a
 *    `movement_switching` block (crouch/run switching timing, new `actor_movement_switching_properties`) and
 *    the `ranged_combat` block (`actor_ranged_combat_properties`, already implicitly relied on by
 *    actor_combat_get_firing_variant_definition's caller decompiles) are now named; the remaining
 *    weapons/grenades/equipment payload is left an opaque, correctly-sized tail.
 *  - The action-data-union field at actor+160 is resolved in the body to the named arm
 *    `actor->state.action_data.___u0.charge.goal` (gated on state.action == actor_action_charge), checked
 *    against the charge-goal values 2/3. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_variant_definition_flags.h"
#include "headers/unit_datum.h"
#include "headers/actor_action.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/charge_goal.h"
#include "headers/blam_data_globals.h"
#include "headers/actor_movement_switching_type.h"
#include "headers/prop_perception_state.h"
#include <stdint.h>


extern uint8_t unit_is_busy(int object_index);
extern uint8_t actor_path_has_path(uint16_t actor_index);
extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);
extern uint8_t actor_has_ranged_weapon(uint16_t actor_index);
extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

uint8_t actor_type_flood_desire_shamble(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_variant_definition *variant = TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);
    actor_variant_definition *firing_variant = actor_combat_get_firing_variant_definition(actor_index);

    unsigned __int8 veto_result = 0;
    unsigned __int8 is_candidate = 0;

    if (!unit_is_busy(actor->meta.unit_index) && !actor_path_has_path(actor_index))
    {
        veto_result = 1;
    }
    else if (actor->state.mode < _actor_mode_combat)
    {
        veto_result = 1;
    }
    else if (actor->state.combat_status >= _actor_combat_status_clear_los)
    {
        unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->meta.unit_index)->datum);
        prop_datum *target_prop = actor->target.target_prop_index == -1
                ? (prop_datum *)0
                : DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index);

        if (unit_object->unit.animation.state == _unit_state_hard_ping && !actor->emotions.berserk)
        {
            veto_result = 1;
        }
        else if (!((target_prop && target_prop->distance > firing_variant->ranged_combat.maximum_firing_range)
                || (actor->emotions.berserk && target_prop
                        && target_prop->distance > firing_variant->ranged_combat.berserk_firing_range_upper_bound)))
        {
            if ((unit_object->object.damage_flags & (1u << _object_melee_attack_inhibited_bit)) != 0)
            {
                veto_result = 1;
            }
            else
            {
                /* Resolved: gated on state.action == actor_action_charge (charge), so union +4 is charge_state_data.goal. */
                __int16 charge_goal = actor->state.action_data.___u0.charge.goal;

                if (!(actor->emotions.berserk
                        || (actor->state.action == actor_action_charge && (charge_goal == charge_goal_melee || charge_goal == charge_goal_melee_leaping))
                        || !actor_has_ranged_weapon(actor_index)
                        || actor->input.underwater
                        || !variant->movement_switching.movement_type))
                {
                    if (variant->movement_switching.movement_type == _actor_movement_switching_always_crouch)
                        is_candidate = 1;
                    else if (target_prop && target_prop->distance < firing_variant->ranged_combat.combat_range_upper_bound)
                        is_candidate = 1;
                }
            }
        }
    }

    if (!is_candidate)
    {
        actor->emotions.crouch_switching_active = 0;
        return veto_result;
    }

    unsigned __int8 just_decided = 0;

    if (!actor->emotions.crouch_switching_active)
    {
        float chance = variant->movement_switching.initial_crouch_chance;

        if (actor->situation.area_friends > 0)
        {
            __int16 crouching_count = 0;
            __int16 standing_count = 0;
            prop_iterator iterator;
            prop_iterator_new(&iterator, actor_index);
            for (prop_datum *p = prop_iterator_next(&iterator); p; p = prop_iterator_next(&iterator))
            {
                if (p->state >= _prop_state_becoming_unacknowledged && p->state <= _prop_state_acknowledged && !p->enemy && !p->dead && p->actor_index != -1)
                {
                    actor_datum *other = DATA_ARRAY_ELEMENT(actor_data, actor_datum, p->actor_index);
                    if (other->meta.type == actor->meta.type && other->state.combat_status >= _actor_combat_status_clear_los)
                    {
                        if (other->emotions.defensive_crouch)
                            crouching_count++;
                        else
                            standing_count++;
                    }
                }
            }
            chance -= 0.5f * ((1.0f - chance) * (float)standing_count - chance * (float)crouching_count);
        }

        unsigned int *seed = get_global_random_seed_address();
        float roll = real_seed_random(seed);
        actor->emotions.crouch_switching_active = 1;
        actor->emotions.crouch_switching_current_value = roll < chance;
        just_decided = 1;
    }
    else
    {
        unsigned __int8 should_switch = 0;

        if (actor->emotions.crouch_switching_change_timer > 0)
        {
            actor->emotions.crouch_switching_change_timer--;
        }
        else if ((variant->flags & (1u << _actor_variant_definition_movement_switching_try_to_stay_with_friends_bit)) != 0 && actor->situation.close_friends > 0)
        {
            __int16 far_count = 0, near_same_side_count = 0, near_other_side_count = 0;
            const prop_datum *target_prop_axis = actor->target.target_prop_index == -1
                    ? (const prop_datum *)0
                    : (const prop_datum *)DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index);

            prop_iterator iterator;
            prop_iterator_new(&iterator, actor_index);
            for (prop_datum *p = prop_iterator_next(&iterator); p; p = prop_iterator_next(&iterator))
            {
                if (p->state >= _prop_state_becoming_unacknowledged && p->state <= _prop_state_acknowledged && !p->enemy && !p->dead && p->distance < 15.0f && p->actor_index != -1)
                {
                    actor_datum *other = DATA_ARRAY_ELEMENT(actor_data, actor_datum, p->actor_index);
                    if (other->emotions.crouch_switching_active)
                    {
                        float dot = target_prop_axis->actor_to_prop.n[0]
                                        * (other->input.position.body_position.n[0] - actor->input.position.body_position.n[0])
                                + target_prop_axis->actor_to_prop.n[2]
                                        * (other->input.position.body_position.n[2] - actor->input.position.body_position.n[2])
                                + target_prop_axis->actor_to_prop.n[1]
                                        * (other->input.position.body_position.n[1] - actor->input.position.body_position.n[1]);

                        if (dot <= 1.4f)
                        {
                            if (dot >= -1.4f)
                                near_same_side_count++;
                            else
                                near_other_side_count++;
                        }
                        else
                        {
                            far_count++;
                        }
                    }
                }
            }

            if (actor->emotions.crouch_switching_current_value)
            {
                if (!near_other_side_count && far_count > near_same_side_count)
                    should_switch = 1;
            }
            else if (!far_count && near_other_side_count > near_same_side_count)
            {
                should_switch = 1;
            }
        }

        if (!should_switch)
        {
            if (--actor->emotions.crouch_switching_stay_timer == 0)
                should_switch = 1;
        }

        if (should_switch)
        {
            actor->emotions.crouch_switching_current_value = (unsigned __int8)!actor->emotions.crouch_switching_current_value;
            just_decided = 1;
        }
    }

    if (just_decided)
    {
        float lower_bound, upper_bound;
        if (actor->emotions.crouch_switching_current_value)
        {
            lower_bound = variant->movement_switching.crouch_time_lower_bound;
            upper_bound = variant->movement_switching.crouch_time_upper_bound;
        }
        else
        {
            lower_bound = variant->movement_switching.run_time_lower_bound;
            upper_bound = variant->movement_switching.run_time_upper_bound;
        }

        unsigned int *seed = get_global_random_seed_address();
        float stay_ticks = real_seed_random_range(seed, lower_bound, upper_bound) * 30.0f;
        if (stay_ticks <= 31.0f)
            stay_ticks = 31.0f;

        actor->emotions.crouch_switching_change_timer = 30;
        actor->emotions.crouch_switching_stay_timer = (__int16)(int)stay_ticks;
    }

    return actor->emotions.crouch_switching_current_value;
}