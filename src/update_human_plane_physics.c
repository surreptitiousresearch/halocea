/* update_human_plane_physics @0x83761080 — flight-physics updater for the human plane (Pelican). Same shape
 * as update_alien_fighter_physics_old (lift+throttle magic force along up/forward, bank via yaw_vectors, and an
 * axis-angle magic torque toward the target orientation built from control direction + banked lift reference),
 * with extra aircraft controls: a flap lift factor and an integrated pitch-trim term. When the vehicle is
 * grounded (flags bit 0x2 at object+1228) the mass-point array is just cleared. Always spawns the pelican
 * effect at the end.
 *
 * DEVIATIONS:
 *   - DB prototype names the 2nd parameter `powered_mass_points`, but it is the plain mass-point array: it is
 *     memset when grounded and passed to physics_update as the *mass_points* argument (with powered = NULL).
 *     Retyped to mass_point_datum* to match the body. The DB's 3rd parameter is a phantom (disasm 2026-07-31:
 *     r5 never read); dropped so the definition matches the caller's 2-arg extern.
 *   - All the (float)(...) casts are single-precision intermediates; the double temporaries and the
 *     bank-angle / quaternion-angle double-splits are decompiler fcfid artifacts. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_quaternion.h"
#include "headers/mass_point_datum.h"
#include "headers/physics_definition.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/vehicle_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"

#include "headers/powered_mass_point_datum.h"
extern float normalize3d(real_vector3d *v);
extern void yaw_vectors(real_vector3d *forward, const real_vector3d *up, float sine, float cosine);
extern void matrix4x3_rotation_from_vectors(real_matrix4x3 *matrix, const real_vector3d *forward, const real_vector3d *up);
extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);
extern void matrix4x3_rotation_to_quaternion(const real_matrix4x3 *matrix, real_quaternion *quaternion);
extern void quaternion_to_angle_and_vector(const real_quaternion *q, float *angle, real_vector3d *axis);
extern void physics_update(int object_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points, const real_vector3d *magic_force, const real_vector3d *magic_torque);
extern void create_pelican_effect(int vehicle_index);
extern double __fabs(double x);
extern float fabsf(float x);  /* DEVIATION: fabs @0x837612B0 feeds fmadds with no frsp - single-precision abs; @0x8376130C is fdiv+frsp and stays __fabs */
extern double sin(double x);
extern double cos(double x);

void update_human_plane_physics(int vehicle_index, mass_point_datum *mass_points)
{
    vehicle_datum *vehicle = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
    int16_t flags = vehicle->vehicle.flags;
    vehicle_definition *definition = TAG_GET(vehicle_definition, *(int *)vehicle);
    /* physics_model is the referenced physics tag blob; +0/+8 (scale factors) and +116 (mass-point count)
     * have no clean DB member — faithful cast-indexed reads. */
    physics_definition *physics = TAG_GET(physics_definition, definition->object.physics.index);

    if (flags & (1u << _vehicle_hovering_bit))
    {
        /* grounded/hovering: just clear the mass points */
        memset(mass_points, 0, sizeof(mass_point_datum) * physics->mass_points.count);
    }
    else
    {
        float max_speed = definition->vehicle.speed.maximum_forward_speed;
        float lift_scale = physics->mass;
        float antigrav = vehicle->unit.seat_power[0];

        /* throttle (clamped to [0, max_speed]) and its normalized fraction */
        float throttle = vehicle->vehicle.speed;
        if (throttle >= 0.0f)
        {
            if (throttle > max_speed)
                throttle = max_speed;
        }
        else
            throttle = 0.0f;
        float throttle_fraction = throttle / max_speed;
        float throttle_fraction_sq = throttle_fraction * throttle_fraction;

        /* flap lift factor from the control flags */
        float flap_factor = (flags & (1u << _vehicle_control_crouch_bit)) ? 0.25f
                            : ((flags & (1u << _vehicle_control_jump_bit)) ? 1.0f : 0.75f);

        /* integrate the pitch-trim term toward the flap target, rate-limited to +/-0.05 per tick */
        float pitch_trim_delta = (1.0f - throttle_fraction_sq) * antigrav * flap_factor
                                 - vehicle->vehicle.hover;
        if (pitch_trim_delta >= -0.050000001f)
        {
            if (pitch_trim_delta > 0.050000001f)
                pitch_trim_delta = 0.050000001f;
        }
        else
            pitch_trim_delta = -0.050000001f;
        vehicle->vehicle.hover = vehicle->vehicle.hover + pitch_trim_delta;

        /* control direction + a banked lift reference perpendicular to it */
        real_vector3d control_direction;
        control_direction.n[0] = vehicle->unit.desired_facing_vector.n[0];
        control_direction.n[1] = vehicle->unit.desired_facing_vector.n[1];
        control_direction.n[2] = vehicle->unit.desired_facing_vector.n[2];

        vehicle->vehicle.thrust = antigrav * throttle_fraction_sq;

        real_vector3d lift_direction;
        lift_direction.n[2] = -((control_direction.n[2] * control_direction.n[2]) - 1.0f);
        lift_direction.n[1] = -(control_direction.n[1] * control_direction.n[2]);
        lift_direction.n[0] = -(control_direction.n[0] * control_direction.n[2]);
        if (normalize3d(&lift_direction) == 0.0f)
        {
            lift_direction.n[0] = 1.0f;
            lift_direction.n[1] = 0.0f;
            lift_direction.n[2] = 0.0f;
        }

        /* forward speed = dot(linear velocity, forward axis) */
        float forward_speed = vehicle->object.translational_velocity.n[0] * vehicle->object.forward.n[0]
                              + (vehicle->object.translational_velocity.n[2] * vehicle->object.forward.n[2]
                                 + vehicle->object.translational_velocity.n[1] * vehicle->object.forward.n[1]);
        float speed_fraction = fabsf(forward_speed / max_speed);
        float throttle_force = ((vehicle->vehicle.speed - forward_speed) * lift_scale
                                * vehicle->vehicle.thrust) * 0.050000001f;
        float lift = ((speed_fraction * 1.05f + vehicle->vehicle.hover * 1.3f) * lift_scale) * global_gravity;

        /* magic linear force = lift*up + throttle_force*forward */
        real_vector3d force;
        force.n[0] = vehicle->object.up.n[0] * lift + vehicle->object.forward.n[0] * throttle_force;
        force.n[1] = vehicle->object.up.n[1] * lift + vehicle->object.forward.n[1] * throttle_force;
        force.n[2] = vehicle->object.up.n[2] * lift + vehicle->object.forward.n[2] * throttle_force;

        /* roll/bank angle from the lateral velocity relative to the control direction */
        float bank_angle = ((vehicle->object.translational_velocity.n[1] * control_direction.n[0]
                             - vehicle->object.translational_velocity.n[0] * control_direction.n[1]) * 1.5707964f)
                           / __fabs(max_speed);
        yaw_vectors(&lift_direction, &control_direction, (float)sin((double)bank_angle),
                    (float)cos((double)bank_angle));

        /* relative rotation toward the target orientation (control direction + lift reference) */
        real_matrix4x3 current_orientation;
        matrix4x3_rotation_from_vectors(&current_orientation, &vehicle->object.forward,
                                        &vehicle->object.up);
        real_matrix4x3 target_orientation;
        matrix4x3_rotation_from_vectors(&target_orientation, &control_direction, &lift_direction);
        matrix4x3_inverse(&target_orientation, &target_orientation);
        real_matrix4x3 relative_rotation;
        matrix4x3_multiply(&current_orientation, &target_orientation, &relative_rotation);
        real_quaternion relative_quaternion;
        matrix4x3_rotation_to_quaternion(&relative_rotation, &relative_quaternion);
        float rotation_angle;
        real_vector3d rotation_axis;
        quaternion_to_angle_and_vector(&relative_quaternion, &rotation_angle, &rotation_axis);

        /* magic torque drives the axis-angle error against the current angular velocity */
        float angular_scale = (physics->radius * physics->radius * lift_scale) * 0.050000001f;
        float angle_factor = rotation_angle * SECONDS_PER_TICK;
        real_vector3d torque;
        torque.n[0] = (rotation_axis.n[0] * angle_factor - vehicle->object.angular_velocity.n[0]) * angular_scale;
        torque.n[1] = (rotation_axis.n[1] * angle_factor - vehicle->object.angular_velocity.n[1]) * angular_scale;
        torque.n[2] = (rotation_axis.n[2] * angle_factor - vehicle->object.angular_velocity.n[2]) * angular_scale;

        /* scale force and torque by the antigrav fraction */
        force.n[0] = antigrav * force.n[0];
        force.n[1] = antigrav * force.n[1];
        force.n[2] = antigrav * force.n[2];
        torque.n[0] = antigrav * torque.n[0];
        torque.n[1] = antigrav * torque.n[1];
        torque.n[2] = antigrav * torque.n[2];

        physics_update(vehicle_index, nullptr, mass_points, &force, &torque);
    }

    create_pelican_effect(vehicle_index);
}
