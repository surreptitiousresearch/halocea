/* actor_stimulus_bumped @0x837D4638 — make an actor glance (secondary look, default priority,
 * _secondary_look_bumped_prop) toward the prop it just bumped into. */

#include <stdint.h>
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"

extern direction_specification *direction_get_empty(direction_specification *result);
extern uint8_t actor_look_secondary(int actor_index, int16_t type, int16_t priority, direction_specification *direction);

void actor_stimulus_bumped(int actor_index, int prop_index)
{
    direction_specification look_target;
    direction_specification empty;
    look_target = *direction_get_empty(&empty);
    look_target.type = _direction_specification_prop;
    look_target.___u1.prop_index = prop_index;
    actor_look_secondary(actor_index, _secondary_look_bumped_prop, _secondary_look_priority_default, &look_target);
}
