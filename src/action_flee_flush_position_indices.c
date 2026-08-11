/* action_flee_flush_position_indices @0x83825160 — invalidates the actor's "flee" action position index
 * and marks its flee state dirty (resolved to the DB-named flee_state_data arm: flee_firing_position_index /
 * find_new_flee_position). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/flee_state_data.h"
#include "headers/blam_data_globals.h"


void action_flee_flush_position_indices(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->state.action_data.___u0.flee.flee_firing_position_index = -1;
    actor->state.action_data.___u0.flee.find_new_flee_position = 1;
}
