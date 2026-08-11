/* action_flee_replace_prop @0x83825708 — swap a specific invalidated prop reference out of the actor's
 * "flee" action state (resolved to the DB-named flee_state_data arm: flee_prop_index). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/flee_state_data.h"
#include "headers/blam_data_globals.h"


void action_flee_replace_prop(int actor_index, int invalid_prop_index, int replacement_prop_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int *prop = &actor->state.action_data.___u0.flee.flee_prop_index;
    if ( *prop == invalid_prop_index )
        *prop = replacement_prop_index;
}
