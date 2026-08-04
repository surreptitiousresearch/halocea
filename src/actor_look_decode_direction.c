/* actor_look_decode_direction @0x837FA9C8 — resolve a direction_specification into a world-space direction
 * vector for an actor's look/aim. The specification's type selects the source: a prop's aim point (1), the
 * actor's current look state — combat vector, alert vector, or look-target prop (2), an explicit point (3),
 * an explicit vector (4), the actor's defensive position (5), an object's head/origin (6), or the actor's
 * idle facing (0). Point-derived directions are returned normalised; the result is true only when a usable
 * (non-degenerate) direction was produced.
 *
 * The actor datum is 1828 bytes; the actor's own position is the three floats at actor+288 (float index 72). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/object_datum.h"
#include "headers/data_array.h"
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/actor_fire_state.h"
#include "headers/actor_danger_zone_type.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern void *datum_try_and_get(const data_array *data, int index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern float normalize3d(real_vector3d *v);

uint8_t actor_look_decode_direction(int actor_index, direction_specification *specification,
                                    real_vector3d *direction)
{
    unsigned int type = (uint16_t)specification->type;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if (type > _direction_specification_object)
        return 0;

    if (type == _direction_specification_prop) /* prop aim point */
    {
        prop_datum *prop = datum_try_and_get(prop_data, specification->___u1.prop_index);
        if (!prop)
            return 0;
        direction->n[0] = prop->head_position.n[0] - actor->input.position.head_position.x;  /* prop+260 */
        direction->n[1] = prop->head_position.n[1] - actor->input.position.head_position.y;
        direction->n[2] = prop->head_position.n[2] - actor->input.position.head_position.z;
        return normalize3d(direction) > 0.0f;
    }

    if (type == _direction_specification_target) /* current look state */
    {
        if (actor->control.fire_state == actor_fire_state_bursting)
        {
            *direction = actor->control.burst_aim_vector;
            return 1;
        }
        if (actor->control.aiming_at_fire_target)
        {
            *direction = actor->control.current_fire_target_aim_vector;
            return 1;
        }
        if (actor->target.target_prop_index != -1)
        {
            prop_datum *target = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index);
            direction->n[0] = target->center_of_mass.n[0] - actor->input.position.head_position.x;
            direction->n[1] = target->center_of_mass.n[1] - actor->input.position.head_position.y;
            direction->n[2] = target->center_of_mass.n[2] - actor->input.position.head_position.z;
            return normalize3d(direction) > 0.0f;
        }
        return 0;
    }

    if (type == _direction_specification_point) /* explicit point */
    {
        direction->n[0] = specification->___u1.point.n[0] - actor->input.position.head_position.x;
        direction->n[1] = specification->___u1.point.n[1] - actor->input.position.head_position.y;
        direction->n[2] = specification->___u1.point.n[2] - actor->input.position.head_position.z;
        return normalize3d(direction) > 0.0f;
    }

    if (type == _direction_specification_vector) /* explicit vector */
    {
        *direction = specification->___u1.vector;
        return 1;
    }

    if (type == _direction_specification_danger) /* actor defensive position */
    {
        if (actor->danger_zone.danger_type > actor_danger_zone_none)
        {
            direction->n[0] = actor->danger_zone.initial_position.x - actor->input.position.head_position.x;
            direction->n[1] = actor->danger_zone.initial_position.y - actor->input.position.head_position.y;
            direction->n[2] = actor->danger_zone.initial_position.z - actor->input.position.head_position.z;
            return normalize3d(direction) > 0.0f;
        }
        return 0;
    }

    if (type == _direction_specification_object) /* object head/origin */
    {
        object_datum *object = object_try_and_get_and_verify_type(specification->___u1.object_index, object_mask_all);
        if (object)
        {
            real_point3d position;
            int16_t object_type = object->object.type;  /* object+0xB4 */
            if (((1 << object_type) & object_mask_unit) != 0) /* biped or vehicle */
                unit_get_head_position(specification->___u1.object_index, &position);
            else
                object_get_origin(specification->___u1.object_index, &position);
            direction->n[0] = position.n[0] - actor->input.position.head_position.x;
            direction->n[1] = position.n[1] - actor->input.position.head_position.y;
            direction->n[2] = position.n[2] - actor->input.position.head_position.z;
            return normalize3d(direction) > 0.0f;
        }
        return 0;
    }

    /* type == _direction_specification_movement (0): actor idle facing */
    if (actor->control.moving)
    {
        *direction = actor->control.moving_towards_vector;
        return normalize3d(direction) > 0.0f;
    }
    return 0;
}
