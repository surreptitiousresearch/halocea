/* actor_action_replace_prop @0x837F1A70 — dispatch a prop-replacement to the actor's current action handler:
 * look up the action_specification for the actor's active action (state.action) and, if it provides a
 * replace_prop callback, invoke it.
 *
 * Deviation: the decompiler drops the callback's arguments; the disasm tail-passes the actor and prop
 * indices, so they are forwarded here. */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/action_specification.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


void actor_action_replace_prop(uint16_t actor_index, int invalid_prop_index,
                               int replacement_prop_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    void (__fastcall *replace_prop)(int, int, int) =
        global_action_functions[actor->state.action].replace_prop;
    if ( replace_prop )
        replace_prop(actor_index, invalid_prop_index, replacement_prop_index);
}
