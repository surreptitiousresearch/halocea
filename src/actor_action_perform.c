/* actor_action_perform @ 0x837F1990 — dispatch to the current action's perform callback (action index at
 * actor+0x6C) and return its result (0 when no callback is present). Passes actor_index. */

#include "headers/data_array.h"
#include "headers/action_specification.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


uint8_t actor_action_perform(uint16_t actor_index)
{
    __int16 action = ((__int16 *)actor_data->data)[914 * actor_index + 54];
    unsigned __int8 (__fastcall *perform)(int) = global_action_functions[action].perform;
    if ( perform )
        return perform(actor_index);
    return 0;
}
