/* actor_input_sample_position @0x8371EE70 — fill an actor_position_data snapshot from a unit: its origin,
 * facing (object +0x74), head marker position, translational velocity and BSP body location (from the
 * ultimate parent object). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_position_data.h"
#include "headers/object_datum.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/blam_data_globals.h"

extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);
extern int object_get_ultimate_parent(int object_index);

void actor_input_sample_position(int actor_index, int unit_index, actor_position_data *position)
{
    object_datum *object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    object_get_origin(unit_index, &position->body_position);
    position->body_facing = object->object.forward;   /* object+116 = _object_datum.forward */
    unit_get_head_position(unit_index, &position->head_position);
    object_get_velocities(unit_index, &position->velocity, 0);
    int ultimate_parent = object_get_ultimate_parent(unit_index);
    object_datum *ultimate_parent_object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, ultimate_parent)->datum);
    position->body_location = ultimate_parent_object->object.location;   /* object+152 = _object_datum.location */
}
