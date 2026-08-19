/* unit_euler_axis_buildplan @0x836C7468 — build a bang-bang (accelerate / coast / decelerate) motion plan that
 * drives one rotational axis from (current_location, current_velocity) back to rest at 0, respecting the given
 * velocity and acceleration limits. If the axis is already essentially at rest it is marked stopped and cleared.
 * If the rest state lies "behind" the current motion it solves the mirrored problem and negates the spatial terms.
 * If the axis will overshoot the origin under a pure decel it emits a single deceleration phase; otherwise it
 * computes the accelerate/decelerate switch time (clamped by the velocity limit) and, when the phase is capped by
 * that limit, the intervening coast time.
 *
 * DEVIATION: all four leading args are floats — an FPR-shadow trap that made Hex-Rays invent phantom integer
 * params and place the plan pointer at "a9"; the DB's 5-arg prototype is authoritative and plan == r7 (disasm
 * 0x836C7480: `stfs f1, 4(r7)`). Both `fsel x, x, x, f13` sites clamp to max(x, 0) (f13 == 0.0, confirmed at
 * 0x836C765C where the same register is the `velocity_limit <= 0` comparand). euler_axis_plan fields use the DB
 * names (accel_a/accel_t/coast_t/decel_a/decel_t). */

#include <stdint.h>
#include "headers/euler_axis_plan.h"

extern double __fabs(double value);
extern float __fsqrts(float value);

void unit_euler_axis_buildplan(float current_location, float current_velocity, float velocity_limit,
        float acceleration_limit, euler_axis_plan *plan)
{
    plan->initial_p = current_location;
    plan->initial_v = current_velocity;

    uint8_t stopped = !(__fabs(current_location) >= 0.001f || __fabs(current_velocity) >= 0.001f);
    plan->stopped = stopped;
    if ( stopped )
    {
        plan->accel_a = 0.0f;
        plan->accel_t = 0.0f;
        plan->coast_t = 0.0f;
        plan->decel_a = 0.0f;
        plan->decel_t = 0.0f;
        return;
    }

    float speed = __fabs(current_velocity);
    float stop_time = speed / acceleration_limit;

    if ( stop_time * 0.5f * current_velocity * 0.5f + current_location < 0.0f )
    {
        unit_euler_axis_buildplan(-current_location, -current_velocity, velocity_limit, acceleration_limit, plan);
        plan->initial_p *= -1.0f;
        plan->initial_v *= -1.0f;
        plan->accel_a *= -1.0f;
        plan->decel_a *= -1.0f;
        return;
    }

    float overshoot = current_velocity * 0.5f * stop_time + current_location;
    if ( overshoot < 0.0f )
    {
        plan->accel_a = 0.0f;
        plan->accel_t = 0.0f;
        plan->decel_a = current_velocity * current_velocity / (current_location * 2.0f);
        plan->decel_t = -(current_velocity / (current_velocity * current_velocity / (current_location * 2.0f)));
        plan->coast_t = 0.0f;
        return;
    }

    float accel_time;
    if ( current_velocity <= 0.0f )
    {
        float discriminant = __fsqrts((current_velocity * 2.0f) * (current_velocity * 2.0f)
                - (-acceleration_limit * overshoot) * 4.0f);
        float root1 = (-(current_velocity * 2.0f) - discriminant) / (-acceleration_limit * 2.0f);
        float root2 = (discriminant - current_velocity * 2.0f) / (-acceleration_limit * 2.0f);
        if ( root1 < 0.0f || (root2 >= 0.0f && root1 >= root2) )
            accel_time = root2 >= 0.0f ? root2 : 0.0f;
        else
            accel_time = root1;
    }
    else
    {
        accel_time = __fsqrts((current_velocity * 0.5f * stop_time + current_location) / acceleration_limit);
    }

    float phase_time;
    if ( velocity_limit <= 0.0f )
    {
        phase_time = accel_time;
    }
    else
    {
        float peak_time = current_velocity <= 0.0f
                ? (current_velocity + velocity_limit) / acceleration_limit
                : velocity_limit / acceleration_limit;
        peak_time = peak_time >= 0.0f ? peak_time : 0.0f;
        phase_time = accel_time <= peak_time ? accel_time : peak_time;
    }

    plan->accel_a = -acceleration_limit;
    plan->decel_a = acceleration_limit;
    float decel_time = phase_time + stop_time;
    if ( current_velocity <= 0.0f )
    {
        plan->decel_t = decel_time;
        plan->accel_t = phase_time;
    }
    else
    {
        plan->decel_t = phase_time;
        plan->accel_t = decel_time;
    }

    if ( phase_time >= accel_time )
    {
        plan->coast_t = 0.0f;
        return;
    }

    plan->coast_t = ((accel_time - phase_time) * (plan->accel_t * -acceleration_limit + current_velocity) * 2.0f
            - (accel_time - phase_time) * (accel_time - phase_time) * acceleration_limit)
            / (plan->accel_t * -acceleration_limit + current_velocity);
}
