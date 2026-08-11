/* player_examine_nearby_device @0x836AC098 — checks whether the player is looking at a usable device in
 * range and front-on, and if so (and the device can change position, e.g. a door/lever) posts the "use
 * device" action result for the player. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/player_action_result.h"
#include "headers/blam_data_globals.h"


extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern uint8_t fast_vector_intersects_sphere(const real_point3d *point, const real_vector3d *vector, const real_point3d *center, float radius);
extern uint8_t device_frontfacing(int device_index, const real_point3d *point, const real_vector3d *vector);
extern uint8_t device_can_change_position(int device_index);
extern void player_set_action_result(int player_index, int16_t action_result, int action_object_index, int16_t action_seat_index);

void player_examine_nearby_device(int player_index, int device_index)
{
    int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    object_datum *device =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;
    unit_datum *unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    real_point3d camera_position;
    unit_get_camera_position(unit_index, &camera_position);
    const real_vector3d *aim = &unit->unit.aiming_vector;

    if (fast_vector_intersects_sphere(&camera_position, aim, &device->object.bounding_sphere_center,
                                      device->object.bounding_sphere_radius)
        && device_frontfacing(device_index, &camera_position, aim))
    {
        if (device_can_change_position(device_index))
            player_set_action_result(player_index, _player_action_result_touch_device, device_index, -1);
    }
}
