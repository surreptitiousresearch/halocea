/* actor_action_handle_initial_action @0x837F42B0 */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t actor_action_set_default_state(int actor_index, int16_t override_state);

int actor_action_handle_initial_action(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if (!actor->state.action && actor->state.mode)
        return actor_action_set_default_state(actor_index, -1);

    return 0;
}
