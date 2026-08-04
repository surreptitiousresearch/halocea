/* first_person_camera_deterministic @0x837C22D0 — first-person camera placement: default to the unit's
 * camera position and its own forward vector (unit +572). If the unit is riding a vehicle (unit +284) in a
 * seat (seat index unit +752) whose seat flags dword (vehicle definition seats block, 284-byte stride,
 * per unit_camera_get.c/local_player_is_piloting_aircraft.c) has bit 0x80 set (a first-person-weapon-style
 * seat), instead snaps to the "primary trigger" marker's own position/forward. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_marker.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);

void first_person_camera_deterministic(int unit_index, real_point3d *position, real_vector3d *forward)
{
    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    unit_get_camera_position(unit_index, position);
    *forward = unit->unit.aiming_vector; /* +572 = unit_datum.unit.aiming_vector */

    int vehicle_index = unit->object.parent_object_index;
    if ( vehicle_index == -1 )
        return;

    int *vehicle = object_try_and_get_and_verify_type(vehicle_index, object_mask_vehicle);
    if ( !vehicle )
        return;

    vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, *vehicle);
    /* fix: parent_seat_index lives in the unit-block (_unit_datum), not on unit_datum directly */
    unit_seat *seat = (unit_seat *)vehicle_def->unit.seats.address + unit->unit.parent_seat_index;

    if ( (seat->flags & (1u << _unit_seat_slave_first_person_camera_bit)) == 0 )
        return;

    object_marker marker;
    if ( object_get_marker_by_name(vehicle_index, "primary trigger", &marker, 1) )
    {
        position->n[0] = marker.matrix.n[3][0];
        position->n[1] = marker.matrix.n[3][1];
        position->n[2] = marker.matrix.n[3][2];
        forward->n[0] = marker.matrix.n[0][0];
        forward->n[1] = marker.matrix.n[0][1];
        forward->n[2] = marker.matrix.n[0][2];
    }
}
