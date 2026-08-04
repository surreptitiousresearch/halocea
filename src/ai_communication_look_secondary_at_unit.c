/* ai_communication_look_secondary_at_unit @0x837CC820 — make an actor glance toward a unit while speaking. If
 * the actor already has an active prop for that unit and the prop is in a "perceived" state (state 2 or 3),
 * the look targets the prop; otherwise it targets the unit's head position directly. No-op for invalid actor /
 * non-positive priority / missing or non-unit target. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern direction_specification *direction_get_empty(direction_specification *result);
extern int prop_get_active_by_unit_index(uint16_t actor_index, int unit_index);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern uint8_t actor_look_secondary(uint16_t actor_index, int16_t type, int16_t priority, direction_specification *direction);

void ai_communication_look_secondary_at_unit(int actor_index, int16_t type, int16_t priority,
                                             int look_unit_index, int prop_index)
{
    if ( actor_index == -1 || priority <= 0 || look_unit_index == -1
      || !object_try_and_get_and_verify_type(look_unit_index, object_mask_unit) )
        return;

    direction_specification scratch;
    direction_specification direction = *direction_get_empty(&scratch);

    int16_t prop_state;
    if ( (prop_index == -1 && (prop_index = prop_get_active_by_unit_index(actor_index, look_unit_index)) == -1)
      || (prop_state = (DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index))->state) < 2
      || prop_state > 3 )
    {
        direction.type = _direction_specification_point;
        unit_get_head_position(look_unit_index, &direction.___u1.point);
    }
    else
    {
        direction.___u1.prop_index = prop_index;
        direction.type = _direction_specification_prop;
    }
    actor_look_secondary(actor_index, type, priority, &direction);
}
