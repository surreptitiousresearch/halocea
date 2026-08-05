/* action_fight_perform @0x83825FB8 — per-tick "fight" behaviour: maintains the actor's firing position while
 * engaging. If the actor is allowed to relocate (flags gate + reachable) it may re-select a firing position
 * via actor_active_select_firing_position / actor_change_firing_position, and when it actually changes
 * position it seeds a random settle timer (actor->state.action_data[0] as int16) from the unit definition's
 * range (def +960..964), clamped by the target object's own settle cap (object def +936). Finally, when
 * actor->target.target_type has advanced far enough, it reports whether the current firing position is
 * unreachable relative to the target prop (actor_perception_unreachable). Always returns 0.
 *
 * DEVIATION: the RtlCheckStack12 call and the back_chain self-store the decompiler emitted inside the
 * reselect block are the large-frame stack probe leaking into the body
 * (firing_position_evaluation_context is a big local) — dropped, they are not program logic. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/firing_position_definition.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/actor_threat_type.h"
#include "headers/vehicle_definition.h"
#include "headers/prop_datum.h"
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/path_state.h"
#include "headers/real_point3d.h"
#include "headers/actor_target_type.h"
#include "headers/firing_point_evaluation_mode.h"
#include "headers/blam_data_globals.h"

extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);
extern void actor_find_pathfinding_location(uint16_t actor_index);
extern uint8_t actor_nearby_firing_positions(int actor_index, real_point3d *test_point, int test_surface_index, int16_t group_selection_mode);
extern float actor_destination_tolerance(uint16_t actor_index);
extern uint8_t actor_move_halt(uint16_t actor_index);
extern int16_t actor_active_select_firing_position(int actor_index, firing_position_evaluation_context *context,
        firing_position *out_position, int *out_current_owner, path_state *path, uint8_t *area_path_state_valid);
extern int16_t actor_change_firing_position(int actor_index, int16_t firing_position_index, firing_position *firing_position, int previous_owner, path_state *cached_path_state, uint8_t cached_path_available);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern uint8_t actor_has_ranged_weapon(uint16_t actor_index);
extern void actor_perception_unreachable(int actor_index, int prop_index, uint8_t unreachable);
extern void *memset(void *dest, int value, unsigned int count);


uint8_t action_fight_perform(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( !actor->meta.timeslice )
        return 0;

    actor_definition *unit_definition = TAG_GET(actor_definition, actor->meta.definition_index);
    actor_variant_definition *firing_variant_definition =
            actor_combat_get_firing_variant_definition(actor_index);

    char do_reselect = 1;
    if ( !actor->input.vehicle_passenger )
    {
        if ( !actor->emotions.defensive_crouch )
            goto reselect;
        if ( (unit_definition->flags & (1u << _actor_definition_crouch_try_not_to_move_bit)) == 0 )
            goto reselect;
        actor_find_pathfinding_location(actor_index);
        if ( !actor_nearby_firing_positions(actor_index, &actor->input.pathfinding_point, actor->input.pathfinding_surface_index, 0) )
            goto reselect;

        uint8_t firing_position_ok = 0;
        if ( actor->meta.encounter_index != -1 )
        {
            int16_t firing_position_index = actor->firing_positions.current_position_index;
            if ( firing_position_index != -1 )
            {
                float *firing_position = ((firing_position_definition *)((encounter_definition *)
                                global_scenario->ai_encounters.address)
                                [(uint16_t)actor->meta.encounter_index].firing_positions.address)
                        [firing_position_index].position.n;
                float tolerance = actor_destination_tolerance(actor_index);
                float dy = (firing_position[1] - actor->input.position.body_position.y);
                float dz = (firing_position[2] - actor->input.position.body_position.z);
                float dx = (*firing_position - actor->input.position.body_position.x);
                firing_position_ok = ((dx * dx) + ((dz * dz) + (dy * dy)))
                        < (double)(tolerance * tolerance);
            }
        }
        if ( actor->control.moving && !actor->situation.cumulative_threats[actor_threat_shooting_near_me] )
        {
            firing_position_ok = 1;
            if ( actor->target.target_prop_index != -1 )
                /* recovered: *(float *)((char *)variant + 0xA0) -> ranged_combat.combat_range_upper_bound */
                firing_position_ok = *((float *)prop_data->data
                                + 78 * (uint16_t)actor->target.target_prop_index + 71)
                        >= (double)firing_variant_definition->ranged_combat.combat_range_upper_bound;
        }
        if ( !firing_position_ok )
        {
            actor->firing_positions.current_position_index = -1;
            actor_move_halt(actor_index);
        }
    }
    do_reselect = 0;

reselect:
    if ( do_reselect )
    {
        int16_t previous_position = actor->firing_positions.current_position_index;
        firing_position_evaluation_context context;
        firing_position selected;
        int selected_a4;
        uint8_t path_bytes[8];

        memset(&context, 0, sizeof(context));
        context.evaluation_mode = _firing_point_evaluation_mode_fight;
        int16_t selection = actor_active_select_firing_position(actor_index, &context, &selected, &selected_a4,
                (path_state *)path_bytes, path_bytes);
        int16_t new_position = actor_change_firing_position(actor_index, selection, &selected, selected_a4,
                (path_state *)path_bytes, path_bytes[0]);
        if ( new_position == -1 )
        {
            actor->state.action_data.___u0.fight.firing_position_timer = 0;
        }
        else if ( new_position != previous_position )
        {
            float settle_max = unit_definition->firing_position.combat_position_time_upper_bound;
            float settle_min = unit_definition->firing_position.combat_position_time_lower_bound;
            unsigned int *seed = get_global_random_seed_address();
            float settle = real_seed_random_range(seed, settle_min, settle_max);
            if ( actor->input.vehicle_driver_type > 0 )
            {
                object_datum *vehicle_object = DATA_ARRAY_ELEMENT(object_header_data,
                        object_header_datum, actor->input.vehicle_index)->datum;
                vehicle_definition *target_definition =
                        TAG_GET(vehicle_definition, vehicle_object->definition_index);
                /* +936 resolved to DB vehicle_definition.vehicle.ai_movement_max_time */
                float settle_cap = target_definition->vehicle.ai_movement_max_time;
                if ( settle_cap > 0.0 && settle > settle_cap )
                    settle = target_definition->vehicle.ai_movement_max_time;
            }
            actor->state.action_data.___u0.fight.firing_position_timer = (int)(settle * 30.0f);
        }
    }

    if ( actor->target.target_type >= actor_target_acknowledged_enemy )
    {
        uint8_t unreachable = 1;
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index);
        if ( actor_has_ranged_weapon(actor_index) )
        {
            if ( prop->distance < (double)actor->control.weapon_maximum_range )
                goto reachable;
            if ( actor->meta.encounter_index != -1 )
            {
                int16_t firing_position_index = actor->firing_positions.current_position_index;
                if ( firing_position_index != -1 )
                {
                    float *firing_position = ((firing_position_definition *)((encounter_definition *)
                                    global_scenario->ai_encounters.address)
                                    [(uint16_t)actor->meta.encounter_index].firing_positions.address)
                            [firing_position_index].position.n;
                    float tolerance = actor_destination_tolerance(actor_index);
                    float dy = (firing_position[1] - actor->input.position.body_position.y);
                    float dz = (firing_position[2] - actor->input.position.body_position.z);
                    float dx = (*firing_position - actor->input.position.body_position.x);
                    if ( ((dx * dx) + ((dz * dz) + (dy * dy)))
                            > (double)(tolerance * tolerance) )
                    {
                        float pdy = (prop->body_position.n[1] - firing_position[1]);
                        float pdz = (prop->body_position.n[2] - firing_position[2]);
                        float pdx = (prop->body_position.n[0] - *firing_position);
                        if ( ((pdx * pdx) + ((pdz * pdz) + (pdy * pdy)))
                                < (double)(actor->control.weapon_maximum_range * actor->control.weapon_maximum_range) )
reachable:
                            unreachable = 0;
                    }
                }
            }
        }
        actor_perception_unreachable(actor_index, actor->target.target_prop_index, unreachable);
    }

    return 0;
}