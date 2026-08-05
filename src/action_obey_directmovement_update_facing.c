/* action_obey_directmovement_update_facing @0x837DCD50 — resolve the world-space movement direction for an
 * actor obeying a scripted "direct movement" order and store it into the order's control vector. The base
 * facing is the actor's own sampled facing when the order targets its controlled unit, otherwise the unit's
 * current facing. The order's facing mode then selects the result: 0 = along the base facing, 1 = opposite it,
 * 2 = a horizontal vector perpendicular to it (base facing x up, falling back to base facing x the unit's
 * lateral axis, then to global forward if degenerate), 3 = opposite that perpendicular. Modes above 3 do
 * nothing.
 *
 * DEVIATION: actor typed as actor_datum* to name meta.unit_index (+24) and input.facing_vector (+372);
 * register-pun doubles are plain float math. The unit object's up axis (object.up) is read via the standard
 * salted object lookup. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/actor_datum.h"
#include "headers/obey_individual_simple_control.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


extern void unit_get_facing_vector(int unit_index, real_vector3d *facing_vector);
extern float normalize3d(real_vector3d *v);

void action_obey_directmovement_update_facing(uint16_t actor_index, int unit_index, obey_individual_simple_control *simple_control)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    real_vector3d base_facing;
    if ( unit_index == actor->meta.unit_index )
    {
        base_facing.n[0] = actor->input.facing_vector.n[0];
        base_facing.n[1] = actor->input.facing_vector.n[1];
        base_facing.n[2] = actor->input.facing_vector.n[2];
    }
    else
    {
        unit_get_facing_vector(unit_index, &base_facing);
    }

    unsigned int facing_mode = (uint16_t)simple_control->___u5.directmovement.facing;
    if ( facing_mode > 3 )
        return;

    real_vector3d result;
    /* No DB enum names this domain: enum_oracle --lookup over "directmovement facing"/
     * "override_movement_facing" returns nothing above noise, and the same codes are already
     * adjudicated BLOCKED where actor_move_calculate_movement.c consumes them as
     * actor_move_orders.override_movement_facing. Domain (from this function's own branches):
     * 0 = base facing, 1 = reversed, 2/3 = perpendicular, >3 = no facing override. */
    if ( facing_mode == 1 )
    {
        result = base_facing;   /* negated at the end */
    }
    else if ( facing_mode == 0 )
    {
        simple_control->___u5.directmovement.vector = base_facing;
        return;
    }
    else   /* modes 2 and 3: perpendicular to the base facing */
    {
        real_vector3d perpendicular;
        perpendicular.n[0] = global_up3d->n[1] * base_facing.n[2] - global_up3d->n[2] * base_facing.n[1];
        perpendicular.n[1] = global_up3d->n[2] * base_facing.n[0] - global_up3d->n[0] * base_facing.n[2];
        perpendicular.n[2] = global_up3d->n[0] * base_facing.n[1] - global_up3d->n[1] * base_facing.n[0];
        if ( normalize3d(&perpendicular) == 0.0f )
        {
            object_datum *unit_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
            perpendicular.n[0] = unit_object->object.up.n[1] * base_facing.n[2] - unit_object->object.up.n[2] * base_facing.n[1];
            perpendicular.n[1] = unit_object->object.up.n[2] * base_facing.n[0] - unit_object->object.up.n[0] * base_facing.n[2];
            perpendicular.n[2] = unit_object->object.up.n[0] * base_facing.n[1] - unit_object->object.up.n[1] * base_facing.n[0];
            if ( normalize3d(&perpendicular) == 0.0f )
            {
                perpendicular.n[0] = global_forward3d->n[0];
                perpendicular.n[1] = global_forward3d->n[1];
                perpendicular.n[2] = global_forward3d->n[2];
            }
        }
        if ( facing_mode == 2 )
        {
            simple_control->___u5.directmovement.vector = perpendicular;
            return;
        }
        result = perpendicular;   /* mode 3: negated at the end */
    }

    simple_control->___u5.directmovement.vector.n[2] = -result.n[2];
    simple_control->___u5.directmovement.vector.n[1] = -result.n[1];
    simple_control->___u5.directmovement.vector.n[0] = -result.n[0];
}
