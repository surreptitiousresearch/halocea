/* update_human_jeep_physics @0x8375E858 — per-tick physics for a human jeep/warthog. Advances and wraps
 * the (front-wheel) steering angle into [0,period), and when the vehicle's physics model is the powered
 * 4-wheel type (model id 2) builds the two front wheels' steering rotation quaternions (w=cos, z=±sin of
 * the half steering angle) before handing off to the shared physics_update.
 *
 * DEVIATION: the decompiler invents phantom double/long-double args a4/a5/a6 and models the fmod/cos/sin
 * results as 128-bit long doubles; the DB prototype confirms the clean 3-argument form, and the fmod
 * divisor (rendered `a6`) is the steering period at definition+784, per the disassembly. */

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
extern double cos(double x);
extern double sin(double x);
extern void physics_update(int object_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points, const real_vector3d *magic_force, const real_vector3d *magic_torque);

void update_human_jeep_physics(int vehicle_index, powered_mass_point_datum *powered_mass_points,
                               mass_point_datum *mass_points)
{
    vehicle_datum *vehicle = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);

    vehicle_definition *definition = TAG_GET(vehicle_definition, *(int *)vehicle);
    /* physics_model is the referenced physics tag blob; +104 (model id) has no clean DB member. */
    physics_definition *physics = TAG_GET(physics_definition, definition->object.physics.index);

    float new_steering = vehicle->vehicle.speed + vehicle->vehicle.wheel;
    vehicle->vehicle.wheel = new_steering;
    float steering_period = definition->vehicle.wheel_circumference;
    float wrapped = fmod(new_steering, steering_period);
    vehicle->vehicle.wheel = wrapped;
    if (wrapped < 0.0f)
        vehicle->vehicle.wheel = steering_period + wrapped;

    powered_mass_point_datum *powered = nullptr;
    if (physics->powered_mass_points.count == 2)
    {
        float half_angle = vehicle->vehicle.turn * 0.5f;
        float cos_half = cos(half_angle);
        float sin_half = sin(half_angle);

        powered_mass_points->ground_friction_velocity = vehicle->vehicle.speed;
        powered_mass_points->rotation.n[1] = 0.0f;
        powered_mass_points->rotation.n[0] = 0.0f;
        powered_mass_points->rotation.n[2] = sin_half;
        powered_mass_points->rotation.n[3] = cos_half;

        powered_mass_points[1].ground_friction_velocity = vehicle->vehicle.speed;
        powered_mass_points[1].rotation.n[0] = 0.0f;
        powered_mass_points[1].rotation.n[1] = 0.0f;
        powered_mass_points[1].rotation.n[2] = -sin_half;
        powered_mass_points[1].rotation.n[3] = cos_half;

        powered = powered_mass_points;
    }

    physics_update(vehicle_index, powered, mass_points, nullptr, nullptr);
}
