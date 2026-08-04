/* action_alert_flush_position_indices @0x83826728 — invalidates the actor's "alert" action position indices,
 * resolved to the DB-named alert_state_data arm of the action_data union. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


void action_alert_flush_position_indices(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->state.action_data.___u0.alert.target_move_position_index = -1;
    actor->state.action_data.___u0.alert.pending_move_position_index = -1;
}
