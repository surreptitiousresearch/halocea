/* actor_action_control @ 0x837F1A28 — dispatch to the current action's control callback (action index at
 * actor+0x6C). Passes actor_index when the callback is present. */

#include "headers/data_array.h"
#include "headers/action_specification.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


void actor_action_control(uint16_t actor_index)
{
    /* recovered: (int16_t *)actor_data->data + 914*idx + 54 -> actor_datum.state.action
     * (914*2 = 1828 = sizeof(actor_datum), +54*2 = +108 = 0x6C). */
    int16_t action = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->state.action;
    void (*control)(int) = global_action_functions[action].control;
    if ( control )
        control(actor_index);
}
