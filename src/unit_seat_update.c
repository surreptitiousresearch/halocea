/* unit_seat_update @ 0x836CBD38 — track the seat-relative acceleration felt by a unit so seated
 * occupants can be jostled. Resolves the unit's reference frame: if it is riding a parent in a seat
 * (object+284 parent, object+752 seat index) it uses the parent's seat marker (seat block word[186] of the
 * parent definition, stride 284, marker name at +36) with the parent origin as position and the seat's
 * acceleration scale at +100; otherwise it uses the unit's own orientation and the unit definition's
 * default acceleration scale (def+512). It differentiates the marker position twice (stored previous
 * position at object+844..852, previous velocity at object+856..864) to get acceleration, projects it onto
 * the marker's forward (row0), left (row0xrow2) and up (row2) axes, scales, biases by 0.5, clamps to [0,1],
 * and writes the three normalized components to object+880/884/888. With no valid frame the components
 * default to 0.5 (neutral). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/global_tag_instances.h"
#include "headers/object_marker.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);

void unit_seat_update(int object_index)
{
    char have_frame = 0;
    unit_datum *object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    int parent_object = object->object.parent_object_index;
    __int16 seat_index;
    const float *acceleration_scale = 0;
    object_marker marker;

    if ( parent_object == -1 || (seat_index = object->unit.parent_seat_index) == -1 )
    {
        have_frame = 1;
        unit_definition *definition = TAG_GET(unit_definition, object->definition_index);
        marker.matrix.n[3][0] = object->object.position.n[0];
        marker.matrix.n[3][1] = object->object.position.n[1];
        marker.matrix.n[3][2] = object->object.position.n[2];
        acceleration_scale = (const float *)&definition->unit.acceleration_scale;
        marker.matrix.n[0][0] = object->object.forward.n[0];
        marker.matrix.n[0][1] = object->object.forward.n[1];
        marker.matrix.n[0][2] = object->object.forward.n[2];
        marker.matrix.n[2][0] = object->object.up.n[0];
        marker.matrix.n[2][1] = object->object.up.n[1];
        marker.matrix.n[2][2] = object->object.up.n[2];
    }
    else
    {
        /* DEVIATION (fix): the previous transcription resolved TAG_INSTANCE twice; per the DB decompile
         * the seat block comes from a single tag-instance lookup of the parent definition index */
        unit_definition *parent_definition = TAG_GET(unit_definition, DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_object)->datum->definition_index);
        unit_seat *seat = &((unit_seat *)parent_definition->unit.seats.address)[seat_index];
        if ( object_get_marker_by_name(parent_object, seat->marker_name, &marker, 1) )
        {
            object_get_origin(object->object.parent_object_index, (real_point3d *)marker.matrix.n[3]);
            acceleration_scale = (const float *)&seat->acceleration_scale;
            have_frame = 1;
        }
    }

    if ( have_frame )
    {
        float dx = marker.matrix.n[3][0] - object->unit.seat_last_position.n[0];
        float dy = marker.matrix.n[3][1] - object->unit.seat_last_position.n[1];
        float dz = marker.matrix.n[3][2] - object->unit.seat_last_position.n[2];
        float ax = dx - object->unit.seat_last_velocity.n[0];
        float ay = dy - object->unit.seat_last_velocity.n[1];
        float az = dz - object->unit.seat_last_velocity.n[2];

        float forward_accel = (((marker.matrix.n[0][1] * ay)
            + ((marker.matrix.n[0][2] * az) + (marker.matrix.n[0][0] * ax)))
            * *acceleration_scale) + 0.5f;
        float left_accel = ((((marker.matrix.n[0][2] * marker.matrix.n[2][1])
                - (marker.matrix.n[2][2] * marker.matrix.n[0][1])) * ax)
            + ((((marker.matrix.n[2][0] * marker.matrix.n[0][1])
                    - (marker.matrix.n[0][0] * marker.matrix.n[2][1])) * az)
                + (((marker.matrix.n[0][0] * marker.matrix.n[2][2])
                    - (marker.matrix.n[0][2] * marker.matrix.n[2][0])) * ay)));
        float up_accel = (((marker.matrix.n[2][1] * ay)
            + ((marker.matrix.n[2][2] * az) + (marker.matrix.n[2][0] * ax)))
            * acceleration_scale[2]) + 0.5f;

        object->unit.seat_desired_acceleration.n[0] = forward_accel;
        object->unit.seat_desired_acceleration.n[1] = (left_accel * acceleration_scale[1]) + 0.5f;
        object->unit.seat_desired_acceleration.n[2] = up_accel;

        if ( forward_accel < 0.0f )
            forward_accel = 0.0f;
        else if ( forward_accel > 1.0f )
            forward_accel = 1.0f;
        object->unit.seat_desired_acceleration.n[0] = forward_accel;

        float left_value = object->unit.seat_desired_acceleration.n[1];
        if ( left_value < 0.0f )
            left_value = 0.0f;
        else if ( left_value > 1.0f )
            left_value = 1.0f;
        object->unit.seat_desired_acceleration.n[1] = left_value;

        float up_value = object->unit.seat_desired_acceleration.n[2];
        if ( up_value < 0.0f )
            up_value = 0.0f;
        else if ( up_value > 1.0f )
            up_value = 1.0f;
        object->unit.seat_desired_acceleration.n[2] = up_value;

        object->unit.seat_last_velocity.n[0] = dx;
        object->unit.seat_last_velocity.n[1] = dy;
        object->unit.seat_last_velocity.n[2] = dz;
        object->unit.seat_last_position.n[0] = marker.matrix.n[3][0];
        object->unit.seat_last_position.n[1] = marker.matrix.n[3][1];
        object->unit.seat_last_position.n[2] = marker.matrix.n[3][2];
    }
    else
    {
        object->unit.seat_desired_acceleration.n[2] = 0.5f;
        object->unit.seat_desired_acceleration.n[1] = 0.5f;
        object->unit.seat_desired_acceleration.n[0] = 0.5f;
    }
}
