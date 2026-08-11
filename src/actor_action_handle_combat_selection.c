/* actor_action_handle_combat_selection @0x837F5518 — per-tick combat action selector: decides whether the
 * actor should charge (melee rush, action 10), vehicle-ram, keep its current charge, or fall back to the
 * generic fight action (action 3). In order:
 *   - stalking discovery (already charging with goal 1 = stalk): if the target prop is damaging the actor,
 *     shooting at point-blank quantized distance, or the stalk has lasted past the character's
 *     stalking_discovery_time, seek cover when the target is beyond combat range, else go berserk;
 *   - on-foot charge: when the actor has no ranged weapon (or the target is on foot and not a swarm), is not
 *     itself a swarm/vehicle actor, and is inside its (berserk_)melee_range, start a goal-2 charge — throttled
 *     by a 10-tick retry stamp (actor+0x37C) and a difficulty-scaled charge delay
 *     (definition->berserk.melee_attack_delay_timer) against the last charge time (actor+0x380);
 *     external_orders.disable_charging still allows point-blank charges within
 *     max(0, definition->berserk.melee_attack_range) + 0.8 wu (the fsel);
 *   - vehicle ram: a vehicle driver (input.vehicle_driver_type) repeats a goal-4 charge when the vehicle's
 *     ai_charge_repeat_time has elapsed since actor+0x388, the target is beyond melee range, and the prop
 *     has no line of sight;
 *   - fallback: re-derive the desire to charge (opaque byte actor+0x375, "must charge" definition flag
 *     bit 24, or a live goal-2/3 charge with no finished/aborted melee and able to advance; a goal-4 ram is
 *     re-validated against ai_strafing_stop_range and target alignment), start a goal-0 charge, else swap
 *     to the fight action unless already fighting.
 *
 * DEVIATION: the two __SPAIR64__/(float)vNN timing-gate garbles are the PPC extsw+std/lfd+fcfid+frsp
 * int-to-float idiom (disasm 0x837F57E8, 0x837F58F8): `(float)(terms*30 + (float)last) < (float)now`.
 * The `fsel f12,f0,f0,f29` @0x837F57C0 has f29 = 0.0f loaded in the prologue (0x837F55C4) = max(0, x).
 * The vehicle-definition floats are vehicle.ai_charge_repeat_time and ai_strafing_stop_range
 * (`vehicle_definition.h`, byte 0x390/0x394 of the full tag). The bare actor_has_ranged_weapon call
 * before begin_charge (0x837F5828) discards its result — reproduced (shipped). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/actor_variant_definition.h"
#include "headers/vehicle_definition.h"
#include "headers/prop_datum.h"
#include "headers/charge_state_data.h"
#include "headers/fight_state_data.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_action.h"
#include "headers/actor_fire_state.h"
#include "headers/charge_goal.h"
#include "headers/destination_type.h"
#include "headers/actor_vehicle_driver_type.h"
#include "headers/game_difficulty_value.h"
#include "headers/blam_data_globals.h"


#include "headers/fight_state_data.h"
extern int game_time_get(void);
extern float game_difficulty_get_value(int16_t value_type);
extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);
extern uint8_t actor_has_ranged_weapon(int actor_index);
extern uint8_t actor_action_try_to_seek_cover(int actor_index, uint8_t cover_from_last_visible_location, uint8_t allow_occluded_points);
extern void actor_berserk(int actor_index, uint8_t berserk);
extern uint8_t action_charge_setup(int actor_index, int16_t goal, charge_state_data *state_data);
extern uint8_t action_fight_setup(int actor_index, fight_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);

uint8_t actor_action_handle_combat_selection(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *definition =
        TAG_GET(actor_definition, actor->meta.definition_index);
    actor_variant_definition *variant =
        TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);
    actor_variant_definition *firing_variant = actor_combat_get_firing_variant_definition(actor_index);
    charge_state_data *charge = &actor->state.action_data.___u0.charge;

    uint8_t action_changed = 0;
    prop_datum *prop = 0;
    float prop_distance = 3.4028235e38f; /* FLT_MAX when the actor has no target prop */
    action_state_data new_action_data;

    int current_prop_index = actor->target.target_prop_index; /* recovered: raw actor+0x270 -> target.target_prop_index */
    if ( current_prop_index != -1 )
    {
        prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, current_prop_index);
        prop_distance = prop->distance;

        /* ---- stalking discovery: charging with goal 1 (stalk) ---- */
        if ( actor->state.action == actor_action_charge && charge->goal == charge_goal_stalking )
        {
            uint8_t discovered =
                prop->currently_damaging_me
                || (prop->shooting && prop->quantized_distance <= 1)
                || (definition->defensive.stalking_discovery_time > 0.0f
                    && charge->stalking_discovery_timer
                       >= (int16_t)(int)(definition->defensive.stalking_discovery_time * 30.0f));
            if ( discovered )
            {
                if ( prop_distance > firing_variant->ranged_combat.combat_range_upper_bound )
                    action_changed = actor_action_try_to_seek_cover(actor_index, 0, 0);
                if ( !action_changed )
                    actor_berserk(actor_index, 1);
            }
        }

        /* ---- on-foot charge consideration ---- */
        if ( !actor_has_ranged_weapon(actor_index)
            || (prop->vehicle_index == -1 && !prop->swarm) )
        {
            if ( actor->state.action != actor_action_charge || (charge->goal != charge_goal_melee && charge->goal != charge_goal_melee_leaping) )
            {
                if ( !action_changed && !actor->meta.swarm && actor->input.vehicle_index == -1
                    && actor->control.fire_state != actor_fire_state_bursting ) /* recovered: *(__int16*)(actor+0x5F2) -> control.fire_state */
                {
                    int current_time = game_time_get();
                    uint8_t berserking = actor->emotions.berserk; /* recovered: raw actor+0x378 -> emotions.berserk */
                    uint8_t use_berserk_range = berserking;
                    if ( !actor_has_ranged_weapon(actor_index) && (definition->flags & (1u << _actor_definition_stalking_behavior_bit)) == 0 )
                        use_berserk_range = 1;
                    float charge_delay = berserking ? 0.0f
                                                    : definition->berserk.melee_attack_delay_timer;
                    float delay_scaled = game_difficulty_get_value(_game_difficulty_melee_delay_scale) * charge_delay;
                    float delay_base = game_difficulty_get_value(_game_difficulty_melee_delay_bonus);
                    float charge_range = use_berserk_range ? variant->ranged_combat.berserk_melee_range
                                                           : variant->ranged_combat.melee_range;

                    uint8_t begin_charge = 0;
                    int last_attempt_time = actor->emotions.last_melee_check_time; /* recovered: raw actor+0x37C -> emotions.last_melee_check_time */
                    if ( last_attempt_time == -1 || last_attempt_time + 10 < current_time )
                    {
                        if ( prop_distance <= charge_range )
                        {
                            uint8_t in_range = 1;
                            if ( actor->external_orders.disable_charging )
                            {
                                /* charging disabled still allows point-blank charges (fsel = max(0, x)) */
                                float point_blank_range = definition->berserk.melee_attack_range;
                                if ( point_blank_range < 0.0f )
                                    point_blank_range = 0.0f;
                                if ( prop_distance > point_blank_range + 0.8f )
                                    in_range = 0;
                            }
                            if ( in_range )
                            {
                                int last_charge_time = actor->emotions.last_melee_attack_time; /* recovered: raw actor+0x380 -> emotions.last_melee_attack_time */
                                if ( last_charge_time == -1
                                    || (delay_base + delay_scaled) * 30.0f + (float)last_charge_time
                                       < (float)current_time )
                                {
                                    actor_has_ranged_weapon(actor_index); /* result discarded (shipped) */
                                    begin_charge = 1;
                                }
                            }
                        }
                    }
                    if ( begin_charge )
                    {
                        actor->emotions.last_melee_check_time = current_time; /* recovered: raw actor+0x37C */
                        if ( action_charge_setup(actor_index, charge_goal_melee, &new_action_data.___u0.charge) )
                        {
                            actor_action_change(actor_index, actor_action_charge, &new_action_data);
                            action_changed = 1;
                        }
                    }
                }
            }
        }

        /* ---- vehicle ram (goal 4) ---- */
        if ( actor->state.action != actor_action_charge && !actor->external_orders.disable_charging && !action_changed
            && actor->input.vehicle_driver_type > _actor_vehicle_driver_none )
        {
            int last_ram_time = actor->emotions.last_vehicle_charge_time; /* recovered: raw actor+0x388 -> emotions.last_vehicle_charge_time */
            uint8_t repeat_elapsed = 0;
            if ( last_ram_time == -1 )
            {
                repeat_elapsed = 1;
            }
            else
            {
                char *vehicle_object = ((char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->input.vehicle_index)->datum);
                vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, *(int *)vehicle_object);
                if ( vehicle_def->vehicle.ai_charge_repeat_time * 30.0f + (float)last_ram_time
                     < (float)game_time_get() )
                    repeat_elapsed = 1;
            }
            if ( repeat_elapsed )
            {
                uint8_t begin_ram = 0;
                if ( actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying )
                {
                    if ( prop_distance > firing_variant->ranged_combat.melee_range && !prop->line_of_sight )
                        begin_ram = 1;
                }
                if ( begin_ram && action_charge_setup(actor_index, charge_goal_vehicle_strafing, &new_action_data.___u0.charge) )
                {
                    actor_action_change(actor_index, actor_action_charge, &new_action_data);
                    action_changed = 1;
                }
            }
        }
    }

    if ( action_changed )
        return action_changed;

    /* ---- fallback: re-derive the desire to charge, else fight ---- */
    uint8_t desires_charge = actor->emotions.forced_to_charge /* recovered: raw actor+0x375 -> emotions.forced_to_charge */
                                     && !actor->external_orders.disable_charging;
    uint8_t restart_charge = 0;

    if ( !actor->external_orders.disable_charging && !actor_has_ranged_weapon(actor_index)
        && (definition->flags & (1u << _actor_definition_charge_in_attacking_mode_bit)) != 0 )
        desires_charge = 1;

    int16_t action = actor->state.action;
    if ( action == actor_action_charge )
    {
        int16_t goal = charge->goal;
        if ( goal == charge_goal_melee || goal == charge_goal_melee_leaping )
        {
            if ( !charge->finished_melee_attack && !charge->aborted_melee_attack
                && !charge->unable_to_advance )
            {
                desires_charge = 1;
                goto evaluate;
            }
            restart_charge = 1;
            goto evaluate;
        }
        if ( actor->external_orders.disable_charging )
        {
            desires_charge = 0;
            restart_charge = 1;
            goto evaluate;
        }
        if ( goal == charge_goal_vehicle_strafing || goal == charge_goal_vehicle_ramming )
        {
            if ( charge->unable_to_advance || actor->input.vehicle_driver_type <= _actor_vehicle_driver_unknown )
            {
                restart_charge = 1;
                goto evaluate;
            }
            desires_charge = 1;
            if ( goal == charge_goal_vehicle_strafing )
            {
                /* recovered: byte +0x484 -> control.path.at_destination, word +0x46C -> destination_type,
                 * dword +0x470 -> destination_orders prop.prop_index — a destination already targeting the
                 * current prop keeps the existing charge */
                if ( actor->control.path.at_destination
                    && actor->control.path.destination_orders.destination_type == _destination_prop
                    && actor->control.path.destination_orders.___u3.prop.prop_index == current_prop_index )
                {
                    desires_charge = 0;
                }
                else
                {
                    char *vehicle_object = ((char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->input.vehicle_index)->datum);
                    vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, *(int *)vehicle_object);
                    float stop_range = vehicle_def->vehicle.ai_strafing_stop_range;
                    if ( prop_distance >= stop_range )
                    {
                        if ( prop_distance < stop_range * 2.0f
                            && actor->input.facing_vector.i * prop->actor_to_prop.i
                             + (actor->input.facing_vector.k * prop->actor_to_prop.k
                              + actor->input.facing_vector.j * prop->actor_to_prop.j) < 0.5f )
                        {
                            desires_charge = 0;
                        }
                    }
                    else
                    {
                        desires_charge = 0;
                    }
                }
            }
        }
    }

evaluate:
    if ( desires_charge && (restart_charge || action != actor_action_charge) )
    {
        if ( action_charge_setup(actor_index, charge_goal_close_range, &new_action_data.___u0.charge) )
        {
            actor_action_change(actor_index, actor_action_charge, &new_action_data);
            action_changed = 1;
        }
        else
        {
            desires_charge = 0;
        }
    }
    if ( !desires_charge && !action_changed && actor->state.action != actor_action_fight )
    {
        action_fight_setup(actor_index, &new_action_data.___u0.fight);
        actor_action_change(actor_index, actor_action_fight, &new_action_data);
        return 1;
    }
    return action_changed;
}
