/* action_uncover_flush_structure_indices @0x838250A0 — invalidates the actor's "uncover" action position/
 * structure cache (resolved to the DB-named uncover_state_data arm:
 * pursuit_location.cluster_index/surface_index — same fields as action_search_flush_structure_indices). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


void action_uncover_flush_structure_indices(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->state.action_data.___u0.uncover.pursuit_location.cluster_index = -1;
    actor->state.action_data.___u0.uncover.pursuit_location.surface_index = -1;
}
