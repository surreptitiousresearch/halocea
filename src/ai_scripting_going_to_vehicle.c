/* ai_scripting_going_to_vehicle @0x83770F08 — return the number of active actors currently running the
 * "go to vehicle" action (action class 9) targeting the given vehicle unit. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_iterator.h"
#include "headers/vehicle_state_data.h"
#include "headers/actor_action.h"

extern void actor_iterator_new(actor_iterator *iterator, uint8_t active_only);
extern actor_datum *actor_iterator_next(actor_iterator *iterator);

int ai_scripting_going_to_vehicle(int unit_index)
{
    int count = 0;
    actor_iterator iterator;
    actor_iterator_new(&iterator, 1u);
    for ( actor_datum *actor = actor_iterator_next(&iterator);
          actor;
          actor = actor_iterator_next(&iterator) )
    {
        vehicle_state_data *vehicle_action = &actor->state.action_data.___u0.vehicle;
        if ( actor->state.action == actor_action_vehicle && vehicle_action->vehicle_index == unit_index )
            count = (int16_t)(count + 1);
    }
    return count;
}
