/* update_human_tank_physics @0x8375E74C — per-tick physics for a human tank. Derives the two tracks'
 * friction velocities from throttle ± turn, advances and wraps each track's tread-scroll angle into
 * [0,period), and when the vehicle uses the powered 4-point physics model (id 2) writes the per-track
 * ground friction velocities (with identity rotations) before handing off to physics_update.
 *
 * DEVIATION: as with update_human_jeep_physics, the decompiler invents phantom double/long-double args
 * a4/a5/a6 and models fmod via 128-bit long doubles; the DB prototype confirms the clean 3-argument
 * form, and both fmod divisors (rendered `a6`/`v15`) are the tread period at definition+784. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/powered_mass_point_datum.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
extern double fmod(double x, double y);
extern void physics_update(int object_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points, const real_vector3d *magic_force, const real_vector3d *magic_torque);

void update_human_tank_physics(int vehicle_index, powered_mass_point_datum *powered_mass_points,
                               mass_point_datum *mass_points)
{
    vehicle_datum *vehicle = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);

    float left_track = vehicle->vehicle.speed - vehicle->vehicle.turn;
    float right_track = vehicle->vehicle.turn + vehicle->vehicle.speed;

    vehicle_definition *definition = TAG_GET(vehicle_definition, *(int *)vehicle);
    /* physics_model is the referenced physics tag blob; +104 (model id) has no clean DB member. */
    physics_definition *physics = TAG_GET(physics_definition, definition->object.physics.index);
    float tread_period = definition->vehicle.wheel_circumference;

    float left_tread = vehicle->vehicle.left_tread + left_track;
    vehicle->vehicle.left_tread = left_tread;
    left_tread = fmod(left_tread, tread_period);
    vehicle->vehicle.left_tread = left_tread;
    if (left_tread < 0.0f)
        vehicle->vehicle.left_tread = tread_period + left_tread;

    float right_tread = vehicle->vehicle.right_tread + right_track;
    vehicle->vehicle.right_tread = right_tread;
    right_tread = fmod(right_tread, tread_period);
    vehicle->vehicle.right_tread = right_tread;
    if (right_tread < 0.0f)
        vehicle->vehicle.right_tread = tread_period + right_tread;

    powered_mass_point_datum *powered = nullptr;
    if (physics->powered_mass_points.count == 2)
    {
        powered_mass_points->ground_friction_velocity = left_track;
        powered_mass_points->rotation.n[0] = 0.0f;
        powered_mass_points->rotation.n[1] = 0.0f;
        powered_mass_points->rotation.n[2] = 0.0f;
        powered_mass_points->rotation.n[3] = 1.0f;

        powered_mass_points[1].ground_friction_velocity = right_track;
        powered_mass_points[1].rotation.n[0] = 0.0f;
        powered_mass_points[1].rotation.n[1] = 0.0f;
        powered_mass_points[1].rotation.n[2] = 0.0f;
        powered_mass_points[1].rotation.n[3] = 1.0f;

        powered = powered_mass_points;
    }

    physics_update(vehicle_index, powered, mass_points, nullptr, nullptr);
}
