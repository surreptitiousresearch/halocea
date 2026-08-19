/* actor_erase @ 0x83721028 — remove an actor and its unit(s) from the world. For a swarm actor, detach and
 * delete every member unit (draining the swarm's unit list) then actor_delete the actor as "died". For a
 * normal actor, run its death notification then delete its single unit. The immediate flag chooses between
 * object_delete_immediately and the deferred object_delete. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


extern void actor_swarm_detach_from_unit(int actor_index, int unit_index);
extern void object_delete_immediately(int object_index);
extern void object_delete(int object_index);
extern void actor_delete(int actor_index, uint8_t died);
extern void actor_died(int actor_index);

void actor_erase(int actor_index, uint8_t immediate)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->meta.swarm )
    {
        int unit_index = actor->meta.swarm_unit_index;
        if ( unit_index != -1 )
        {
            do
            {
                actor_swarm_detach_from_unit(actor_index, unit_index);
                if ( immediate )
                    object_delete_immediately(unit_index);
                else
                    object_delete(unit_index);
                unit_index = actor->meta.swarm_unit_index;
            }
            while ( unit_index != -1 );
        }
        actor_delete(actor_index, 1u);
    }
    else
    {
        int unit_index = actor->meta.unit_index;
        actor_died(actor_index);
        if ( immediate )
            object_delete_immediately(unit_index);
        else
            object_delete(unit_index);
    }
}
