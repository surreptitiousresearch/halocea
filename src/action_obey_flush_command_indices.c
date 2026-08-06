/* action_obey_flush_command_indices @0x837DF140 — if the actor's obey command list index is out of range,
 * clear it (mark finished) and cancel the actor's current action; otherwise flush queued command indices
 * across every individual in the obey group. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/scenario.h"
#include "headers/obey_state_data.h"
#include "headers/action_state_data.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"

extern void action_obey_individuals_iterate(int actor_index, uint8_t initialize_structures,
    obey_state_data *state_data,
    void (*iterator)(int, int, int16_t, obey_individual_simple_control *, obey_individual_complex_control *, void *),
    void *user_data);
extern void action_obey_individual_flush_command_indices(int actor_index, int unit_index, int16_t command_list_index,
    obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, void *user_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);

void action_obey_flush_command_indices(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    obey_state_data *state_data = &actor->state.action_data.___u0.obey;

    if ( state_data->command_list_index < 0 || state_data->command_list_index >= global_scenario->ai_command_lists.count )
    {
        state_data->command_list_index = -1;
        state_data->finished = 1;
        actor_action_change(actor_index, actor_action_none, 0);
    }
    else
    {
        action_obey_individuals_iterate(actor_index, 0, state_data, action_obey_individual_flush_command_indices, 0);
    }
}
