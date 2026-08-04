/* first_person_camera_for_unit_and_vector @ 0x837C23D8 — fill an observer command for a first-person
 * view of a unit looking along a given forward vector. The eye rides the unit's camera position with a
 * default FOV; if the unit is seated in a parent vehicle whose seat definition requests it (seat flags
 * bit 0x80), the camera is instead snapped to the parent's "primary trigger" marker (turret viewmodel),
 * or its forward/up are rotated into the parent object's local frame. */

#include <stdint.h>
#include "headers/observer_command.h"
#include "headers/object_marker.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern void observer_up_from_forward(const real_vector3d *forward, real_vector3d *up);
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern real_vector3d *matrix4x3_inverse_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);

/* object data pointer for an object handle (object header datum: data ptr at int element 3*idx+2) */
static unit_datum *object_data_ptr(int object_handle)
{
    return ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_handle)->datum);
}

void first_person_camera_for_unit_and_vector(int unit_index, const real_vector3d *forward,
                                             observer_command *result)
{
    result->flags = 0;
    result->timer = 0.0f;
    result->___u1.__s0.focus_offset = *global_zero_vector3d;
    result->___u1.__s0.focus_distance = 0.0f;
    result->___u1.__s0.forward = *forward;
    result->___u1.__s0.field_of_view = 1.2217305f;
    observer_up_from_forward(&result->___u1.__s0.forward, &result->___u1.__s0.up);

    if ( unit_index != -1 )
    {
        unit_datum *unit_object = object_data_ptr(unit_index);
        int parent_object_index;

        unit_get_camera_position(unit_index, &result->___u1.__s0.focus_position);
        object_get_velocities(unit_index, &result->focus_velocity, 0);

        parent_object_index = unit_object->object.parent_object_index;
        if ( parent_object_index != -1 )
        {
            unit_datum *parent = object_try_and_get_and_verify_type(parent_object_index, object_mask_vehicle);
            if ( parent )
            {
                unit_definition *parent_definition = TAG_GET(unit_definition, parent->definition_index);
                unit_seat *seat = (unit_seat *)parent_definition->unit.seats.address
                                  + unit_object->unit.parent_seat_index;
                if ( (seat->flags & (1u << _unit_seat_slave_first_person_camera_bit)) != 0 )
                {
                    object_marker marker;
                    if ( object_get_marker_by_name(unit_object->object.parent_object_index,
                                                   "primary trigger", &marker, 1) )
                    {
                        result->___u1.__s0.focus_position.n[0] = marker.matrix.n[3][0];
                        result->___u1.__s0.focus_position.n[1] = marker.matrix.n[3][1];
                        result->___u1.__s0.focus_position.n[2] = marker.matrix.n[3][2];
                        result->___u1.__s0.forward.n[0] = marker.matrix.n[0][0];
                        result->___u1.__s0.forward.n[1] = marker.matrix.n[0][1];
                        result->___u1.__s0.forward.n[2] = marker.matrix.n[0][2];
                        result->___u1.__s0.up.n[0] = marker.matrix.n[2][0];
                        result->___u1.__s0.up.n[1] = marker.matrix.n[2][1];
                        result->___u1.__s0.up.n[2] = marker.matrix.n[2][2];
                        result->flags = 1;
                        return;
                    }
                }
                else
                {
                    /* rotate forward/up through the parent object's local frame */
                    real_matrix4x3 parent_matrix[2];
                    matrix4x3_from_point_and_vectors(parent_matrix,
                                                     &parent->object.position,
                                                     &parent->object.forward,
                                                     &parent->object.up);
                    matrix4x3_inverse_transform_normal(parent_matrix, &result->___u1.__s0.forward, &result->___u1.__s0.forward);
                    observer_up_from_forward(&result->___u1.__s0.forward, &result->___u1.__s0.up);
                    matrix4x3_transform_normal(parent_matrix, &result->___u1.__s0.forward, &result->___u1.__s0.forward);
                    matrix4x3_transform_normal(parent_matrix, &result->___u1.__s0.up, &result->___u1.__s0.up);
                }
            }
        }

        result->flags = 1;
    }
}
