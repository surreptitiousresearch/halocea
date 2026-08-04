/* player_control_get_unit_camera_info @0x836DE844 — fills out the camera context for a local player's
 * unit. It records the unit and its camera origin, and if the unit is riding a vehicle seat, resolves
 * the seat's camera definition from the vehicle's seat block. If the unit is not in a (verified vehicle)
 * seat, it falls back to the unit definition's default camera. */

#include <stdint.h>
#include "headers/unit_camera_info.h"
#include "headers/player_control_globals.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void player_control_get_unit_camera_info(int16_t local_player_index, unit_camera_info *camera_info)
{
    camera_info->unit_camera = nullptr;
    int unit_index = player_control_globals->players[local_player_index].unit_index;
    camera_info->unit_index = unit_index;
    camera_info->seat_index = -1;

    if (unit_index == -1)
        return;

    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit_get_camera_position(unit_index, &camera_info->unit_origin);

    int parent_index = unit->object.parent_object_index;
    if (parent_index != -1)
    {
        /* verify the parent is a vehicle (type flag 0x2) before reading its seat block */
        unit_datum *parent = object_try_and_get_and_verify_type(parent_index, object_mask_vehicle);
        if (parent)
        {
            __int16 seat_index = unit->unit.parent_seat_index;
            int parent_index2 = unit->object.parent_object_index;
            unit_definition *parent_definition = TAG_GET(unit_definition, parent->definition_index);
            unit_seat *seats = (unit_seat *)parent_definition->unit.seats.address;
            camera_info->unit_index = parent_index2;
            camera_info->unit_camera = &seats[seat_index].camera;
            camera_info->seat_index = unit->unit.parent_seat_index;
            unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_index2)->datum);
        }
    }

    if ((unsigned __int16)camera_info->seat_index == 0xFFFF)
    {
        unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);
        camera_info->unit_camera = &definition->unit.camera;
    }
}
