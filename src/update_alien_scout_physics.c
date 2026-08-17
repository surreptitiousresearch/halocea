/* update_alien_scout_physics @0x83761488 — per-frame physics for the alien "Ghost" hover vehicle.
 *
 * Seeds each powered mass point with the vehicle's antigrav authority and an identity rotation, then (when the
 * hull is out of deep water and not flipped past horizontal) builds "magic" linear/angular accelerations that
 *   1. thrust the hull toward the requested planar velocity (clamped to a max lateral acceleration, reduced
 *      while the handbrake is held through a sharp turn),
 *   2. yaw it toward the requested turn rate,
 *   3. level it back upright (a PD term over the forward/left axes projected into the ground plane) whenever the
 *      hover authority is below full, and
 *   4. damp tipping while the flip-recovery flag is set.
 * These are scaled by the antigrav base and handed to physics_update, after which the hover authority
 * (antigrav_ratio, object+0x4EC) is integrated toward the fraction of grounded mass points that are settled
 * (clamped to +/-0.1 per frame) and the ghost trail effect is refreshed.
 *
 * Reconstructed from a reg-alloc-failed decompile; every expression was cross-checked against the disassembly
 * (0x83761488-0x83761ED0). Deviations from the raw decompiler output:
 *   - The database prototype has FOUR parameters. The decompiler modelled `steering` as a double, which shifts
 *     the PPC argument registers and made it fabricate a fifth pointer parameter. `steering` is a float in f1
 *     (its GPR slot r4 is skipped), so r5=powered_mass_points and r6=mass_points; there is no fifth argument.
 *   - The decompiler's phantom accumulator registers (v31..v33 / v43..v45) are written straight into
 *     magic_force / magic_torque here, since each update was immediately stored back to those stack vectors.
 *   - Reg-alloc garbage globals (&g_PFP[208], "neoffire_crouching", &hkpMeshShape_..., 0x82000000, COERCE_FLOAT
 *     junk) stood in for spilled int->float sign values and for the two normalized planar axes; they are
 *     restored to their real values below. global_zero_vector2d spill terms (all zero) are dropped.
 *
 * Object datum fields (offsets DB/disasm-confirmed for this vehicle object), accessed by raw byte offset in the
 * item_* house style:
 *    +92  (0x5C)  position         (real_point3d)      +632 (0x278) control_throttle.x (float)
 *    +104 (0x68)  velocity         (real_vector3d)      +636 (0x27C) control_throttle.y (float)
 *    +116 (0x74)  forward          (real_vector3d)      +824 (0x338) antigrav_base      (float)
 *    +128 (0x80)  up               (real_vector3d)      +0x4CC       action_flags       (uint16, bit 0x8=boost)
 *    +140 (0x8C)  angular_velocity (real_vector3d)      +0x4D0       flip_recovery_flag (uint8)
 *    +152 (0x98)  location         (location)           +0x4D3       handbrake_ticks    (uint8)
 *                                                       +0x4EC       antigrav_ratio     (float, 0..1)
 */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/powered_mass_point_datum.h"
#include "headers/mass_point_datum.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_definition.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/ppc_intrinsics.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/vehicle_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"
#include "headers/mass_point_flags.h"


extern float scenario_location_water_depth(const location *location, const real_point3d *position);
extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern real_vector3d *matrix4x3_inverse_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern real_vector3d *matrix4x3_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern float normalize3d(real_vector3d *v);
extern void physics_update(int object_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points, const real_vector3d *magic_force, const real_vector3d *magic_torque);
extern void create_ghost_effect(int vehicle_index);

/* sign(x): 0 for zero, +1 above, -1 below — matches the extsw'd li r,{0,1,-1} sequences. */
static int fsign(float x)
{
    if (x == 0.0f)
        return 0;
    return (x > 0.0f) ? 1 : -1;
}

void update_alien_scout_physics(int vehicle_index, float steering,
                                powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points)
{
    vehicle_datum *object = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    vehicle_definition *definition = TAG_GET(vehicle_definition, *(int *)object);
    /* physics_definition is the referenced physics tag blob; +8/+80/+84/+88 (scale factors), +104/+116
     * (counts) and +120 (mass-point def block ptr) have no clean DB member — faithful cast-indexed reads. */
    physics_definition *physics = TAG_GET(physics_definition, definition->object.physics.index);

    const real_point3d  *position         = &object->object.position;
    const real_vector3d *velocity         = &object->object.translational_velocity;
    const real_vector3d *forward          = &object->object.forward;
    const real_vector3d *up               = &object->object.up;
    const real_vector3d *angular_velocity = &object->object.angular_velocity;

    float antigrav_base  = object->unit.seat_power[0];
    float antigrav_ratio = object->vehicle.hover;
    float water_depth;
    int   mass_point_index, powered_mass_point_count;

    real_matrix4x3 vehicle_frame;
    real_vector3d  local_velocity;
    real_vector3d  steering_vector;
    real_vector3d  left_axis;
    real_vector3d  magic_force;
    real_vector3d  magic_torque;

    water_depth = scenario_location_water_depth(&object->object.location, position);

    magic_force  = *global_zero_vector3d;
    magic_torque = *global_zero_vector3d;

    /* Seed every powered mass point: antigrav authority + identity rotation. */
    powered_mass_point_count = physics->powered_mass_points.count;
    for (mass_point_index = 0; mass_point_index < powered_mass_point_count;
         mass_point_index = (int16_t)(mass_point_index + 1))
    {
        powered_mass_points[mass_point_index].antigrav_fraction = antigrav_base;
        powered_mass_points[mass_point_index].rotation.n[0] = 0.0f;
        powered_mass_points[mass_point_index].rotation.n[1] = 0.0f;
        powered_mass_points[mass_point_index].rotation.n[2] = 0.0f;
        powered_mass_points[mass_point_index].rotation.n[3] = 1.0f;
    }

    if (water_depth < 0.5f && up->n[2] > -0.2f)
    {
        matrix4x3_from_point_and_vectors(&vehicle_frame, position, forward, up);
        matrix4x3_inverse_transform_vector(&vehicle_frame, velocity, &local_velocity);

        /* (1) Thrust toward the requested planar velocity, clamped to a max lateral acceleration. */
        if (antigrav_ratio > 0.0f)
        {
            float reference_speed = definition->vehicle.speed.maximum_forward_speed;
            float target_vx, target_vy, max_accel, delta_mag2;

            if ((object->vehicle.flags & (1u << _vehicle_control_jump_bit)) != 0)
                reference_speed = reference_speed * 0.80000001f;   /* boosting caps thrust */

            target_vx = reference_speed * object->unit.throttle.n[0];
            target_vy = reference_speed * object->unit.throttle.n[1];
            steering_vector.n[0] = target_vx - local_velocity.n[0];
            steering_vector.n[1] = target_vy - local_velocity.n[1];
            steering_vector.n[2] = 0.0f;

            max_accel = definition->vehicle.speed.acceleration;
            if (object->vehicle.on_ground_ticks && __fabs(steering) > 0.78539819f)
            {
                float reduce = (float)object->vehicle.on_ground_ticks * 0.050000001f;
                if (reduce > 0.98000002f)
                    reduce = 0.98000002f;
                max_accel = (1.0f - reduce) * max_accel;
            }

            delta_mag2 = steering_vector.n[0] * steering_vector.n[0]
                       + steering_vector.n[1] * steering_vector.n[1];
            if (delta_mag2 > max_accel * max_accel)
            {
                float inv = max_accel / __fsqrts(delta_mag2);
                steering_vector.n[0] = inv * steering_vector.n[0];
                steering_vector.n[1] = steering_vector.n[1] * inv;
                steering_vector.n[2] = inv * 0.0f;
            }

            matrix4x3_transform_vector(&vehicle_frame, &steering_vector, &steering_vector);
            {
                float scale = physics->mass * antigrav_ratio;
                magic_force.n[0] += steering_vector.n[0] * scale;
                magic_force.n[1] += steering_vector.n[1] * scale;
                magic_force.n[2] += scale * steering_vector.n[2];
            }
        }

        /* (2) Yaw torque steering the hull toward the requested turn rate. */
        if (antigrav_ratio > 0.0f)
        {
            float current_yaw_rate = up->n[0] * angular_velocity->n[0]
                                   + up->n[1] * angular_velocity->n[1]
                                   + up->n[2] * angular_velocity->n[2];
            float desired_rate = __fsqrts((float)(__fabs(steering) * 0.006981317419558764))
                               * (float)fsign(steering);
            float rate_error, clamped;

            if (__fabs(desired_rate) > 0.000099999997f && steering / desired_rate < 2.0f)
                desired_rate = steering * 0.5f;

            rate_error = desired_rate - current_yaw_rate;
            clamped = -0.0034906587f;
            if (rate_error >= -0.0034906587f)
            {
                clamped = 0.0034906587f;
                if (rate_error <= 0.0034906587f)
                    clamped = rate_error;
            }
            {
                float torque = physics->zz_moment * clamped * antigrav_ratio;
                magic_torque.n[0] += up->n[0] * torque;
                magic_torque.n[1] += torque * up->n[1];
                magic_torque.n[2] += torque * up->n[2];
            }
        }

        /* (3) Level the hull back upright while hover authority is below full. */
        if (antigrav_ratio < 1.0f)
        {
            /* left_axis = up x forward */
            float fwd_nx, fwd_ny, left_nx, left_ny, fwd_len, left_len;
            float level_torque_a, level_torque_b;

            left_axis.n[0] = forward->n[2] * up->n[1] - forward->n[1] * up->n[2];
            left_axis.n[1] = forward->n[0] * up->n[2] - up->n[0] * forward->n[2];
            left_axis.n[2] = up->n[0] * forward->n[1] - forward->n[0] * up->n[1];

            /* Normalized projections of forward and left onto the world plane. */
            fwd_nx = forward->n[0];
            fwd_ny = forward->n[1];
            fwd_len = __fsqrts(fwd_ny * fwd_ny + fwd_nx * fwd_nx);
            if (__fabs(fwd_len) >= 0.000099999997f)
            {
                float inv = 1.0f / fwd_len;
                fwd_nx = inv * forward->n[0];
                fwd_ny = forward->n[1] * inv;
            }

            left_nx = left_axis.n[0];
            left_ny = left_axis.n[1];
            left_len = __fsqrts(left_axis.n[1] * left_axis.n[1] + left_axis.n[0] * left_axis.n[0]);
            if (__fabs(left_len) >= 0.000099999997f)
            {
                float inv = 1.0f / left_len;
                left_nx = inv * left_axis.n[0];
                left_ny = left_axis.n[1] * inv;
            }

            if (up->n[2] <= 0.0f)
            {
                /* Hull past horizontal: minimal control-input righting torque. */
                level_torque_b = object->unit.throttle.n[0] * 0.0015514038f;
                level_torque_a = object->unit.throttle.n[1] * 0.0015514038f;
            }
            else
            {
                /* PD term: 15x the planar angular-rate error minus the alignment error, per axis.
                 * (The forward/left axes are asymmetric in the binary — pd_a adds the rate term, pd_b
                 * subtracts it — because of the fnmadds/fmadds pairing; reproduced verbatim.) */
                float align_left = up->n[0] * left_nx + up->n[1] * left_ny;
                float align_fwd  = up->n[0] * fwd_nx + up->n[1] * fwd_ny;
                float rate_fwd   = angular_velocity->n[0] * fwd_nx + angular_velocity->n[1] * fwd_ny;
                float rate_left  = angular_velocity->n[0] * left_nx + angular_velocity->n[1] * left_ny;
                float pd_a = 15.0f * rate_fwd - align_left;    /* about the forward axis */
                float pd_b = -align_fwd - 15.0f * rate_left;   /* about the left axis */
                float weight_b = __fabs(pd_b) * (float)fsign(object->unit.throttle.n[0] * pd_b) + 1.0f;
                float weight_a = __fabs(pd_a) * (float)fsign(object->unit.throttle.n[1] * pd_a) + 1.0f;
                float one_minus_upz = 1.0f - up->n[2];

                if (weight_b < 0.30000001f) weight_b = 0.30000001f;
                else if (weight_b > 2.5f)   weight_b = 2.5f;
                if (weight_a < 0.30000001f) weight_a = 0.30000001f;
                else if (weight_a > 2.5f)   weight_a = 2.5f;

                level_torque_b = one_minus_upz * pd_b * 0.0038785094f
                               + object->unit.throttle.n[0] * weight_b * 0.0015514038f;
                level_torque_a = one_minus_upz * pd_a * 0.0038785094f
                               + object->unit.throttle.n[1] * weight_a * 0.0015514038f;
            }

            {
                /* DEVIATION: the leveling accumulation targets the magic_torque stack vector, not
                 * magic_force — fmadds f31/f30/f29 + stfs @0x83761B20-0x83761B3C write var_138/134/130,
                 * the block passed as physics_update's magic_torque (addi r7 @0x83761D84); magic_force
                 * is var_148-based (addi r6 @0x83761D88). An earlier pass accumulated this into force. */
                float torque_a = physics->xx_moment * level_torque_a;
                float torque_b = physics->yy_moment * level_torque_b;
                float one_minus_ratio = 1.0f - antigrav_ratio;
                magic_torque.n[0] += one_minus_ratio * (-torque_a * forward->n[0] + torque_b * left_axis.n[0]);
                magic_torque.n[1] += (forward->n[1] * -torque_a + left_axis.n[1] * torque_b) * one_minus_ratio;
                magic_torque.n[2] += (-torque_a * forward->n[2] + left_axis.n[2] * torque_b) * one_minus_ratio;
            }
        }

        /* (4) Anti-tip damping while the flip-recovery flag is set. */
        if ((object->vehicle.flags & (1u << _vehicle_control_jump_bit)) != 0)
        {
            float forward_speed_fraction =
                  (velocity->n[0] * forward->n[0]
                 + (velocity->n[2] * forward->n[2] + velocity->n[1] * forward->n[1]))
                / definition->vehicle.speed.maximum_forward_speed;
            float left_x = forward->n[2] * up->n[1] - forward->n[1] * up->n[2];  /* up x forward */
            float left_y = up->n[2] * forward->n[0] - up->n[0] * forward->n[2];
            float left_z = up->n[0] * forward->n[1] - up->n[1] * forward->n[0];

            if (forward_speed_fraction >= 0.0f)
            {
                if (forward_speed_fraction > 1.0f)
                    forward_speed_fraction = 1.0f;
            }
            else
                forward_speed_fraction = 0.0f;

            if (forward_speed_fraction > 0.0f)
            {
                float roll_torque = physics->yy_moment * forward_speed_fraction
                                  * antigrav_ratio * -0.005817764f;
                float lift = physics->mass * forward_speed_fraction
                           * antigrav_ratio * 0.0040000002f;
                magic_torque.n[0] += left_x * roll_torque;
                magic_torque.n[1] += left_y * roll_torque;
                magic_torque.n[2] += left_z * roll_torque;
                magic_force.n[0] += global_up3d->n[0] * lift;
                magic_force.n[1] += global_up3d->n[1] * lift;
                magic_force.n[2] += global_up3d->n[2] * lift;
            }

            if (object->vehicle.airborne_ticks)
            {
                /* Straighten spin: torque axis = left x up, faded out over the first 30 ticks of recovery. */
                left_axis.n[0] = left_y * global_up3d->n[2] - left_z * global_up3d->n[1];
                left_axis.n[1] = left_z * global_up3d->n[0] - left_x * global_up3d->n[2];
                left_axis.n[2] = left_x * global_up3d->n[1] - left_y * global_up3d->n[0];
                if (normalize3d(&left_axis) > 0.0f)
                {
                    float fade = 1.0f - (float)object->vehicle.airborne_ticks * SECONDS_PER_TICK;
                    float damp;
                    if (fade >= 0.0f)
                    {
                        if (fade > 1.0f)
                            fade = 1.0f;
                    }
                    else
                        fade = 0.0f;
                    damp = (1.0f - antigrav_ratio) * physics->mass * fade;
                    magic_force.n[0] += left_axis.n[0] * (damp * 0.0020000001f)
                                      + global_up3d->n[0] * (damp * 0.001f);
                    magic_force.n[1] += left_axis.n[1] * (damp * 0.0020000001f)
                                      + global_up3d->n[1] * (damp * 0.001f);
                    magic_force.n[2] += left_axis.n[2] * (damp * 0.0020000001f)
                                      + global_up3d->n[2] * (damp * 0.001f);
                }
            }
        }

        magic_force.n[0]  *= antigrav_base;
        magic_force.n[1]  *= antigrav_base;
        magic_force.n[2]  *= antigrav_base;
        magic_torque.n[0] *= antigrav_base;
        magic_torque.n[1] *= antigrav_base;
        magic_torque.n[2] *= antigrav_base;
    }

    physics_update(vehicle_index, powered_mass_points, mass_points, &magic_force, &magic_torque);

    /* Integrate hover authority toward the fraction of grounded mass points that are settled. */
    {
        float settle_cap = 0.40000001f;
        int   mass_point_count;
        int   grounded = 0, grounded_settled = 0;
        float ratio = 0.0f;
        float target, delta, current_ratio;
        int   i;

        if (up->n[2] >= 0.40000001f)
            settle_cap = up->n[2];

        mass_point_count = physics->mass_points.count;
        if (mass_point_count > 0)
        {
            mass_point_definition *mass_point_defs = (mass_point_definition *)physics->mass_points.address;   /* +120 */
            for (i = 0; i < mass_point_count; i = (int16_t)(i + 1))
            {
                if ((uint16_t)mass_point_defs[i].powered_mass_point_index != 0xFFFF)
                {
                    grounded = (int16_t)(grounded + 1);
                    if ((mass_points[i].flags & (1u << _point_antigraving_bit)) != 0)
                        ++grounded_settled;
                }
            }
        }

        if ((int16_t)grounded > 0)
            ratio = (float)grounded_settled / (float)grounded;

        target = ratio * settle_cap;
        if (target >= 0.0f)
        {
            if (target > 1.0f)
                target = 1.0f;
        }
        else
            target = 0.0f;

        current_ratio = object->vehicle.hover;
        delta = target - current_ratio;
        if (delta > 0.1f)
            target = current_ratio + 0.1f;
        else if (delta < -0.1f)
            target = current_ratio - 0.1f;
        object->vehicle.hover = target;
    }

    create_ghost_effect(vehicle_index);
}
