/* actor_action_control @ 0x837F1A28 — dispatch to the current action's control callback (action index at
 * actor+0x6C). Passes actor_index when the callback is present. */

#include "headers/data_array.h"
#include "headers/action_specification.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


void actor_action_control(uint16_t actor_index)
{
    int16_t action = ((int16_t *)actor_data->data)[914 * actor_index + 54];
    void (__fastcall *control)(int) = global_action_functions[action].control;
    if ( control )
        control(actor_index);
}
