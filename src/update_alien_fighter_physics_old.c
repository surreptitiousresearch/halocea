/* update_alien_fighter_physics_old @0x8375F228 — legacy flight-physics updater for the alien fighter
 * (banshee). Builds the magic linear force and torque that steer the vehicle toward the pilot's control
 * direction, then hands them to physics_update. The lift/throttle force pushes along the vehicle up and
 * forward axes; the torque rotates the current orientation toward a target frame (control direction + a
 * banked lift reference), expressed as an axis-angle from the relative-rotation quaternion. Both are scaled
 * by the vehicle's antigrav fraction. Falls back to a plain physics_update when the physics model is not a
 * powered ("alien") flight model (physics_model[104] != 2).
 *
 * Same clean 3-arg prototype as the other vehicle-physics updaters; the decompiler invents phantom double
 * temporaries from the single-precision math — all (float)(...) casts are intermediates and are preserved.
 * cos/sin return double and are narrowed to float. real_quaternion is { real_vector3d v; float w; }. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_quaternion.h"
#include "headers/powered_mass_point_datum.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/blam_data_globals.h"
extern double cos(double x);
extern double sin(double x);


extern float normalize3d(real_vector3d *v);
extern void yaw_vectors(real_vector3d *forward, const real_vector3d *up, float sine, float cosine);
extern void matrix4x3_rotation_from_vectors(real_matrix4x3 *matrix, const real_vector3d *forward, const real_vector3d *up);
extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);
extern void matrix4x3_rotation_to_quaternion(const real_matrix4x3 *matrix, real_quaternion *quaternion);
extern void quaternion_to_angle_and_vector(const real_quaternion *q, float *angle, real_vector3d *axis);
extern void physics_update(int object_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points, const real_vector3d *magic_force, const real_vector3d *magic_torque);
extern float __fabs(float x);

void update_alien_fighter_physics_old(int vehicle_index, powered_mass_point_datum *powered_mass_points,
                                      mass_point_datum *mass_points)
{
    vehicle_datum *vehicle = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
    vehicle_definition *definition = TAG_GET(vehicle_definition, *(int *)vehicle);
    /* physics_model is the referenced physics tag blob; +0/+8 (scale factors) and +104 (model id) have no
     * clean DB member — faithful cast-indexed reads. */
    physics_definition *physics = TAG_GET(physics_definition, definition->object.physics.index);

    if (physics->powered_mass_points.count != 2)
    {
        physics_update(vehicle_index, nullptr, mass_points, nullptr, nullptr);
        return;
    }

    /* pilot control direction */
    real_vector3d control_direction;
    control_direction.n[0] = vehicle->unit.desired_facing_vector.n[0];
    control_direction.n[1] = vehicle->unit.desired_facing_vector.n[1];
    control_direction.n[2] = vehicle->unit.desired_facing_vector.n[2];

    /* banked lift reference: a vector perpendicular to the control direction (fallback +x if degenerate) */
    real_vector3d lift_direction;
    lift_direction.n[0] = -(control_direction.n[0] * control_direction.n[2]);
    lift_direction.n[1] = -(control_direction.n[1] * control_direction.n[2]);
    lift_direction.n[2] = -((control_direction.n[2] * control_direction.n[2]) - 1.0f);
    if (normalize3d(&lift_direction) == 0.0f)
    {
        lift_direction.n[0] = 1.0f;
        lift_direction.n[1] = 0.0f;
        lift_direction.n[2] = 0.0f;
    }

    float max_speed = definition->vehicle.speed.maximum_forward_speed;
    float lift_scale = physics->mass;

    /* forward speed = dot(linear velocity, forward axis) */
    float forward_speed = vehicle->object.translational_velocity.n[0] * vehicle->object.forward.n[0]
                          + (vehicle->object.translational_velocity.n[2] * vehicle->object.forward.n[2]
                             + vehicle->object.translational_velocity.n[1] * vehicle->object.forward.n[1]);
    float speed_fraction = __fabs(forward_speed / max_speed);
    float throttle = (vehicle->vehicle.speed - forward_speed) * lift_scale * 0.050000001f;
    float lift = speed_fraction * lift_scale * global_gravity * 1.05f;

    /* magic linear force = lift*up + throttle*forward */
    real_vector3d force;
    force.n[0] = lift * vehicle->object.up.n[0] + throttle * vehicle->object.forward.n[0];
    force.n[1] = vehicle->object.up.n[1] * lift + vehicle->object.forward.n[1] * throttle;
    force.n[2] = vehicle->object.up.n[2] * lift + vehicle->object.forward.n[2] * throttle;

    /* roll/bank angle from the lateral velocity relative to the control direction */
    float bank_angle = ((vehicle->object.translational_velocity.n[1] * control_direction.n[0]
                         - vehicle->object.translational_velocity.n[0] * control_direction.n[1]) * 1.5707964f)
                       / __fabs(max_speed);
    yaw_vectors(&lift_direction, &control_direction, (float)sin((double)bank_angle),
                (float)cos((double)bank_angle));

    /* relative rotation from the current orientation to the target (control direction + lift reference) */
    real_matrix4x3 current_orientation;
    matrix4x3_rotation_from_vectors(&current_orientation, &vehicle->object.forward,
                                    &vehicle->object.up);
    real_matrix4x3 target_orientation;
    matrix4x3_rotation_from_vectors(&target_orientation, &control_direction, &lift_direction);
    matrix4x3_inverse(&target_orientation, &target_orientation);
    real_matrix4x3 relative_rotation[2];
    matrix4x3_multiply(&current_orientation, &target_orientation, relative_rotation);
    real_quaternion relative_quaternion;
    matrix4x3_rotation_to_quaternion(relative_rotation, &relative_quaternion);
    float rotation_angle;
    real_vector3d rotation_axis;
    quaternion_to_angle_and_vector(&relative_quaternion, &rotation_angle, &rotation_axis);

    /* magic torque drives the axis-angle error against the current angular velocity */
    float angular_scale = (physics->radius * physics->radius * lift_scale) * 0.050000001f;
    float angle_factor = rotation_angle * 0.13333334f;
    real_vector3d torque;
    torque.n[0] = (rotation_axis.n[0] * angle_factor - vehicle->object.angular_velocity.n[0]) * angular_scale;
    torque.n[1] = (rotation_axis.n[1] * angle_factor - vehicle->object.angular_velocity.n[1]) * angular_scale;
    torque.n[2] = (rotation_axis.n[2] * angle_factor - vehicle->object.angular_velocity.n[2]) * angular_scale;

    /* both powered mass points: identity rotation, antigrav from the vehicle's lift fraction */
    float antigrav_fraction = vehicle->unit.seat_power[0];
    powered_mass_points->antigrav_fraction = antigrav_fraction;
    powered_mass_points->rotation.w = 1.0f;
    powered_mass_points->rotation.v.n[0] = 0.0f;
    powered_mass_points->rotation.v.n[1] = 0.0f;
    powered_mass_points->rotation.v.n[2] = 0.0f;
    powered_mass_points[1].antigrav_fraction = antigrav_fraction;
    powered_mass_points[1].rotation.w = 1.0f;
    powered_mass_points[1].rotation.v.n[0] = 0.0f;
    powered_mass_points[1].rotation.v.n[1] = 0.0f;
    powered_mass_points[1].rotation.v.n[2] = 0.0f;

    /* scale the magic force/torque by the antigrav fraction */
    force.n[0] = antigrav_fraction * force.n[0];
    force.n[1] = antigrav_fraction * force.n[1];
    force.n[2] = antigrav_fraction * force.n[2];
    torque.n[0] = antigrav_fraction * torque.n[0];
    torque.n[1] = antigrav_fraction * torque.n[1];
    torque.n[2] = antigrav_fraction * torque.n[2];

    physics_update(vehicle_index, powered_mass_points, mass_points, &force, &torque);
}
