/* ai_scripting_exit_vehicle @0x83770F90 — order every actor addressed by the AI index that is currently in
 * a vehicle to exit its seat. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"

extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern uint8_t unit_try_and_exit_seat(int object_index, uint8_t should_allow_clients);

void ai_scripting_exit_vehicle(int ai_index)
{
    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);
    for ( actor_datum *actor = ai_index_actor_iterator_next(&iterator);
          actor;
          actor = ai_index_actor_iterator_next(&iterator) )
    {
        if ( actor->input.vehicle_index != -1 )
        {
            int unit_index = actor->meta.unit_index;
            if ( unit_index != -1 )
                unit_try_and_exit_seat(unit_index, 0);
        }
    }
}
