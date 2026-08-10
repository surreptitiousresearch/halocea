/* update_human_boat_physics @0x8375E9A0 — physics updater for the human boat. Sets up the three powered
 * mass points (water friction/lift, plus a speed-attenuated steering rotation on the first), then builds a
 * self-righting roll torque that rotates the boat's reference up vector back toward its body up axis. Falls
 * back to a plain physics_update when the physics model is not a water model (physics_model[104] != 3).
 *
 * Same clean 3-arg prototype as the other vehicle-physics updaters; the phantom trailing `long double`
 * parameter and the double temporaries are decompiler artifacts of the single-precision math. real_quaternion
 * is n[4] (n[3]=scalar). cos/sin return double, narrowed to float on store.
 *
 * DEVIATION: the decompiler rendered the roll-direction sign as `HIDWORD(v28) = "neoffire_crouching"` — the
 * high half of a register that coincidentally held a string address; the value used is just the sign of the
 * signed angle (-1 / 0 / +1). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/powered_mass_point_datum.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/blam_data_globals.h"
extern double cos(double x);
extern double sin(double x);


extern float normalize3d(real_vector3d *v);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern float angle_between_vectors3d(const real_vector3d *a, const real_vector3d *b);
extern void physics_update(int object_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points, const real_vector3d *magic_force, const real_vector3d *magic_torque);
extern float __fsqrts(float x);
extern double __fabs(double x);

void update_human_boat_physics(int vehicle_index, powered_mass_point_datum *powered_mass_points,
                               mass_point_datum *mass_points)
{
    vehicle_datum *vehicle = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
    vehicle_definition *definition = TAG_GET(vehicle_definition, *(int *)vehicle);
    /* physics_model is the referenced physics tag blob; +104 (model id), +80 (torque scale) have no clean DB member. */
    physics_definition *physics = TAG_GET(physics_definition, definition->object.physics.index);

    if (physics->powered_mass_points.count != 3)
    {
        physics_update(vehicle_index, nullptr, mass_points, nullptr, nullptr);
        return;
    }

    float half_steer = vehicle->vehicle.turn * 0.5f;

    /* speed (clamped to 1) attenuates the steering rotation */
    float speed = __fabs(__fsqrts(vehicle->object.translational_velocity.n[2] * vehicle->object.translational_velocity.n[2]
                                  + (vehicle->object.translational_velocity.n[0] * vehicle->object.translational_velocity.n[0]
                                     + vehicle->object.translational_velocity.n[1] * vehicle->object.translational_velocity.n[1]))
                         * 2.5f);
    if (speed > 1.0f)
        speed = 1.0f;

    powered_mass_points->water_friction_velocity = vehicle->vehicle.speed;
    powered_mass_points->water_lift_ratio = 0.003f;

    /* first mass point: steering rotation about z, reduced at speed */
    float steer_angle = (1.0f - speed) * half_steer;
    powered_mass_points->rotation.n[2] = (float)sin((double)steer_angle);
    powered_mass_points->rotation.n[3] = (float)cos((double)steer_angle);
    powered_mass_points->rotation.n[0] = 0.0f;
    powered_mass_points->rotation.n[1] = 0.0f;

    powered_mass_points[1].water_lift_ratio = 0.003f;
    powered_mass_points[1].rotation.n[0] = 0.0f;
    powered_mass_points[1].rotation.n[1] = 0.0f;
    powered_mass_points[1].rotation.n[2] = 0.0f;
    powered_mass_points[1].rotation.n[3] = 1.0f;

    powered_mass_points[2].water_lift_ratio = 0.0049999999f;
    powered_mass_points[2].rotation.n[0] = 0.0f;
    powered_mass_points[2].rotation.n[1] = 0.0f;
    powered_mass_points[2].rotation.n[2] = 0.0f;
    powered_mass_points[2].rotation.n[3] = 1.0f;

    const real_vector3d *world_up = global_up3d;
    const float *forward = vehicle->object.forward.n;

    real_vector3d magic_force;
    magic_force.n[0] = 0.0f;
    magic_force.n[1] = 0.0f;
    magic_force.n[2] = 0.0f;

    /* reference up = world up with the forward-axis projection removed */
    float forward_z = vehicle->object.forward.n[2];
    real_vector3d reference_up;
    reference_up.n[0] = forward[0] * -forward_z + world_up->n[0];
    reference_up.n[1] = -forward_z * forward[1] + world_up->n[1];
    reference_up.n[2] = -forward_z * forward[2] + world_up->n[2];

    real_vector3d magic_torque;
    if (normalize3d(&reference_up) == 0.0f)
    {
        magic_torque.n[0] = 0.0f;
        magic_torque.n[1] = 0.0f;
        magic_torque.n[2] = 0.0f;
    }
    else
    {
        /* body-up x forward components, used to sign the roll angle below */
        float sign_x = vehicle->object.up.n[0] * vehicle->object.forward.n[1]
                       - vehicle->object.up.n[1] * vehicle->object.forward.n[0];
        float sign_a = vehicle->object.forward.n[2] * vehicle->object.up.n[1]
                       - vehicle->object.forward.n[1] * vehicle->object.up.n[2];
        float sign_b = vehicle->object.up.n[2] * vehicle->object.forward.n[0]
                       - vehicle->object.up.n[0] * vehicle->object.forward.n[2];

        const float *linear_velocity = vehicle->object.translational_velocity.n;
        float twist = (world_up->n[0] * (linear_velocity[2] * forward[1] - linear_velocity[1] * forward[2])
                       + (world_up->n[1] * (forward[2] * linear_velocity[0] - linear_velocity[2] * forward[0])
                          + world_up->n[2] * (linear_velocity[1] * forward[0] - forward[1] * linear_velocity[0])))
                      * 6.2831855f;
        rotate_vector_about_axis(&reference_up, &vehicle->object.forward,
                                 (float)sin((double)twist), (float)cos((double)twist));

        float roll_angle = angle_between_vectors3d(&reference_up, &vehicle->object.up);
        if (sign_a * reference_up.n[0] + (sign_x * reference_up.n[2] + sign_b * reference_up.n[1]) > 0.0f)
            roll_angle = -roll_angle;

        int roll_sign = roll_angle == 0.0f ? 0 : (roll_angle >= 0.0f ? 1 : -1);

        /* corrective roll rate vs current angular velocity along forward, clamped to ~+/-0.8 deg */
        float correction = __fsqrts(__fabs(roll_angle) * 0.02792526967823505) * (float)roll_sign
                           - (forward[0] * vehicle->object.angular_velocity.n[0]
                              + (vehicle->object.angular_velocity.n[2] * forward[2]
                                 + vehicle->object.angular_velocity.n[1] * forward[1]));
        if (correction < -0.013962635f || correction > 0.013962635f)
            correction = correction < -0.013962635f ? -0.013962635f : 0.013962635f;

        float torque_scale = physics->xx_moment * correction;
        magic_torque.n[0] = forward[0] * torque_scale;
        magic_torque.n[1] = torque_scale * forward[1];
        magic_torque.n[2] = torque_scale * forward[2];
    }

    physics_update(vehicle_index, powered_mass_points, mass_points, &magic_force, &magic_torque);
}
