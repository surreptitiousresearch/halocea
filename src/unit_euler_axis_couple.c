/* unit_euler_axis_couple @0x836C76F8 — synchronize two independent euler-axis motion plans (e.g. pitch
 * and yaw) so the one that finishes sooner is stretched to finish at the same time as the slower one:
 * picks whichever plan has the shorter total duration (accel_t+coast_t+decel_t) and, if that plan is
 * still in its acceleration phase, re-solves its coast velocity so its own total duration grows to match
 * the other plan's, then redistributes its acceleration/deceleration/coast phase durations accordingly.
 * No-op if either plan is stopped, if neither plan has remaining acceleration time, or if the "faster"
 * plan is already the longer (or equal) of the two. */

#include "headers/euler_axis_plan.h"

extern double __fabs(double x);
extern float __fsqrts(float x);

void unit_euler_axis_couple(euler_axis_plan *plan1, euler_axis_plan *plan2, float velocity_limit,
    float acceleration_limit)
{
    if ( plan1->stopped || plan2->stopped )
        return;

    float duration1 = plan1->decel_t + plan1->coast_t + plan1->accel_t;
    float duration2 = plan2->decel_t + plan2->coast_t + plan2->accel_t;

    euler_axis_plan *shorter_plan;
    float duration_delta;

    if ( plan1->accel_t > 0.0f && duration1 < duration2 )
    {
        shorter_plan = plan1;
        duration_delta = duration2 - duration1;
    }
    else if ( plan2->accel_t > 0.0f && duration2 < duration1 )
    {
        shorter_plan = plan2;
        duration_delta = duration1 - duration2;
    }
    else
    {
        return;
    }

    float accel_t = shorter_plan->accel_t;
    float accel_a = shorter_plan->accel_a;
    float initial_v = shorter_plan->initial_v;
    float decel_t = shorter_plan->decel_t;

    float stretched_coast_v = (shorter_plan->coast_t + duration_delta) * velocity_limit;
    float coast_v = (__fsqrts(stretched_coast_v * stretched_coast_v
                     - __fabs(shorter_plan->accel_a * shorter_plan->accel_t + shorter_plan->initial_v)
                       * -duration_delta * velocity_limit * 4.0f)
                    - stretched_coast_v)
                   / (velocity_limit * 2.0f);

    float phase_limit = (accel_t <= decel_t) ? shorter_plan->accel_t : shorter_plan->decel_t;
    if ( coast_v > phase_limit )
        coast_v = (accel_t <= decel_t) ? shorter_plan->accel_t : shorter_plan->decel_t;

    if ( coast_v > 0.0f )
    {
        float new_accel_t = shorter_plan->accel_t - coast_v;
        shorter_plan->accel_t = new_accel_t;
        shorter_plan->decel_t = decel_t - coast_v;

        float velocity_at_coast_start = new_accel_t * accel_a + initial_v;
        shorter_plan->coast_t = (accel_a * coast_v + velocity_at_coast_start * 2.0f) * coast_v
                               / velocity_at_coast_start;
    }
}
