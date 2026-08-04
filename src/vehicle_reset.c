/* vehicle_reset @0x8375CD6C — zeroes a vehicle's runtime physics/steering scratch fields (offsets 1228-1312;
 * same salted object-header lookup as vehicle_hover.c). The hover anchor point at +1276 (see vehicle_hover.c)
 * is deliberately left untouched — it is not part of this reset range. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/blam_data_globals.h"


void vehicle_reset(int vehicle_index)
{
    vehicle_datum *vehicle = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;

    vehicle->vehicle.speed = 0.0f;
    vehicle->vehicle.flags = 0;
    vehicle->vehicle.slide = 0.0f;
    vehicle->vehicle.stop_time = 0;
    vehicle->vehicle.turn = 0.0f;
    vehicle->vehicle.airborne_ticks = 0;
    vehicle->vehicle.wheel = 0.0f;
    vehicle->vehicle.upending_type = 0;
    vehicle->vehicle.left_tread = 0.0f;
    vehicle->vehicle.upending_ticks = 0;
    vehicle->vehicle.right_tread = 0.0f;
    vehicle->vehicle.on_ground_ticks = 0;
    vehicle->vehicle.thrust = 0.0f;
    vehicle->vehicle.hover = 0.0f;
    *(int *)&vehicle->vehicle.suspension[0] = 0;   /* suspension is char[8]; cleared as two 32-bit stores */
    *(int *)&vehicle->vehicle.suspension[4] = 0;
    vehicle->vehicle.collision_force.n[0] = 0.0f;
    vehicle->vehicle.collision_force.n[1] = 0.0f;
    vehicle->vehicle.collision_force.n[2] = 0.0f;
    vehicle->vehicle.collision_torque.n[0] = 0.0f;
    vehicle->vehicle.collision_torque.n[1] = 0.0f;
    vehicle->vehicle.collision_torque.n[2] = 0.0f;
    vehicle->vehicle.stuck_mass_point_flags = 0;
}
