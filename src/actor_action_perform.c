/* actor_action_perform @ 0x837F1990 — dispatch to the current action's perform callback (action index at
 * actor+0x6C) and return its result (0 when no callback is present). Passes actor_index. */

#include "headers/data_array.h"
#include "headers/action_specification.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


uint8_t actor_action_perform(uint16_t actor_index)
{
    /* recovered: (int16_t *)actor_data->data + 914*idx + 54 -> actor_datum.state.action
     * (914*2 = 1828 = sizeof(actor_datum), +54*2 = +108 = 0x6C). */
    int16_t action = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->state.action;
    uint8_t (*perform)(int) = global_action_functions[action].perform;
    if ( perform )
        return perform(actor_index);
    return 0;
}
