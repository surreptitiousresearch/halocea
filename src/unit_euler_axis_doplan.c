/* unit_euler_axis_doplan @0x836C7840 — advance one axis of a trapezoidal motion plan by delta_t:
 * integrate through the (possibly clipped) acceleration, coast, and deceleration phases in turn, emitting
 * the resulting location and velocity. Returns 1 when the plan is exhausted (already stopped, or time
 * remains after the decel phase), else 0.
 *
 * DEVIATION (bug class 1): the two float args burn the r4/r5/r7 shadow slots and the decompiler split the
 * out-pointers into phantom trailing params; the DB's 6-arg prototype is used. */

#include <stdint.h>
#include "headers/euler_axis_plan.h"

uint8_t unit_euler_axis_doplan(euler_axis_plan *plan, float delta_t, float current_location,
        float *out_location, float current_velocity, float *out_velocity)
{
    uint8_t finished = plan->stopped;
    float time_left = delta_t;
    float location = current_location;
    float velocity = current_velocity;

    if ( !finished && delta_t > 0.0f )
    {
        float accel_t = plan->accel_t;
        if ( accel_t > 0.0f )
        {
            if ( time_left <= accel_t )
                accel_t = time_left;
            time_left -= accel_t;
            location += (plan->accel_a * accel_t * 0.5f + velocity) * accel_t;
            velocity += plan->accel_a * accel_t;
        }
        if ( time_left > 0.0f )
        {
            float coast_t = plan->coast_t;
            if ( coast_t > 0.0f )
            {
                if ( time_left <= coast_t )
                    coast_t = time_left;
                location += coast_t * velocity;
                time_left -= coast_t;
            }
            if ( time_left > 0.0f )
            {
                float decel_t = plan->decel_t;
                if ( decel_t > 0.0f )
                {
                    if ( time_left <= decel_t )
                        decel_t = time_left;
                    time_left -= decel_t;
                    location += (plan->decel_a * decel_t * 0.5f + velocity) * decel_t;
                    velocity += plan->decel_a * decel_t;
                }
                if ( time_left > 0.0f )
                    finished = 1;
            }
        }
    }

    *out_location = location;
    *out_velocity = velocity;
    return finished;
}
