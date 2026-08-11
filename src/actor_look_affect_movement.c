/* actor_look_affect_movement @ 0x837FB5B8 — decide whether the actor's look direction should steer its
 * movement this tick. The actor's movement direction spec sits at actor+1004. When the actor neither holds a
 * movement-look override (word at actor+1004's index 502, i.e. actor+1004) nor has a path, the look-steer
 * counter (word at actor+1000) is cleared. The result flag (actor[1285]) is set when that counter has
 * reached 3, the direction spec has a type, and the look direction decodes successfully into actor+1316. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/direction_specification.h"
#include "headers/real_vector3d.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/blam_data_globals.h"


extern uint8_t actor_path_has_path(int actor_index);
extern uint8_t actor_look_decode_direction(int actor_index, direction_specification *specification, real_vector3d *direction);

void actor_look_affect_movement(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    direction_specification *movement_direction = &actor->orders.look.primary_direction;

    if ( actor->orders.look.primary_direction.type == _direction_specification_movement && !actor_path_has_path(actor_index) )
        actor->orders.look.primary_priority = _primary_priority_none;

    actor->control.moving_forced_by_aiming = actor->orders.look.primary_priority >= _primary_priority_facing
               && movement_direction->type
               && actor_look_decode_direction(actor_index, movement_direction, (real_vector3d *)actor->control.moving_forced_aim_direction.n);
}
