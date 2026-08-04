/* actor_swarm_unit_died @0x83720874 — handle the death of a unit belonging to a swarm actor: detach
 * the actor from the unit, and if the swarm has no remaining members (word @ actor +30 == 0) delete
 * the actor and refresh its encounter status. Actor records are stride 1828. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void actor_swarm_detach_from_unit(uint16_t actor_index, int unit_index);
extern void actor_delete(int actor_index, uint8_t died);
extern void encounter_update_status(int encounter_index);

void actor_swarm_unit_died(uint16_t actor_index, int unit_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_swarm_detach_from_unit(actor_index, unit_index);
    if ( !actor->meta.swarm_unit_count )
    {
        int encounter_index = actor->meta.encounter_index;
        actor_delete(actor_index, 1u);
        if ( encounter_index != -1 )
            encounter_update_status(encounter_index);
    }
}
