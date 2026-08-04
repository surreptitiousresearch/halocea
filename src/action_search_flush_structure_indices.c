/* action_search_flush_structure_indices @0x83823420 — invalidates the actor's "search" action position/
 * structure cache (resolved to the DB-named search_state_data arm:
 * pursuit_location.cluster_index/surface_index, same convention as its siblings). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void action_search_flush_structure_indices(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->state.action_data.___u0.search.pursuit_location.cluster_index = -1;
    actor->state.action_data.___u0.search.pursuit_location.surface_index = -1;
}
