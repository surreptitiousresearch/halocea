/* actor_conversation_control @ 0x837FCD28 — when the actor is in a conversation (conversation index at
 * actor+476 and addressee unit index at actor+480 both valid), aim a secondary "look" at the addressee:
 * prefer the addressee's active prop (look type "prop"); otherwise look at the addressee's head position
 * (look type "point"). Issued through actor_look_secondary at priority 5. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/real_point3d.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/blam_data_globals.h"


extern direction_specification *direction_get_empty(direction_specification *result);
extern int prop_get_active_by_unit_index(int actor_index, int unit_index);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern uint8_t actor_look_secondary(int actor_index, int16_t type, int16_t priority, direction_specification *direction);

void actor_conversation_control(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->external_orders.conversation_index == -1 || actor->external_orders.conversation_attention_unit_index == -1 )
        return;

    direction_specification look;
    direction_specification empty;
    look = *direction_get_empty(&empty);

    int addressee_unit_index = actor->external_orders.conversation_attention_unit_index;
    int active_prop_index = prop_get_active_by_unit_index(actor_index, addressee_unit_index);
    if ( active_prop_index == -1 )
    {
        look.type = _direction_specification_point;
        unit_get_head_position(actor->external_orders.conversation_attention_unit_index, &look.___u1.point); /* recovered: (real_point3d *)&look.___u1 -> look.___u1.point */
    }
    else
    {
        look.___u1.prop_index = active_prop_index; /* recovered: look.___u1.prop_index -> look.___u1.prop_index */
        look.type = _direction_specification_prop;
    }
    actor_look_secondary(actor_index, _secondary_look_communicating_prop, _secondary_look_priority_turn_and_aim, &look);
}
