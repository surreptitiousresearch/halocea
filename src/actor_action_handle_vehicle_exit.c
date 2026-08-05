/* actor_action_handle_vehicle_exit @0x837F2428 — decide whether an actor should bail out of its
 * current vehicle this tick. No-op if the actor isn't in a vehicle. Wants to exit if: an enemy player is
 * spotted (via a prop) actively targeting/firing on the same vehicle (perception state 2-3), the
 * stimuli.vehicle_eviction flag is set, or the actor is a vehicle passenger with either a pending
 * delayed attached projectile or a projectile-type danger attached to us
 * (danger_zone.danger_type == projectile && danger_zone.attached_to_us). The enemy-prop and threat
 * cases both additionally mark the exit as "forced". On a successful exit, remembers which vehicle
 * was left and starts a 180-tick re-entry cooldown. Always clears stimuli.vehicle_eviction.
 * Returns whether the actor actually exited. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/prop_iterator.h"
#include "headers/prop_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_danger_zone_type.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern uint8_t unit_try_and_exit_seat(int object_index, uint8_t should_allow_clients);

int actor_action_handle_vehicle_exit(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int exited = 0;

    if ( actor->input.vehicle_index == -1 )
        return exited;

    uint8_t want_exit = 0;
    uint8_t forced_exit = 0;

    prop_iterator iterator;
    prop_iterator_new(&iterator, actor_index);
    for ( prop_datum *prop = prop_iterator_next(&iterator); prop; prop = prop_iterator_next(&iterator) )
    {
        if ( prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged && prop->player && prop->enemy
          && prop->vehicle_index == actor->input.vehicle_index )
        {
            want_exit = 1;
            forced_exit = 1;
            break;
        }
    }

    /* pending forced-exit flag: stimuli.vehicle_eviction (actor+749). */
    if ( actor->stimuli.vehicle_eviction )
        want_exit = 1;

    if ( actor->input.vehicle_passenger
      && (actor->input.delayed_attached_projectile_index != -1
       || (actor->danger_zone.danger_type == actor_danger_zone_projectile && actor->danger_zone.attached_to_us)) )
    {
        want_exit = 1;
        forced_exit = 1;
    }

    if ( want_exit )
    {
        actor->emotions.last_vehicle_exit_forced = forced_exit;

        if ( unit_try_and_exit_seat(actor->meta.unit_index, 0) )
        {
            actor->emotions.vehicle_ignore_index = actor->input.vehicle_index;
            exited = 1;
            actor->emotions.vehicle_ignore_time = game_time_get() + 180;
        }

        actor->emotions.last_vehicle_exit_forced = 0;
    }

    actor->stimuli.vehicle_eviction = 0;

    return exited;
}
