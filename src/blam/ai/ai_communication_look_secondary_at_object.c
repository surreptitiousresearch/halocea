/* ai_communication_look_secondary_at_object @0x837CC938 — make an actor glance toward an object
 * (_direction_specification_object) as a secondary look, provided the actor/object indices are valid, priority is positive,
 * and the object can actually be resolved (object_try_and_get_and_verify_type with an "any type" mask). */

#include <stdint.h>
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/object_type.h"

extern direction_specification *direction_get_empty(direction_specification *result);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t actor_look_secondary(int actor_index, int16_t type, int16_t priority, direction_specification *direction);

void ai_communication_look_secondary_at_object(int actor_index, int16_t type, int16_t priority, int object_index)
{
    if ( actor_index == -1 || priority <= 0 || object_index == -1 )
        return;

    if ( !object_try_and_get_and_verify_type(object_index, object_mask_all) )
        return;

    direction_specification scratch;
    direction_specification direction = *direction_get_empty(&scratch);
    direction.type = _direction_specification_object;
    direction.___u1.object_index = object_index;

    actor_look_secondary(actor_index, type, priority, &direction);
}
