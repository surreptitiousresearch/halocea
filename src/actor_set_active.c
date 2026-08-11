/* actor_set_active @0x83720D78 — toggle an actor's active state. Activating a swarm actor builds its swarm cache
 * (and aborts, flagging a pending retry, if the cache could not be created); deactivating frees the actor's props
 * and swarm cache. On a successful state change it also drives dormancy: activation clears dormancy when the
 * actor has no pending state, deactivation sets dormancy and stamps the deactivation time. Returns whether the
 * change went through (0 when a swarm activation was deferred). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


extern int actor_swarm_cache_new(int actor_index);
extern void actor_delete_props(int actor_index);
extern void actor_swarm_cache_delete(int actor_index);
extern void actor_set_dormant(int actor_index, uint8_t dormant);
extern int game_time_get(void);

int actor_set_active(int actor_index, uint8_t active)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int succeeded = 1;
    if ( actor->meta.active != active )
    {
        if ( active )
        {
            if ( actor->meta.swarm )
            {
                actor_swarm_cache_new(actor_index);
                if ( actor->meta.swarm_cache_index == -1 )  /* cache creation failed */
                {
                    succeeded = 0;
                    actor->meta.erase_next_tick = 1;  /* pending activation */
                }
            }
        }
        else
        {
            actor_delete_props(actor_index);
            actor_swarm_cache_delete(actor_index);
        }

        if ( succeeded )
        {
            if ( active )
            {
                int pending_state = actor->state.mode;
                actor->meta.active = 1;
                if ( !pending_state )
                {
                    actor_set_dormant(actor_index, 0);
                    return succeeded;
                }
            }
            else
            {
                actor_set_dormant(actor_index, 1u);
                actor->meta.active = 0;
                actor->meta.last_active_time = game_time_get();
            }
        }
    }
    return succeeded;
}
