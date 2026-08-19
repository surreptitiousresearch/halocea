/* action_obey_flush_structure_indices @0x837DF1B8 — invalidates the actor's "obey" action structure cache
 * (resolved to the DB-named obey_state_data arm: complex_control.destination_valid /
 * destination_surface_index, same convention as the other action_*_flush_structure_indices siblings). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


void action_obey_flush_structure_indices(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->state.action_data.___u0.obey.complex_control.destination_valid )
        actor->state.action_data.___u0.obey.complex_control.destination_surface_index = -1;
}
