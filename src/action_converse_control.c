/* action_converse_control @0x837FCC58 — per-tick "converse" action update: resolve which unit this actor is
 * conversing with (an explicit override at +172, else the conversation record's +16 field via
 * prop_get_active_by_unit_index), latch the converse-active flag (+1020 word), and if a target unit was
 * found, stash it in the shared action-state scratch fields (+1008/+1004/+1000 — same fields
 * action_wait_control.c documents as the action-state word pair / saved path-target slot). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/conversation_datum.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/idle_look_type.h"
#include "headers/blam_data_globals.h"

extern int prop_get_active_by_unit_index(uint16_t actor_index, int unit_index);

void action_converse_control(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    conversation_datum *conversation = 0;
    int active_by_unit_index = -1;

    int conversation_index = actor->state.action_data.___u0.converse.conversation_index;
    if ( conversation_index != -1 )
        conversation = &((conversation_datum *)conversation_data->data)[(unsigned __int16)conversation_index];

    int override_unit_index = actor->state.action_data.___u0.converse.run_to_prop_index;
    if ( override_unit_index == -1 )
    {
        if ( conversation )
        {
            int leader_unit_index = conversation->triggering_player_unit_index;
            if ( leader_unit_index != -1 )
                active_by_unit_index = prop_get_active_by_unit_index(actor_index, leader_unit_index);
        }
    }
    else
    {
        active_by_unit_index = override_unit_index;
    }

    actor->orders.look.idle_look_type = _idle_look_noncombat;

    if ( active_by_unit_index != -1 )
    {
        actor->orders.look.primary_direction.___u1.prop_index = active_by_unit_index;
        actor->orders.look.primary_direction.type = _direction_specification_prop;
        actor->orders.look.primary_priority = _primary_priority_facing;
    }
}
