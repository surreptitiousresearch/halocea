/* update_alien_fighter_physics_new @0x8375ED40 — updated flight-physics updater for the alien fighter
 * (banshee): a "new" sibling of update_alien_fighter_physics_old.c using 3x3 rotation matrices and
 * compute_acceleration (rather than 4x3 affine matrices and manual throttle/lift math). Falls back to a
 * plain physics_update when the physics model is not a powered ("alien") flight model (physics_model+104
 * != 2), matching the old version's fallback exactly.
 *
 * Magic force: compute_acceleration() steers the vehicle's current velocity (vehicle+0x68) toward a desired
 * velocity (forward axis scaled by the vehicle's throttle input at +0x4D4), clamped by
 * definition+0x300/+0x304 (scaled by +0x2F8/+0x2FC depending on throttle sign); the result is scaled by the
 * physics model's lift_scale (+8) and the vehicle's antigrav_fraction (+0x338).
 *
 * Magic torque: builds a target orientation from the vehicle's control-direction input (+0x224..+0x22C)
 * refined by a lift-reference vector (control direction crossed against global up, pitched by the
 * definition's pitch-input field +868 when not AI-driven, then yawed by a bank angle derived from lateral
 * velocity), takes the relative rotation from the current forward/up frame to that target as an axis-angle,
 * and scales the axis by (angle * definition+788 / pi) minus the current angular velocity (+140/+144/+148),
 * scaled by the physics model's averaged moment of inertia (average of +80/+84/+88).
 *
 * A separate, unrelated tail computation smooths a per-vehicle stored spin fraction (vehicle+1264) toward
 * the vehicle's actual angular-velocity-over-max-angular-velocity ratio, clamped to a per-frame delta that
 * itself scales with how far vehicle+1264 already is from 0/1 — reproduced as decompiled/disassembled
 * without further interpretation of the exact tuning intent.
 *
 * DEVIATION: the decompiler's own displayed locals `v7` (`real_vector3d *v7 = *(real_vector3d **)v6;`) and
 * `v53.v.n[0..2]` (read before v53 is ever written as a quaternion) are both confirmed via disasm to be
 * stack-slot/register mislabeling, not real values: `v7` is never actually used as a distinct pointer (the
 * disasm shows the corresponding register is simply the vehicle's own definition_index, read for the
 * TAG_INSTANCE lookup that produces `vehicle_definition` below); and the reads decompiled as `v53.v.n[i]`
 * are the freshly-computed `compute_acceleration()` output occupying the same stack slot v53 (a
 * `real_quaternion`) is later reused for — reproduced here as a dedicated `computed_acceleration` local. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix3x3.h"
#include "headers/real_quaternion.h"
#include "headers/powered_mass_point_datum.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/blam_data_globals.h"


extern float normalize3d(real_vector3d *v);
extern real_vector3d *compute_acceleration(const real_vector3d *current_velocity, const real_vector3d *desired_velocity, float maximum_acceleration, float maximum_deceleration, real_vector3d *acceleration);
extern real_matrix3x3 *matrix3x3_from_forward_and_up(real_matrix3x3 *matrix, const real_vector3d *forward, const real_vector3d *up);
extern uint8_t unit_driven_by_ai(int unit_index);
extern void pitch_vectors(real_vector3d *forward, real_vector3d *up, float sine, float cosine);
extern void yaw_vectors(real_vector3d *forward, const real_vector3d *up, float sine, float cosine);
extern real_matrix3x3 * matrix3x3_transpose(const real_matrix3x3 *matrix, real_matrix3x3 *result);
extern real_matrix3x3 *matrix3x3_multiply(const real_matrix3x3 *a, const real_matrix3x3 *b, real_matrix3x3 *result);
extern real_quaternion *matrix3x3_rotation_to_quaternion(const real_matrix3x3 *matrix, real_quaternion *quaternion);
extern void quaternion_to_angle_and_vector(const real_quaternion *q, float *angle, real_vector3d *axis);
extern void physics_update(int object_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points, const real_vector3d *magic_force, const real_vector3d *magic_torque);
extern double cos(double x);
extern double sin(double x);
extern float __fsqrts(float x);

void update_alien_fighter_physics_new(int vehicle_index, powered_mass_point_datum *powered_mass_points,
                                      mass_point_datum *mass_points)
{
    vehicle_datum *vehicle = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
    vehicle_definition *definition = TAG_GET(vehicle_definition, *(int *)vehicle);
    /* physics_model is the referenced physics tag blob; +8/+80/+84/+88/+104/+776 (scale factors, model id,
     * moment-of-inertia components) have no clean DB member — faithful cast-indexed reads. */
    physics_definition *physics = TAG_GET(physics_definition, definition->object.physics.index);

    const real_vector3d *force = 0;
    const real_vector3d *torque = 0;
    powered_mass_point_datum *updated_mass_points = 0;
    real_vector3d scaled_force;
    real_vector3d scaled_torque;

    if (physics->powered_mass_points.count == 2)
    {
        float throttle_input = vehicle->vehicle.speed;

        real_vector3d desired_velocity;
        desired_velocity.n[0] = vehicle->object.forward.n[0] * throttle_input;
        desired_velocity.n[1] = vehicle->object.forward.n[1] * throttle_input;
        desired_velocity.n[2] = vehicle->object.forward.n[2] * throttle_input;

        float acceleration_scale = (throttle_input <= 0.0f)
                                  ? -(throttle_input / definition->vehicle.speed.maximum_reverse_speed)
                                  : (throttle_input / definition->vehicle.speed.maximum_forward_speed);

        real_vector3d computed_acceleration;
        compute_acceleration(&vehicle->object.translational_velocity, &desired_velocity,
                             definition->vehicle.speed.acceleration * acceleration_scale,
                             definition->vehicle.speed.deceleration * acceleration_scale, &computed_acceleration);

        float lift_scale = physics->mass;
        float antigrav_fraction = vehicle->unit.seat_power[0];

        scaled_force.n[0] = computed_acceleration.n[0] * lift_scale * antigrav_fraction;
        scaled_force.n[1] = computed_acceleration.n[1] * lift_scale * antigrav_fraction;
        scaled_force.n[2] = computed_acceleration.n[2] * lift_scale * antigrav_fraction;

        /* target orientation: control-direction input, refined by a lift reference (control direction
         * crossed toward global up), pitched (if not AI-driven) and yawed toward the current motion */
        real_matrix3x3 target_frame;
        target_frame.forward.n[0] = vehicle->unit.desired_facing_vector.n[0];
        target_frame.forward.n[1] = vehicle->unit.desired_facing_vector.n[1];
        target_frame.forward.n[2] = vehicle->unit.desired_facing_vector.n[2];

        float negated_z = -target_frame.forward.n[2];
        target_frame.up.n[0] = target_frame.forward.n[0] * negated_z + global_up3d->n[0];
        target_frame.up.n[1] = target_frame.forward.n[1] * negated_z + global_up3d->n[1];
        target_frame.up.n[2] = target_frame.forward.n[2] * negated_z + global_up3d->n[2];
        if (normalize3d(&target_frame.up) == 0.0f)
        {
            target_frame.up.n[0] = global_forward3d->n[0];
            target_frame.up.n[1] = global_forward3d->n[1];
            target_frame.up.n[2] = global_forward3d->n[2];
        }

        if (!unit_driven_by_ai(vehicle_index))
        {
            float pitch_input = definition->vehicle.fixed_gun_pitch;
            pitch_vectors(&target_frame.forward, &target_frame.up, (float)sin(pitch_input), (float)cos(pitch_input));
        }

        /* RAW: +776 is past the 128-byte physics_definition (res.py: no member) — a banking scale */
        const float *banking_scale_ptr = (const float *)((char *)physics + 776);
        float bank_angle = ((vehicle->object.translational_velocity.n[1] * target_frame.forward.n[0]
                            - vehicle->object.translational_velocity.n[0] * target_frame.forward.n[1])
                           / definition->vehicle.speed.maximum_forward_speed) * *banking_scale_ptr;
        yaw_vectors(&target_frame.up, &target_frame.forward, (float)sin(bank_angle), (float)cos(bank_angle));

        /* relative rotation from the current forward/up frame to the target frame */
        real_matrix3x3 current_frame;
        matrix3x3_from_forward_and_up(&current_frame, &vehicle->object.forward,
                                      &vehicle->object.up);

        /* target_frame.left is re-derived here as forward x up (matching the original's direct field
         * arithmetic rather than a cross-product helper call) */
        target_frame.left.n[1] = target_frame.up.n[2] * target_frame.forward.n[0]
                                - target_frame.up.n[0] * target_frame.forward.n[2];
        target_frame.left.n[0] = target_frame.up.n[1] * target_frame.forward.n[2]
                                - target_frame.up.n[2] * target_frame.forward.n[1];
        target_frame.left.n[2] = target_frame.forward.n[1] * target_frame.up.n[0]
                                - target_frame.up.n[1] * target_frame.forward.n[0];

        /* current_frame is transposed in place, then relative_rotation = target_frame * transpose(current_frame)
         * (note the multiply order: target_frame first, not current_frame — matches disasm exactly) */
        matrix3x3_transpose(&current_frame, &current_frame);
        real_matrix3x3 relative_rotation;
        matrix3x3_multiply(&target_frame, &current_frame, &relative_rotation);
        real_quaternion relative_quaternion;
        matrix3x3_rotation_to_quaternion(&relative_rotation, &relative_quaternion);
        float rotation_angle;
        real_vector3d rotation_axis;
        quaternion_to_angle_and_vector(&relative_quaternion, &rotation_angle, &rotation_axis);

        float inertia_scale = (physics->zz_moment + physics->yy_moment
                              + physics->xx_moment) * 0.33333334f;
        float axis_scale = definition->vehicle.turn_rate * -rotation_angle * 0.31830987f;

        scaled_torque.n[0] = (rotation_axis.n[0] * axis_scale - vehicle->object.angular_velocity.n[0]) * inertia_scale;
        scaled_torque.n[1] = (rotation_axis.n[1] * axis_scale - vehicle->object.angular_velocity.n[1]) * inertia_scale;
        scaled_torque.n[2] = (rotation_axis.n[2] * axis_scale - vehicle->object.angular_velocity.n[2]) * inertia_scale;

        /* unrelated tail computation: smooth vehicle+1264's stored spin fraction toward
         * (angular velocity magnitude / definition+788), clamped by a delta that itself scales with how
         * close vehicle+1264 already is to 0 or 1 */
        float angular_speed = __fsqrts(vehicle->object.angular_velocity.n[0] * vehicle->object.angular_velocity.n[0]
                                      + (vehicle->object.angular_velocity.n[1] * vehicle->object.angular_velocity.n[1]
                                         + vehicle->object.angular_velocity.n[2] * vehicle->object.angular_velocity.n[2]));
        float spin_fraction = vehicle->vehicle.thrust;
        float speed_ratio = angular_speed / definition->vehicle.turn_rate;
        float delta = speed_ratio - spin_fraction;
        float clamp;

        if (speed_ratio <= spin_fraction)
        {
            clamp = (spin_fraction * spin_fraction) * 0.050000001f;
            if (clamp <= 0.0049999999f)
                clamp = 0.0049999999f;
            if (delta <= -clamp)
                delta = -clamp;
        }
        else
        {
            clamp = 0.0099999998f;
            if (((1.0f - spin_fraction) * (1.0f - spin_fraction)) * 0.2f >= 0.0099999998f)
            {
                clamp = 0.050000001f;
                if (((1.0f - spin_fraction) * (1.0f - spin_fraction)) * 0.2f <= 0.050000001f)
                    clamp = ((1.0f - spin_fraction) * (1.0f - spin_fraction)) * 0.2f;
            }
            if (delta > clamp)
                delta = clamp;
        }
        vehicle->vehicle.thrust = spin_fraction + delta;

        powered_mass_points->antigrav_fraction = antigrav_fraction;
        powered_mass_points->rotation = *global_identity_quaternion;
        powered_mass_points[1].antigrav_fraction = antigrav_fraction;
        powered_mass_points[1].rotation = *global_identity_quaternion;

        force = &scaled_force;
        torque = &scaled_torque;
        updated_mass_points = powered_mass_points;
    }

    physics_update(vehicle_index, updated_mass_points, mass_points, force, torque);
}
