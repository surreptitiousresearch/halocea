/* action_converse_replace_prop @0x837FCCF8 — swap a specific invalidated prop reference out of the actor's
 * "converse" action state (resolved to the DB-named converse_state_data arm: run_to_prop_index). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


void action_converse_replace_prop(int actor_index, int invalid_prop_index, int replacement_prop_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int *prop = &actor->state.action_data.___u0.converse.run_to_prop_index;
    if ( *prop == invalid_prop_index )
        *prop = replacement_prop_index;
}
