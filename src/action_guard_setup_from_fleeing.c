/* action_guard_setup_from_fleeing @0x83823A00 — build an actor's "guard" action state as the follow-on to a
 * completed flee. Clears the guard payload, then if the actor is in a special post-combat/guarding condition
 * (actor->input.vehicle_passenger or actor->meta.swarm) it just marks the guard-location type as "current
 * position". Otherwise it sets up a cower: chooses whether the cower is a panic cower (from the flee panic
 * type) or a retreat cower (actor->emotions.unopposable_retreat_timer gate), and when it is neither, rolls a
 * cower duration from the actor variant's cower-time bounds (retreat pair v6[166..167] vs normal pair
 * v6[180..181], seconds -> ticks x30). Finally it converts the flee's stored firing position and approach
 * point into a guard location and aim direction. Always returns 1. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/flee_state_data.h"
#include "headers/guard_state_data.h"
#include "headers/actor_panic_type.h"
#include "headers/actor_guard_location_type.h"
#include "headers/actor_definition.h"
#include "headers/blam_data_globals.h"


extern float normalize3d(real_vector3d *v);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

int action_guard_setup_from_fleeing(int actor_index, flee_state_data *flee_state_data, guard_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *definition = TAG_GET(actor_definition, actor->meta.definition_index);

    memset(state_data, 0, sizeof(guard_state_data));

    if ( actor->input.vehicle_passenger || actor->meta.swarm )
    {
        state_data->guard_location_type = _actor_guard_location_current;
    }
    else
    {
        state_data->wait_ticks = 0;
        state_data->cower = 1;
        state_data->cower_ticks = 0;

        /* sign bit set iff panic_type > _actor_panic_none */
        int panic_positive = -flee_state_data->panic_type & ~flee_state_data->panic_type;
        state_data->cower_panicked = panic_positive < 0;

        uint8_t cower_from_retreat = 1;
        if ( actor->emotions.unopposable_retreat_timer <= 0 || panic_positive < 0 )
            cower_from_retreat = 0;
        state_data->cower_from_retreat = cower_from_retreat;

        if ( !cower_from_retreat )
        {
            float cower_time_min;
            float cower_time_max;
            if ( panic_positive >= 0 )
            {
                /* normal cower: actor_definition.defensive.hide_time_* (abs +720/+724 = index [180]/[181]) */
                cower_time_max = definition->defensive.hide_time_upper_bound;
                cower_time_min = definition->defensive.hide_time_lower_bound;
            }
            else
            {
                /* panic cower: actor_definition.panic.cower_time_* (abs +664/+668 = index [166]/[167]) */
                cower_time_max = definition->panic.cower_time_upper_bound;
                cower_time_min = definition->panic.cower_time_lower_bound;
            }
            unsigned int *seed = get_global_random_seed_address();
            state_data->cower_ticks =
                (int)(real_seed_random_range(seed, cower_time_min, cower_time_max) * 30.0f);
        }

        if ( (uint16_t)flee_state_data->flee_firing_position_index == 0xFFFF )
        {
            state_data->find_new_guard_position = 1;
            state_data->guard_location_type = _actor_guard_location_none;
        }
        else
        {
            state_data->find_new_guard_position = 0;
            state_data->guard_location_type = _actor_guard_location_firing_position;
            state_data->___u17.guard_firing_position_index = flee_state_data->flee_firing_position_index;
            if ( flee_state_data->has_approach_point )
            {
                state_data->aim_in_guard_direction = 1;
                state_data->has_guard_direction = 1;
                state_data->guard_direction.n[0] = flee_state_data->approach_point.n[0] - actor->input.position.body_position.n[0];
                state_data->guard_direction.n[1] = flee_state_data->approach_point.n[1] - actor->input.position.body_position.n[1];
                state_data->guard_direction.n[2] = flee_state_data->approach_point.n[2] - actor->input.position.body_position.n[2];
                if ( normalize3d(&state_data->guard_direction) == 0.0f )
                    state_data->has_guard_direction = 0;
            }
        }
    }

    state_data->guard_look_prop_index = -1;
    return 1;
}
