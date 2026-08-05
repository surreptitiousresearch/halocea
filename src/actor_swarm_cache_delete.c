/* actor_swarm_cache_delete @0x8371DA18 — free the actor's swarm datum (actor +0x28 / dword[10]) and every
 * swarm component datum it owns (component handles array @ swarm +0x58, count at swarm word[1]), then clear
 * the actor's swarm-cache reference. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/swarm_datum.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

void actor_swarm_cache_delete(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->meta.swarm_cache_index == -1 )
        return;

    data_array *array = swarm_data;
    swarm_datum *swarm = DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
    if ( swarm->unit_count > 0 )
    {
        int i = 0;
        do
        {
            datum_delete(swarm_component_data, swarm->component_indices[i]);
            i = (int16_t)(i + 1);
        }
        while ( i < swarm->unit_count );
        array = swarm_data;
    }
    datum_delete(array, actor->meta.swarm_cache_index);
    actor->meta.swarm_cache_index = -1;
}
