/* unit_euler_aiming_update @0x836C7920 — per-tick update of a unit's aim: turns aiming_vector toward
 * desired_aiming_vector, clamped to aiming_bounds (a yaw/pitch box in the weapon/turret's own local
 * frame), respecting angular_velocity_limit/angular_acceleration_limit via independent bang-bang motion
 * plans on each axis (see euler_axis_plan.h), and updates aiming_velocity to match.
 *
 * If `orientation` is given, aiming_vector/desired_aiming_vector/the output are all rotated into that
 * matrix's local frame first (and back out at the end) so the yaw/pitch math and aiming_bounds are
 * always evaluated in local space; otherwise the vectors are used directly in world space.
 *
 * DEVIATION: this decompile is "local variable allocation failed"-class corruption from a large stack
 * frame — dozens of `v14`..`v70` locals with no clear correspondence to the 5 real stack slots actually
 * in play, and several call arguments (`v39`-`v49`) appear to reference locals that are never assigned
 * in the shown decompile at all. Rebuilt from disasm_range(0x836C7920, 0x836C7EB4) register-by-register;
 * see the inline notes below for the specific findings.
 *
 * FAITHFUL QUIRK: the `unit_euler_axis_couple` call passes `angular_acceleration_limit` as the
 * *velocity_limit* argument (disasm: `fmr f1, f26` where f26 holds `angular_acceleration_limit` from the
 * function's own parameter) — `angular_velocity_limit` is never passed to this call at all. This isn't a
 * transcription slip: `unit_euler_axis_couple`'s own body (decompiled directly) never reads its
 * `acceleration_limit` parameter, so the only value that matters for coupling the two axes' plan
 * durations is whatever ends up in the velocity_limit slot — which here is the acceleration limit.
 * Reproduced verbatim, not "fixed".
 *
 * FAITHFUL: `unit_euler_axis_doplan`'s `out_location` pointer is passed as the SAME stack slot backing
 * its own `current_location` argument for both axes (disasm: `stfs f31, var_138` then later
 * `addi r6, r1, var_138` for both the float-load and the out-pointer) — i.e. each axis's pending delta
 * angle is updated in place. The `out_velocity` outputs are written to slots that are never read again
 * (dead outputs); modeled here as throwaway locals. */

#include <math.h>

#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/euler_axis_plan.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"

#define TWO_PI 6.2831855f

extern real_vector3d *matrix4x3_inverse_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern float normalize3d(real_vector3d *v);
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);

void unit_euler_aiming_update(const real_matrix4x3 *orientation, real_vector3d *aiming_vector,
        const real_vector3d *desired_aiming_vector, real_vector3d *aiming_velocity,
        const real_rectangle2d *aiming_bounds, float angular_velocity_limit, float angular_acceleration_limit)
{
    real_vector3d local_aiming_vector;
    real_vector3d local_desired_vector;
    if (orientation)
    {
        matrix4x3_inverse_transform_normal(orientation, aiming_vector, &local_aiming_vector);
        matrix4x3_inverse_transform_normal(orientation, desired_aiming_vector, &local_desired_vector);
    }
    else
    {
        local_aiming_vector = *aiming_vector;
        local_desired_vector = *desired_aiming_vector;
    }

    /* Yaw bounds spanning (close to) the full circle are treated as wraparound rather than clamped. */
    unsigned char yaw_wraps = (aiming_bounds->x1 - aiming_bounds->x0) - TWO_PI > -0.0001f;

    real_euler_angles2d current_angles;
    real_euler_angles2d desired_angles;
    euler_angles2d_from_vector3d(&current_angles, &local_aiming_vector);
    euler_angles2d_from_vector3d(&desired_angles, &local_desired_vector);

    float desired_yaw = desired_angles.yaw;
    unsigned char within_bounds = 1;

    if (yaw_wraps)
    {
        if (desired_yaw < aiming_bounds->x0)
            desired_yaw = desired_angles.yaw + TWO_PI;
        else if (desired_yaw > aiming_bounds->x1)
            desired_yaw = desired_angles.yaw - TWO_PI;
    }
    else if (desired_yaw < aiming_bounds->x0 || desired_yaw > aiming_bounds->x1)
    {
        within_bounds = 0;
        desired_yaw = desired_yaw < aiming_bounds->x0 ? aiming_bounds->x0 : aiming_bounds->x1;
    }
    desired_angles.yaw = desired_yaw;

    if (desired_angles.pitch < aiming_bounds->y0 || desired_angles.pitch > aiming_bounds->y1)
    {
        desired_angles.pitch = desired_angles.pitch < aiming_bounds->y0 ? aiming_bounds->y0 : aiming_bounds->y1;
        within_bounds = 0;
    }

    real_vector3d bounded_desired_vector;
    if (within_bounds)
    {
        /* Already inside the bounds: use the caller's vector as-is (avoids a redundant angle round-trip). */
        bounded_desired_vector = local_desired_vector;
    }
    else
    {
        vector3d_from_euler_angles2d(&bounded_desired_vector, &desired_angles);
        if (orientation)
        {
            matrix4x3_transform_normal(orientation, &bounded_desired_vector, &bounded_desired_vector);
            normalize3d(&bounded_desired_vector);
        }
        /* NOTE: disasm re-reads `v23 = v71.n[0]` here, i.e. desired_yaw — already equal to
         * desired_angles.yaw at this point, so no-op; nothing else to reproduce for this branch. */
    }

    /* Estimate each axis's current angular velocity from the drift of the (normalized) velocity vector
     * over one tick: rotate the current aiming vector by aiming_velocity's own magnitude/direction, then
     * diff the resulting angles against the un-rotated current angles.
     *
     * FAITHFUL QUIRK: `rotated_angles` is only written on the non-degenerate path below. When the
     * velocity is near zero, disasm_range(0x836C7B18, 0x836C7BC4) shows the euler_angles2d_from_vector3d
     * call that would populate it is skipped entirely, and its backing stack slots are never written by
     * anything earlier in the function either — so the original code reads uninitialized stack memory
     * into `next_angles` below in that case. Reproduced as-is (uninitialized), not "fixed" with a zero. */
    real_euler_angles2d rotated_angles;
    real_vector3d rotated_vector = local_aiming_vector;
    real_vector3d velocity_axis = *aiming_velocity;
    float velocity_length = sqrtf(velocity_axis.k * velocity_axis.k
            + velocity_axis.j * velocity_axis.j + velocity_axis.i * velocity_axis.i);

    float yaw_velocity;
    float pitch_velocity;
    if (fabsf(velocity_length) < 0.0001f)
    {
        yaw_velocity = 0.0f;
        pitch_velocity = 0.0f;
    }
    else
    {
        float inv_length = 1.0f / velocity_length;
        velocity_axis.i = inv_length * velocity_axis.i;
        velocity_axis.j = velocity_axis.j * inv_length;
        velocity_axis.k = inv_length * velocity_axis.k;

        if (velocity_length == 0.0f)
        {
            yaw_velocity = 0.0f;
            pitch_velocity = 0.0f;
        }
        else
        {
            rotate_vector_about_axis(&rotated_vector, &velocity_axis, sinf(velocity_length), cosf(velocity_length));
            euler_angles2d_from_vector3d(&rotated_angles, &rotated_vector);
            yaw_velocity = rotated_angles.yaw - current_angles.yaw;
            pitch_velocity = rotated_angles.pitch - current_angles.pitch;
        }
    }

    float yaw_delta = current_angles.yaw - desired_yaw;
    float pitch_delta = current_angles.pitch - desired_angles.pitch;
    if (yaw_wraps)
    {
        if (yaw_delta > PI)
            yaw_delta = (current_angles.yaw - desired_yaw) - TWO_PI;
        else if (yaw_delta < -3.1415927f)
            yaw_delta = (current_angles.yaw - desired_yaw) + TWO_PI;
    }

    euler_axis_plan yaw_plan, pitch_plan;
    unit_euler_axis_buildplan(yaw_delta, yaw_velocity, angular_velocity_limit, angular_acceleration_limit, &yaw_plan);
    unit_euler_axis_buildplan(pitch_delta, pitch_velocity, angular_velocity_limit, angular_acceleration_limit, &pitch_plan);
    /* FAITHFUL QUIRK: passes acceleration_limit where couple() expects velocity_limit — see file header. */
    unit_euler_axis_couple(&yaw_plan, &pitch_plan, angular_acceleration_limit, yaw_velocity);

    float dead_velocity_out1, dead_velocity_out2;
    unsigned __int8 yaw_plan_done = unit_euler_axis_doplan(&yaw_plan, 1.0f, yaw_delta, &yaw_delta,
            yaw_velocity, &dead_velocity_out1);
    unsigned __int8 pitch_plan_done = unit_euler_axis_doplan(&pitch_plan, 1.0f, pitch_delta, &pitch_delta,
            pitch_velocity, &dead_velocity_out2);

    if (yaw_plan_done && pitch_plan_done)
    {
        *aiming_vector = bounded_desired_vector;
        *aiming_velocity = *global_zero_vector3d;
        return;
    }

    real_euler_angles2d new_angles;
    new_angles.yaw = yaw_delta + desired_yaw;
    new_angles.pitch = pitch_delta + desired_angles.pitch;
    if (yaw_wraps)
    {
        if (new_angles.yaw >= aiming_bounds->x0)
        {
            if (new_angles.yaw > aiming_bounds->x1)
                new_angles.yaw = (yaw_delta + desired_yaw) - TWO_PI;
        }
        else
        {
            new_angles.yaw = (yaw_delta + desired_yaw) + TWO_PI;
        }
    }
    else if (new_angles.yaw < aiming_bounds->x0 || new_angles.yaw > aiming_bounds->x1)
    {
        new_angles.yaw = new_angles.yaw < aiming_bounds->x0 ? aiming_bounds->x0 : aiming_bounds->x1;
    }
    if (new_angles.pitch < aiming_bounds->y0 || new_angles.pitch > aiming_bounds->y1)
        new_angles.pitch = new_angles.pitch < aiming_bounds->y0 ? aiming_bounds->y0 : aiming_bounds->y1;

    real_vector3d new_vector;
    vector3d_from_euler_angles2d(&new_vector, &new_angles);

    /* Re-derive the angular velocity as the axis/angle between the new-target vector and a second vector
     * offset from it by `new_angles` again (i.e. `rotated_angles + new_angles`, NOT `current_angles +
     * new_angles` — confirmed via disasm: this reuses the same `rotated_angles` struct from the
     * velocity-estimate step above, including its possibly-uninitialized value per the note above). */
    real_euler_angles2d next_angles = { .yaw = rotated_angles.yaw + new_angles.yaw,
                                         .pitch = rotated_angles.pitch + new_angles.pitch };
    real_vector3d next_vector;
    vector3d_from_euler_angles2d(&next_vector, &next_angles);

    double cos_angle = new_vector.k * next_vector.k + new_vector.j * next_vector.j + new_vector.i * next_vector.i;
    if (cos_angle < -1.0)
        cos_angle = -1.0;
    else if (cos_angle > 1.0)
        cos_angle = 1.0;

    aiming_velocity->i = new_vector.j * next_vector.k - next_vector.j * new_vector.k;
    aiming_velocity->j = new_vector.k * next_vector.i - next_vector.k * new_vector.i;
    aiming_velocity->k = next_vector.j * new_vector.i - new_vector.j * next_vector.i;
    normalize3d(aiming_velocity);

    float rate = (float)acos(cos_angle);
    if (rate > angular_velocity_limit)
        rate = angular_velocity_limit;
    aiming_velocity->i *= rate;
    aiming_velocity->j *= rate;
    aiming_velocity->k *= rate;

    if (orientation)
    {
        matrix4x3_transform_normal(orientation, &new_vector, aiming_vector);
        normalize3d(aiming_vector);
    }
    else
    {
        *aiming_vector = new_vector;
    }
}
