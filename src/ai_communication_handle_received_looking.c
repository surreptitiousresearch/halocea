/* ai_communication_handle_received_looking @0x837CDE58 — when an actor hears a communication carrying a look
 * directive, make it glance at the indicated target. look_type 1 directs a secondary look at a unit (priority 8
 * if it is this actor's own prop's unit, else 9); look_type 2 directs a secondary look at an arbitrary object
 * (direction type 6). No-op for non-positive look_type. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/unit_speech_item.h"  /* ai_information_packet */
#include "headers/object_type.h"
#include "headers/ai_information_look_type.h"
#include "headers/blam_data_globals.h"

extern void ai_communication_look_secondary_at_unit(int actor_index, int16_t type, int16_t priority, int look_unit_index, int prop_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern direction_specification *direction_get_empty(direction_specification *result);
extern uint8_t actor_look_secondary(uint16_t actor_index, int16_t type, int16_t priority, direction_specification *direction);

void ai_communication_handle_received_looking(int actor_index, uint16_t prop_index,
                                              ai_information_packet *information)
{
    int16_t look_type = information->look_type;
    if ( look_type <= 0 )
        return;

    int look_target = information->look_data.___u0.unit.unit_index;  /* recovered: *(int *)information->look_data.raw -> look_data.___u0.unit.unit_index */
    int16_t type = 9;
    if ( look_type == 1 && look_target == (DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index))->unit_index )
        type = 8;

    if ( information->look_type == _ai_information_look_unit )
    {
        ai_communication_look_secondary_at_unit(actor_index, type, information->look_priority, look_target, -1);
    }
    else if ( information->look_type == _ai_information_look_object )
    {
        int unit_index = look_target;
        int16_t look_priority = information->look_priority;
        if ( actor_index != -1 && look_priority > 0 && unit_index != -1
          && object_try_and_get_and_verify_type(look_target, object_mask_all) )
        {
            direction_specification scratch;
            direction_specification direction = *direction_get_empty(&scratch);
            direction.type = _direction_specification_object;
            direction.___u1.prop_index = unit_index;
            actor_look_secondary(actor_index, type, look_priority, &direction);
        }
    }
}
