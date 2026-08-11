/* actor_action_class @0x837F3F20 — return the action class of the actor's currently active action, looked up
 * through the global action-function dispatch table. */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/action_specification.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


int16_t actor_action_class(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    return (uint16_t)global_action_functions[actor->state.action].action_class;
}
