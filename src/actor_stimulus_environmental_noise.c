/* actor_stimulus_environmental_noise @0x837D46C0 — unless the actor is already in a particular state
 * (action-state word +106 == 1), make it glance toward a one-off environmental noise at `position` as a
 * secondary look direction (type 3 = explicit point).
 *
 * DEVIATION: the decompiler copied `position` into the direction_specification's point field as a raw
 * bit-copy of just the x component's DWORD (`LODWORD(position->n[0])` into the aliased prop_index union
 * slot) plus separate float stores for y/z — a decompiler union-aliasing artifact, not a real type-pun.
 * Restored as a plain point copy, matching the direction_get_empty-then-override idiom already established
 * in ai_communication_look_secondary_at_object.c. The action-state word resolves to the DB-named actor_datum member state.mode (+106). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/actor_mode.h"
#include "headers/blam_data_globals.h"


extern direction_specification *direction_get_empty(direction_specification *result);
extern uint8_t actor_look_secondary(int actor_index, int16_t type, int16_t priority, direction_specification *direction);

void actor_stimulus_environmental_noise(int actor_index, int object_index, const real_point3d *position, int16_t count)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->state.mode == _actor_mode_asleep )
        return;

    direction_specification scratch;
    direction_specification direction = *direction_get_empty(&scratch);
    direction.type = _direction_specification_point;
    direction.___u1.point = *position;

    actor_look_secondary(actor_index, _secondary_look_environmental_noise, _secondary_look_priority_default, &direction);
}
