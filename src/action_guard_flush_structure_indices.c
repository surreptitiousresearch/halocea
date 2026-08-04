/* action_guard_flush_structure_indices @0x838246B8 — invalidates the actor's "guard" action structure
 * cache (resolved to the DB-named guard_state_data arm: guard_location_type /
 * guard_point.cluster_index/surface_index). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/actor_guard_location_type.h"
#include "headers/blam_data_globals.h"


void action_guard_flush_structure_indices(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->state.action_data.___u0.guard.guard_location_type == _actor_guard_location_point )
        actor->state.action_data.___u0.guard.___u17.guard_point.surface_index = -1;
}
