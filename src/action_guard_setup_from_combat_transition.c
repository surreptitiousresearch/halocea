/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* action_guard_setup_from_combat_transition @0x83823598 — build a fresh guard_state_data for an actor
 * entering the guard action from a combat transition. Zeroes the state, seeds wait_ticks from the actor's
 * opaque behaviour payload. If the actor is a swarm-type or currently a vehicle passenger, forces
 * guard_location_type=1 (plain in-place guard, no positioning). Otherwise tries to guard at a nearby firing
 * position (if the actor has one recorded and actor_nearby_firing_positions confirms it), falling back to
 * either "find a new guard position" (if no wait time remains) or holding the current position with an
 * optional guard-facing direction (validated by normalize3d). If the transition reason was specifically
 * "dead friend" (opaque field == 2, distinct from the general pending-transition flag already established
 * in actor_action_handle_combat_transition.c), flags shout_about_dead_friend. Always sets up the
 * guard-look prop (if any) and its look_ticks/until-reached-point. Always returns 1.
 *
 * All actor payload accesses resolve to named actor_datum members (input.vehicle_passenger@352,
 * meta.swarm@6, and the stimuli.combat_transition_* block @786-840); no raw offset accesses remain in
 * the body. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/guard_state_data.h"
#include "headers/real_point3d.h"
#include "headers/actor_guard_location_type.h"
#include "headers/actor_combat_transition_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern void *memset(void *destination, int value, unsigned int size);
extern uint8_t actor_nearby_firing_positions(int actor_index, real_point3d *test_point, int test_surface_index, int16_t group_selection_mode);
extern float normalize3d(real_vector3d *v);

uint8_t action_guard_setup_from_combat_transition(int actor_index, guard_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    memset(state_data, 0, sizeof(guard_state_data));
    state_data->wait_ticks = actor->stimuli.combat_transition_guard_timer;

    if ( actor->input.vehicle_passenger || actor->meta.swarm )
    {
        state_data->guard_location_type = _actor_guard_location_current;
    }
    else
    {
        uint8_t have_firing_position = 0;

        if ( actor->stimuli.combat_transition_guard_at_point && actor_nearby_firing_positions(actor_index,
                &actor->stimuli.combat_transition_guard_point,
                actor->stimuli.combat_transition_guard_point_surface_index, 1) )
        {
            have_firing_position = 1;
            state_data->guard_location_type = _actor_guard_location_point;
            state_data->___u17.guard_point.position.n[0] = actor->stimuli.combat_transition_guard_point.n[0];
            state_data->___u17.guard_point.position.n[1] = actor->stimuli.combat_transition_guard_point.n[1];
            state_data->___u17.guard_point.position.n[2] = actor->stimuli.combat_transition_guard_point.n[2];
            state_data->___u17.guard_point.surface_index = actor->stimuli.combat_transition_guard_point_surface_index;
            state_data->___u17.guard_point.radius = actor->stimuli.combat_transition_guard_point_distance;
        }

        if ( !have_firing_position )
        {
            if ( state_data->wait_ticks <= 0 )
            {
                state_data->guard_location_type = _actor_guard_location_none;
                state_data->find_new_guard_position = 1;
            }
            else
            {
                state_data->guard_location_type = _actor_guard_location_current;
                state_data->has_guard_direction = actor->stimuli.combat_transition_has_vector;
                state_data->aim_in_guard_direction = 0;

                if ( actor->stimuli.combat_transition_has_vector )
                {
                    state_data->guard_direction.n[0] = actor->stimuli.combat_transition_vector.n[0];
                    state_data->guard_direction.n[1] = actor->stimuli.combat_transition_vector.n[1];
                    state_data->guard_direction.n[2] = actor->stimuli.combat_transition_vector.n[2];
                    if ( normalize3d(&state_data->guard_direction) == 0.0f )
                        state_data->has_guard_direction = 0;
                }
            }
        }
    }

    if ( actor->stimuli.combat_transition == _actor_stimulus_combat_body )
    {
        state_data->shout_about_dead_friend = 1;
        state_data->shout_dead_friend_prop_index = actor->stimuli.combat_transition_prop_index;
    }

    int guard_look_prop_index = actor->stimuli.combat_transition_prop_index;
    state_data->guard_look_prop_index = guard_look_prop_index;

    if ( guard_look_prop_index != -1 )
    {
        state_data->look_ticks = actor->stimuli.combat_transition_prop_look_timer;
        state_data->guard_look_until_reached_point = actor->stimuli.combat_transition_prop_look_while_moving;
    }

    return 1;
}
