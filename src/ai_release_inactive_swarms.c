/* ai_release_inactive_swarms @ 0x836E7A68 — iterate every actor and erase each
 * inactive swarm actor (swarm set, not active, previously spawned i.e.
 * last_active_time != -1), accumulating the total swarm unit count released.
 * Writes a "%d swarm units" description, clears *more_to_release, and returns 1 if
 * any units were released, 0 otherwise. */

#include <stdint.h>
#include "headers/actor_iterator.h"
#include "headers/actor_datum.h"

extern int sprintf_0(char *string, const char *format, ...);
extern void actor_erase(int actor_index, uint8_t immediate);

unsigned int ai_release_inactive_swarms(char *result_description, uint8_t *more_to_release, uint8_t *working_memory, int16_t working_memory_size)
{
    int16_t released_unit_count = 0;
    actor_iterator iterator;
    actor_datum *actor;

    actor_iterator_new(&iterator, 0);
    for ( actor = actor_iterator_next(&iterator); actor; actor = actor_iterator_next(&iterator) )
    {
        if ( actor->meta.swarm && !actor->meta.active && actor->meta.last_active_time != -1 )
        {
            released_unit_count += actor->meta.swarm_unit_count;
            actor_erase(iterator.index, 1u);
        }
    }

    sprintf_0(result_description, "%d swarm units", released_unit_count);
    *more_to_release = 0;
    return released_unit_count != 0;
}
