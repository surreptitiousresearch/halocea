/* action_charge_setup @0x837F9578 — set up (or validate the feasibility of) an actor's "charge" melee action
 * for a given goal, initializing the charge_state_data block. It always zeroes the state, stamps the start time,
 * and records the goal. Behavior by goal:
 *   4 / 5 : return whether input.vehicle_driver_type (@+350) is more than one (the branchless (>=0)+(<=1)
 *           form) — goals 4/5 are the vehicle-ram charges (see actor_action_handle_combat_selection).
 *   2     : the melee-charge proper. Aborts (returns 0) if the actor is a swarm; otherwise, if the
 *           target unit is not un-meleeable and a pursuit prop is set, decides whether a leap is possible
 *           (character leap ranges = definition->berserk.melee_leap_range_upper_bound/melee_leap_chance/
 *           melee_leap_range_lower_bound, prop leap-state = prop->flying/unreachable_ticks/distance,
 *           a random roll), sets
 *           goal 3 when leaping, fetches the unit's melee range/ticks, derives the danger range and
 *           ticks-until-dangerous (suicide units use the end tick with zero danger range), computes an acceptable
 *           target range, moves the actor to the prop at max(range, leap?4:1.5), and validates the melee
 *           destination — returning 1 only if that destination is valid.
 *   0     : if the character allows a "charge from cover" (flag 0x20000), combat_status >= _actor_combat_status_clear_los, and not
 *           berserk, start a plain charge (goal 1) and return 1.
 * Otherwise returns the goal-4/5 counter result (default 1).
 *
 * DEVIATION: the `((x>=0)+((unsigned)x<=1))&1` form is the established branchless "x > 1" test (for x>=0),
 * reproduced verbatim. `(v18>>1)+(v18<0 && v18&1)` is signed divide-by-2 toward zero = v18/2. The
 * valid-melee goal arg is prop->center_of_mass (disasm 0x837F9814: addi r4,r27,0xC8); its
 * melee_target_point output is a discarded local. All actor payload fields are read through named
 * actor_datum members (see the @+offset annotations on each access below). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/charge_state_data.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_datum.h"
#include "headers/prop_datum.h"
#include "headers/real_point3d.h"
#include <string.h>
#include "headers/charge_goal.h"
#include "headers/object_damage_flags.h"
#include "headers/actor_combat_status.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern uint8_t unit_get_melee_range_and_ticks(int unit_index, uint8_t airborne, int16_t *start_tick_count, float *start_range, int16_t *end_tick_count, float *end_range);
extern float action_charge_find_target_range(int actor_index, int16_t goal, charge_state_data *state_data);
extern uint8_t actor_move_to_prop(int actor_index, unsigned int prop_index, float accept_radius);
extern void actor_move_keep_moving_past_destination(int actor_index);
extern uint8_t action_charge_valid_melee_destination(int actor_index, real_point3d *goal, real_point3d *melee_target_point);

uint8_t action_charge_setup(int actor_index, int16_t goal, charge_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int result = 1;
    actor_definition *definition = TAG_GET(actor_definition, actor->meta.definition_index);   /* meta.definition_index @+88 */

    memset(state_data, 0, sizeof(charge_state_data));
    state_data->charge_start_time = game_time_get();

    if ( goal == charge_goal_vehicle_ramming || goal == charge_goal_vehicle_strafing )
    {
        /* input.vehicle_driver_type @+350 (0x15E). Branchless "> 1" for a non-negative value. */
        int16_t counter = actor->input.vehicle_driver_type;
        result = ((counter >= 0) + ((unsigned int)counter <= 1)) & 1;
    }
    else if ( goal == charge_goal_melee )
    {
        if ( actor->meta.swarm )
        {
            state_data->goal = goal;
            return 0;
        }

        result = 0;
        int unit_index = actor->meta.unit_index;   /* meta.unit_index @+24 */
        int pursuit_prop_index = actor->target.target_prop_index;   /* actor->target.target_prop_index @+624 */

        /* target unit not flagged un-meleeable (object_datum.object.damage_flags bit 0x80) and a pursuit prop is set */
        object_datum *target_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        if ( (target_object->object.damage_flags & (1u << _object_melee_attack_inhibited_bit)) == 0
          && pursuit_prop_index != -1 )
        {
            prop_datum *prop = DATUM_GET(prop_data, prop_datum, pursuit_prop_index);

            uint8_t leap;
            if ( definition->berserk.melee_leap_range_upper_bound == 0.0f
              || definition->berserk.melee_leap_chance == 0.0f )
            {
                state_data->leap_possible_if_at_range = 0;
                leap = 0;
            }
            else if ( prop->flying || prop->unreachable_ticks > 0 )
            {
                leap = 1;
                state_data->leap_possible_if_at_range = 1;
            }
            else
            {
                unsigned int *seed = get_global_random_seed_address();
                leap = real_seed_random(seed) < definition->berserk.melee_leap_chance;
                state_data->leap_possible_if_at_range = leap;
                if ( !(prop->distance >= definition->berserk.melee_leap_range_lower_bound) )
                    leap = 0;
            }

            if ( leap )
                goal = charge_goal_melee_leaping;

            int16_t start_tick;
            float start_range;
            int16_t end_tick;
            float end_range[2];
            if ( unit_get_melee_range_and_ticks(unit_index, leap, &start_tick, &start_range, &end_tick, end_range) )
            {
                int16_t ticks_until_dangerous;
                if ( (definition->flags & (1u << _actor_definition_suicidal_melee_attack_bit)) != 0 )
                {
                    ticks_until_dangerous = end_tick;
                    state_data->melee_danger_range = 0.0f;
                    state_data->melee_suicide = 1;
                }
                else
                {
                    ticks_until_dangerous = start_tick;
                    if ( !start_tick )
                    {
                        start_range = end_range[0] * 0.5f;
                        ticks_until_dangerous = end_tick / 2;   /* (v>>1)+(v<0&&v&1) = toward-zero /2 */
                        start_tick = ticks_until_dangerous;
                    }
                    state_data->melee_danger_range = end_range[0] - start_range;
                }
                state_data->melee_ticks_until_dangerous = ticks_until_dangerous;

                uint8_t valid = 0;
                float target_range = action_charge_find_target_range(actor_index, goal, state_data);
                state_data->acceptable_target_range = target_range;

                float move_range = (goal == charge_goal_melee_leaping) ? 4.0f : 1.5f;
                if ( move_range <= target_range )
                    move_range = target_range;

                if ( actor_move_to_prop(actor_index, pursuit_prop_index, move_range) )
                {
                    actor_move_keep_moving_past_destination(actor_index);
                    real_point3d melee_target_point;
                    valid = action_charge_valid_melee_destination(actor_index,
                            &prop->center_of_mass, &melee_target_point);
                }

                if ( valid )
                {
                    state_data->goal = goal;
                    return 1;
                }
            }
        }
    }
    else if ( !goal
           && (definition->flags & (1u << _actor_definition_stalking_behavior_bit)) != 0
           && actor->state.combat_status >= _actor_combat_status_clear_los          /* state.combat_status @+110 */
           && !actor->emotions.berserk )               /* emotions.berserk @+888 */
    {
        state_data->goal = charge_goal_stalking;
        return 1;
    }

    state_data->goal = goal;
    return result;
}