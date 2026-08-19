/* ai_scripting_command_list_by_unit @0x837716D8 — script command: order the actor controlling a specific unit to
 * obey a scenario command list (actor_action_obey). No-op if the unit has no actor (object+500 == -1). */

#include <stdint.h>
#include "headers/obey_state_data.h"
#include "headers/action_state_data.h"
#include "headers/object_type.h"
#include "headers/actor_action.h"
#include "headers/unit_datum.h"

#include "headers/obey_state_data.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t action_obey_command_list_setup(int actor_index, int16_t command_list_index, obey_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);

void ai_scripting_command_list_by_unit(int unit_index, int16_t command_list_index)
{
    if ( unit_index == -1 )
        return;

    unit_datum *unit_object = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    if ( !unit_object )
        return;

    int actor_index = unit_object->unit.actor_index;   /* unit +0x1F4 */
    if ( actor_index == -1 )
        return;

    action_state_data action_data;
    if ( action_obey_command_list_setup(actor_index, command_list_index, &action_data.___u0.obey) )
        actor_action_change(actor_index, actor_action_obey, &action_data);
}
