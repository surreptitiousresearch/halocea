/* actors_freeze @ 0x8371E688 — freeze every live actor's unit(s) when AI control is being released
 * (called from ai_update when AI goes inactive but still holds control data). For a swarm actor each
 * member unit is frozen; for a normal actor its single unit is frozen. Every actor is then marked frozen.
 *
 * Deviation: actor_freeze_unit's DB prototype is 2-arg (actor_index, unit_index) but the disassembly
 * (0x8371E71C / 0x8371E744) sets only r3 at both call sites — the second argument is a stale register,
 * never initialized in this function. Reconstructed as a single-argument call accordingly. */

#include <stdint.h>
#include "headers/actor_iterator.h"
#include "headers/actor_datum.h"
#include "headers/swarm_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void actor_freeze_unit(int actor_index);   /* DB: (int actor_index, int unit_index); 2nd arg phantom here */

void actors_freeze(void)
{
    actor_iterator iterator;
    actor_datum   *actor;

    actor_iterator_new(&iterator, 1u);
    for ( actor = actor_iterator_next(&iterator); actor; actor = actor_iterator_next(&iterator) )
    {
        actor_datum *current = DATA_ARRAY_ELEMENT(actor_data, actor_datum, iterator.index);

        if ( current->meta.swarm )
        {
            if ( current->meta.swarm_cache_index != -1 )
            {
                swarm_datum *swarm = DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, current->meta.swarm_cache_index);
                for ( int i = 0; i < swarm->unit_count; i = (int16_t)(i + 1) )
                    actor_freeze_unit(swarm->unit_indices[i]);
            }
        }
        else
        {
            actor_freeze_unit(current->meta.unit_index);
        }
        current->meta.frozen = 1;
    }
}
