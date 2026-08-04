/* unit_camera_get @0x837C2728 — pick the unit_camera to use for a unit: if the unit isn't attached to a
 * valid vehicle object (unit object field +284, type-verified as vehicle/mask 2), fall back to the unit's
 * own tag-defined camera (unit definition + 424). Otherwise look up the unit's seat within the vehicle
 * (vehicle definition's seats tag_block, field[186]; seat record stride 284, seat index at unit+752); if
 * the seat's flags dword has none of the driver/invisible/third-person-camera bits set, or the resolved seat-camera pointer would be
 * NULL, fall back to the unit's own camera as well; otherwise return the seat's embedded camera (seat
 * record + 132). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/vehicle_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_camera.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

unit_camera * unit_camera_get(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_definition *unit_def = TAG_GET(unit_definition, unit->definition_index);
    unit_camera *own_camera = &unit_def->unit.camera;

    int vehicle_index = unit->object.parent_object_index;
    if ( vehicle_index == -1 )
        return own_camera;

    object_datum *vehicle = object_try_and_get_and_verify_type(vehicle_index, object_mask_vehicle);
    if ( !vehicle )
        return own_camera;

    vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, vehicle->definition_index);
    unit_seat *seat = &((unit_seat *)vehicle_def->unit.seats.address)[unit->unit.parent_seat_index];
    unsigned int seat_flags = seat->flags;

    if ( (seat_flags & (1u << _unit_seat_is_driver_bit)) == 0
         && (seat_flags & (1u << _unit_seat_is_invisible_bit)) == 0
         && (seat_flags & (1u << _unit_seat_has_third_person_camera_bit)) == 0 )
        return own_camera;

    unit_camera *seat_camera = &seat->camera;
    if ( !seat_camera )
        return own_camera;
    return seat_camera;
}
