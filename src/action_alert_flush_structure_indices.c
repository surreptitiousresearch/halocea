/* action_alert_flush_structure_indices @0x83826758 — invalidates the actor's "alert" action structure cache,
 * resolved to the DB-named alert_state_data arm of the action_data union
 * (target_position_definition.cluster_index/surface_index). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


void action_alert_flush_structure_indices(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->state.action_data.___u0.alert.target_position_definition.cluster_index = -1;
    actor->state.action_data.___u0.alert.target_position_definition.surface_index = -1;
}
