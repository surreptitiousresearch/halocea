/* actor_swarm_unit_died @0x83720848 — handle the death of a unit belonging to a swarm actor: detach
 * the actor from the unit, and if the swarm has no remaining members (word @ actor +30 == 0) delete
 * the actor and refresh its encounter status. Actor records are stride 1828. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void actor_swarm_detach_from_unit(int actor_index, int unit_index);
extern void actor_delete(int actor_index, uint8_t died);
extern void encounter_update_status(int encounter_index);

/* DEVIATION: actor_index is a full 32-bit datum handle, not a uint16_t index — the same truncation as
 * actor_swarm_detach_from_unit, one level up. @0x83720860 the prologue narrows into a SEPARATE register
 * (`clrlwi r10, r3, 16`) that feeds ONLY the actor_data index (`mulli r10, r10, 0x724`), while `mr r31, r3`
 * @0x83720864 keeps the whole word and r3 reaches `bl actor_swarm_detach_from_unit` @0x83720878 untouched —
 * and that callee compares it whole (`cmpw cr6, r4, r3` @0x8371D8A0). r31 is likewise handed to actor_delete
 * whole (`mr r3, r31` @0x83720890). All three callers load the argument with a full-word `lwz r3, 0x1F8(rN)`
 * and guard it with a 32-bit `cmpwi r3, -1`. DATA_ARRAY_ELEMENT applies the (uint16_t) index extract itself. */
void actor_swarm_unit_died(int actor_index, int unit_index)
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
