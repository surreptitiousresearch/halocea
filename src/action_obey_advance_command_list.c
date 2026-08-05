/* action_obey_advance_command_list @0x837DF1E8 — advance every individual of an actor to the next command-list
 * command. Runs the advance callback over all individuals using the obey_state_data already stored in the actor
 * datum (at actor+156). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/obey_state_data.h"
#include "headers/blam_data_globals.h"
#include "headers/obey_individual_iterator.h"

extern void action_obey_individual_advance(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, void *user_data);
extern void action_obey_individuals_iterate(int actor_index, uint8_t initialize_structures, obey_state_data *state_data, obey_individual_iterator iterator, void *user_data);

void action_obey_advance_command_list(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    obey_state_data *state_data = &actor->state.action_data.___u0.obey;
    action_obey_individuals_iterate(actor_index, 0, state_data, action_obey_individual_advance, 0);
}
