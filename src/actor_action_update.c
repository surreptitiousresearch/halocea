/* actor_action_update @ 0x837F19E0 — dispatch to the current action's update callback. The action index is
 * stored at actor+0x6C (*(short*)(actor_data->data + 1828*idx + 108)). Passes actor_index when the callback
 * is present (the decompiler dropped the argument from the indirect tail call). */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/action_specification.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


void actor_action_update(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int16_t action = actor->state.action;
    void (__fastcall *update)(int) = global_action_functions[action].update;
    if ( update )
        update(actor_index);
}
