/* physics_update_new @0x837BFC28 — advance a vehicle's rigid body for one tick given the already-combined
 * total force/torque (physics_update, the caller, has already merged the mass-point + magic force/torque
 * and cleared the object's force accumulator). Three stages:
 *
 *  1. Linear/angular integration: v = v + F/mass, p = p + v; the world-space inverse inertia tensor is
 *     rebuilt each tick from the body-space inverse inertia tensor (inertial_matrix tag block entry [1])
 *     via R * I_inv_local * R^T, then used to turn torque into an angular acceleration; angular velocity is
 *     integrated the same way, and the forward/up frame is advanced by rotate_vectors3d_by_angular_velocity.
 *
 *  2. Penetration-freeze correction (skipped entirely under the debug flag): up to 4 passes recompute the
 *     candidate position/frame from the *original* (pre-tick) position/orientation plus the current
 *     (possibly already-corrected) velocity/angular velocity, sweep every physics mass point (from the tag
 *     block, position at +56 of each 128-byte entry) against its cached runtime position, and if any sweep
 *     collides, pick the deepest/earliest one and use its contact normal to pull the velocity back along
 *     the normal and damp the angular velocity by the leftover time fraction. A pass with no collisions at
 *     all breaks out immediately and the resolved position/frame IS committed; but if all 4 passes are
 *     exhausted while still penetrating, position/frame are NOT committed this tick (the actual
 *     "penetration freeze" the debug flag disables) — only the stuck bitmask and the already-corrected
 *     velocity/angular velocity survive.
 *
 *  3. "At rest" flag maintenance: tallies each of 4 independent bits across every runtime mass point's
 *     flags word, and sets/clears bits in the vehicle's own flags based on those tallies plus how small the
 *     final linear/angular velocity and acceleration are.
 *
 * DEVIATIONS (disasm-resolved, 0x837BFC90-0x837C0254):
 *  - `powered_mass_points` is unused by this function's body (only `mass_points` and the tag-side
 *    `physics->mass_points` definition block are read) — kept in the signature since it matches the DB
 *    prototype and the caller (physics_update.c) always supplies it.
 *  - The salted object-header lookup casts straight to `vehicle_datum *`: `stuck_mass_point_flags` lives at
 *    absolute offset 1312, past the end of `unit_datum` (1228 bytes), so this function is only ever called
 *    with a vehicle-shaped object; `vehicle_datum.h`/`mass_point_datum.h`/`vehicle_datum_network_data.h`
 *    were fleshed out this pass from previously-opaque/nonexistent headers via `types_members`.
 *  - The decompiler renders the per-iteration "worst collision" capture as a raw `float v74[20]` +
 *    `memcpy(v74, v75, sizeof(v74))` from a `collision_result v75[2]`; only `v75[0]` is ever read, so this
 *    is reproduced as a single `collision_result worst_collision` with a plain struct copy.
 *  - Both the 0x4 and 0x8 "at rest" flag bits are gated on the SAME tally (`mass_point_flag_bit3_count`) —
 *    confirmed via disasm, not a transcription slip; reproduced as-is (faithful shipped quirk).
 *  - `mass_point_datum.flags` bits 0/1/2/3 have no database-level names; the local tally variables are
 *    named after the bit they count, not an inferred meaning. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/vehicle_datum.h"
#include "headers/object_flags.h"
#include "headers/physics_instance.h"
#include "headers/physics_definition.h"
#include "headers/powered_mass_point_datum.h"
#include "headers/mass_point_datum.h"
#include "headers/mass_point_definition.h"
#include "headers/collision_result.h"
#include "headers/real_matrix3x3.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"


extern real_matrix3x3 *matrix3x3_from_forward_and_up(real_matrix3x3 *matrix, const real_vector3d *forward, const real_vector3d *up);
extern real_matrix3x3 * matrix3x3_transpose(const real_matrix3x3 *matrix, real_matrix3x3 *result);
extern real_matrix3x3 *matrix3x3_multiply(const real_matrix3x3 *a, const real_matrix3x3 *b, real_matrix3x3 *result);
extern real_vector3d *matrix3x3_transform_vector(const real_matrix3x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern void rotate_vectors3d_by_angular_velocity(const real_vector3d *forward, const real_vector3d *up, const real_vector3d *angular_velocity, real_vector3d *rotated_forward, real_vector3d *rotated_up);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern void object_set_position(int object_index, const real_point3d *position, const real_vector3d *forward, const real_vector3d *up);
extern float __fabs(float x);

void physics_update_new(const physics_instance *instance, const powered_mass_point_datum *powered_mass_points,
                        const mass_point_datum *mass_points, const real_vector3d *total_force,
                        const real_vector3d *total_torque)
{
    vehicle_datum *vehicle = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, instance->object_index)->datum);
    const physics_definition *physics = instance->physics;

    real_vector3d linear_acceleration;
    linear_acceleration.n[0] = total_force->n[0] * (1.0f / physics->mass);
    linear_acceleration.n[1] = total_force->n[1] * (1.0f / physics->mass);
    linear_acceleration.n[2] = total_force->n[2] * (1.0f / physics->mass);

    real_vector3d velocity;
    velocity.n[0] = vehicle->object.translational_velocity.n[0] + linear_acceleration.n[0];
    velocity.n[1] = vehicle->object.translational_velocity.n[1] + linear_acceleration.n[1];
    velocity.n[2] = vehicle->object.translational_velocity.n[2] + linear_acceleration.n[2];

    real_point3d position;
    position.n[0] = vehicle->object.position.n[0] + velocity.n[0];
    position.n[1] = vehicle->object.position.n[1] + velocity.n[1];
    position.n[2] = vehicle->object.position.n[2] + velocity.n[2];

    /* world_inverse_inertia = R * I_inv_local * R^T, R = current forward/up frame */
    real_matrix3x3 frame;
    matrix3x3_from_forward_and_up(&frame, &vehicle->object.forward, &vehicle->object.up);
    real_matrix3x3 world_inverse_inertia;
    matrix3x3_multiply(&frame, (const real_matrix3x3 *)physics->inertial_matrix.address + 1, &world_inverse_inertia);
    real_matrix3x3 frame_transpose;
    matrix3x3_transpose(&frame, &frame_transpose);
    matrix3x3_multiply(&world_inverse_inertia, &frame_transpose, &world_inverse_inertia);

    real_vector3d angular_acceleration;
    matrix3x3_transform_vector(&world_inverse_inertia, total_torque, &angular_acceleration);

    real_vector3d angular_velocity;
    angular_velocity.n[0] = vehicle->object.angular_velocity.n[0] + angular_acceleration.n[0];
    angular_velocity.n[1] = vehicle->object.angular_velocity.n[1] + angular_acceleration.n[1];
    angular_velocity.n[2] = vehicle->object.angular_velocity.n[2] + angular_acceleration.n[2];

    real_vector3d forward, up;
    rotate_vectors3d_by_angular_velocity(&vehicle->object.forward, &vehicle->object.up, &angular_velocity,
                                         &forward, &up);

    vehicle->object.translational_velocity = velocity;
    vehicle->object.angular_velocity = angular_velocity;

    unsigned int stuck_mass_point_flags = 0;

    if (debug_physics_disable_penetration_freeze)
    {
        object_set_position(instance->object_index, &position, &forward, &up);
    }
    else
    {
        int16_t passes_remaining = 4;
        unsigned char penetration_unresolved = 0;
        for (;;)
        {
            passes_remaining--;
            unsigned char found_collision = 0;
            real_vector3d worst_delta;
            collision_result worst_collision;

            real_matrix4x3 world_matrix;
            matrix4x3_from_point_and_vectors(&world_matrix, &position, &forward, &up);

            real_point3d center_of_mass_negated;
            center_of_mass_negated.n[0] = -physics->center_of_mass.n[0];
            center_of_mass_negated.n[1] = -physics->center_of_mass.n[1];
            center_of_mass_negated.n[2] = -physics->center_of_mass.n[2];
            real_point3d world_center_of_mass;
            matrix4x3_transform_point(&world_matrix, &center_of_mass_negated, &world_center_of_mass);
            world_matrix.position = world_center_of_mass;

            for (int i = 0; i < physics->mass_points.count; i = (int16_t)(i + 1))
            {
                const mass_point_datum *runtime_mass_point = &mass_points[i];
                const real_point3d *tag_position =
                        &((const mass_point_definition *)physics->mass_points.address)[i].position;

                real_point3d swept_position;
                matrix4x3_transform_point(&world_matrix, tag_position, &swept_position);

                real_vector3d delta;
                delta.n[0] = swept_position.n[0] - runtime_mass_point->position.n[0];
                delta.n[1] = swept_position.n[1] - runtime_mass_point->position.n[1];
                delta.n[2] = swept_position.n[2] - runtime_mass_point->position.n[2];

                collision_result collision;
                if (collision_test_vector(_collision_test_for_vehicles_flags | (1u << _collision_test_front_facing_surfaces_bit), &runtime_mass_point->position, &delta, instance->object_index, &collision))
                {
                    stuck_mass_point_flags |= 1 << i;
                    if (!found_collision || worst_collision.t > collision.t)
                    {
                        found_collision = 1;
                        worst_delta = delta;
                        worst_collision = collision;
                    }
                }
            }

            if (!found_collision)
                break;

            float normal_dot_delta = worst_collision.plane.normal.n[0] * worst_delta.n[0]
                    + worst_collision.plane.normal.n[1] * worst_delta.n[1]
                    + worst_collision.plane.normal.n[2] * worst_delta.n[2];
            float epsilon = (normal_dot_delta == 0.0f) ? 0.03125f : 0.0078125f / __fabs(normal_dot_delta);
            float t = worst_collision.t - epsilon;
            if (t <= 0.0f)
                t = 0.0f;

            float normal_dot_velocity = worst_collision.plane.normal.n[0] * velocity.n[0]
                    + worst_collision.plane.normal.n[1] * velocity.n[1]
                    + worst_collision.plane.normal.n[2] * velocity.n[2];
            if (normal_dot_velocity < 0.0f)
            {
                velocity.n[0] += worst_collision.plane.normal.n[0] * ((t - 1.0f) * normal_dot_velocity);
                velocity.n[1] += worst_collision.plane.normal.n[1] * ((t - 1.0f) * normal_dot_velocity);
                velocity.n[2] += worst_collision.plane.normal.n[2] * ((t - 1.0f) * normal_dot_velocity);

                vehicle->object.translational_velocity = velocity;

                position.n[0] = vehicle->object.position.n[0] + velocity.n[0];
                position.n[1] = vehicle->object.position.n[1] + velocity.n[1];
                position.n[2] = vehicle->object.position.n[2] + velocity.n[2];
            }

            angular_velocity.n[0] *= t;
            angular_velocity.n[1] *= t;
            angular_velocity.n[2] *= t;
            vehicle->object.angular_velocity = angular_velocity;

            rotate_vectors3d_by_angular_velocity(&vehicle->object.forward, &vehicle->object.up, &angular_velocity,
                                                 &forward, &up);

            if (passes_remaining <= 0)
            {
                /* out of correction passes and still penetrating: this is the actual "penetration freeze" —
                 * position/frame are NOT committed this tick (object_set_position below is skipped), though
                 * the corrected velocity/angular velocity above already were. Disasm-confirmed: the exhausted-
                 * passes exit jumps past the object_set_position call, unlike the no-collision-found exit. */
                penetration_unresolved = 1;
                break;
            }
        }

        if (!penetration_unresolved)
            object_set_position(instance->object_index, &position, &forward, &up);
        vehicle->vehicle.stuck_mass_point_flags = stuck_mass_point_flags;
    }

    /* "at rest" bookkeeping: tally 4 independent flag bits across every runtime mass point */
    int16_t mass_point_flag_bit0_count = 0;
    int16_t mass_point_flag_bit1_count = 0;
    int16_t mass_point_flag_bit2_count = 0;
    int16_t mass_point_flag_bit3_count = 0;
    int mass_point_count = physics->mass_points.count;
    for (int i = 0; i < mass_point_count; i = (int16_t)(i + 1))
    {
        unsigned int flags = mass_points[i].flags;
        mass_point_flag_bit0_count += flags & 1;
        mass_point_flag_bit1_count += (flags >> 1) & 1;
        mass_point_flag_bit2_count += (flags >> 2) & 1;
        mass_point_flag_bit3_count += (flags >> 3) & 1;
    }

    unsigned int flags = vehicle->object.flags;
    if (mass_point_flag_bit0_count == mass_point_count
        && mass_point_flag_bit1_count >= 3
        && mass_point_flag_bit2_count == 0
        && (velocity.n[0] * velocity.n[0] + velocity.n[2] * velocity.n[2] + velocity.n[1] * velocity.n[1]) <= 0.0011111111f
        && (angular_velocity.n[0] * angular_velocity.n[0] + angular_velocity.n[2] * angular_velocity.n[2]
                + angular_velocity.n[1] * angular_velocity.n[1]) <= 0.0027415568f
        && (linear_acceleration.n[0] * linear_acceleration.n[0] + linear_acceleration.n[2] * linear_acceleration.n[2]
                + linear_acceleration.n[1] * linear_acceleration.n[1]) <= 0.00000030864197f
        && (angular_acceleration.n[0] * angular_acceleration.n[0] + angular_acceleration.n[2] * angular_acceleration.n[2]
                + angular_acceleration.n[1] * angular_acceleration.n[1]) <= 0.0000030461742f)
    {
        flags |= (1u << _object_at_rest_bit);
    }
    else
    {
        flags &= ~(1u << _object_at_rest_bit);
    }

    if (mass_point_flag_bit1_count <= 0)
        flags &= ~(1u << _object_on_ground_bit);
    else
        flags |= (1u << _object_on_ground_bit);

    /* both on-media (0x2) and partially-under-media (0x3) are gated on the SAME tally (bit3_count) —
     * disasm-confirmed, faithful shipped quirk */
    if (mass_point_flag_bit3_count <= 0)
        flags &= ~((1u << _object_on_media_bit) | (1u << _object_partially_under_media_bit));
    else
        flags |= (1u << _object_on_media_bit) | (1u << _object_partially_under_media_bit);

    if (mass_point_flag_bit3_count != mass_point_count)
        flags &= ~(1u << _object_wholly_under_media_bit);
    else
        flags |= (1u << _object_wholly_under_media_bit);

    vehicle->object.flags = flags;
}
