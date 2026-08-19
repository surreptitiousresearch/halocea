#include "headers/object_type.h"
#include "headers/unit_datum.h"
/* ai_scripting_command_list_advance_by_unit @0x837717B0 — script command: advance the actor controlling a
 * specific unit to the next command in its command list. The actor index is read at unit+0x1F4 (unit.actor_index),
 * falling back to unit.swarm_actor_index if the first is -1. */

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void action_obey_advance_command_list(int actor_index);

void ai_scripting_command_list_advance_by_unit(int unit_index)
{
    if ( unit_index == -1 )
        return;

    unit_datum *unit_object = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    if ( !unit_object )
        return;

    int actor_index = unit_object->unit.actor_index;
    if ( actor_index != -1 || (actor_index = unit_object->unit.swarm_actor_index) != -1 )
        action_obey_advance_command_list(actor_index);
}
