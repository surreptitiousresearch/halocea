/* accelerate_to_position @0x837001E4 — advances a 1-D position toward a desired value under a maximum
 * acceleration and speed, optionally wrapping within [lower,upper] for a circular (angular) coordinate.
 * If the position is already within one acceleration step of the target it snaps to it (clamped) and
 * returns 1 (arrived); otherwise it integrates one step of velocity/position and returns 0.
 *
 * DEVIATION: the decompiler bloats the signature with phantom GPR args a9..a12 (the seven leading
 * float/double parameters each reserve a GPR slot) and names the real clamp flag `a13`; the DB
 * prototype confirms the clean 8-argument form used here, with circular_position as the flag. The
 * `fsel f0,f0,f2,f13` idiom clamps the velocity delta to ±acceleration_maximum preserving its sign. */

#include <stdint.h>

extern double __fabs(double x);
extern float  __fsqrts(float x);

uint8_t accelerate_to_position(float *position_reference, float *velocity_reference,
                                     float position_desired, float acceleration_maximum,
                                     float velocity_maximum, float position_lower_bound,
                                     float position_upper_bound, uint8_t circular_position)
{
    float current_position = *position_reference;
    int result = 0;
    float delta = position_desired - *position_reference;

    if (circular_position)
    {
        float half_range = (position_upper_bound - position_lower_bound) * 0.5f;
        if (delta <= half_range)
        {
            if (delta < -half_range)
                delta = (position_upper_bound - position_lower_bound) * 0.5f * 2.0f + delta;
        }
        else
        {
            delta = -((position_upper_bound - position_lower_bound) * 0.5f * 2.0f - delta);
        }
    }

    float step_limit = (acceleration_maximum <= velocity_maximum) ? acceleration_maximum : velocity_maximum;
    if (__fabs(delta - *velocity_reference) > step_limit)
    {
        float braking_speed_sq = __fabs(delta) * (acceleration_maximum * 2.0f);
        float target_speed = velocity_maximum;
        if (braking_speed_sq < velocity_maximum * velocity_maximum)
            target_speed = __fsqrts(braking_speed_sq);
        if (delta < 0.0f)
            target_speed = -target_speed;

        float velocity_delta = target_speed - *velocity_reference;
        if (__fabs(velocity_delta) > acceleration_maximum)
            velocity_delta = (velocity_delta >= 0.0f) ? acceleration_maximum : -acceleration_maximum;

        float new_velocity = velocity_delta + *velocity_reference;
        float new_position = (velocity_delta * 0.5f + (velocity_delta + *velocity_reference)) + current_position;

        if (circular_position)
        {
            if (new_position >= position_lower_bound)
            {
                if (new_position > position_upper_bound)
                    new_position -= (position_upper_bound - position_lower_bound);
            }
            else
            {
                new_position = (position_upper_bound - position_lower_bound) + new_position;
            }
        }

        if (new_position >= position_lower_bound)
        {
            if (new_position > position_upper_bound)
                new_position = position_upper_bound;
            *velocity_reference = new_velocity;
            *position_reference = new_position;
        }
        else
        {
            *velocity_reference = new_velocity;
            *position_reference = position_lower_bound;
        }
    }
    else if (position_desired >= position_lower_bound)
    {
        float clamped = position_desired;
        if (position_desired > position_upper_bound)
            clamped = position_upper_bound;
        *velocity_reference = 0.0f;
        *position_reference = clamped;
        return 1;
    }
    else
    {
        *velocity_reference = 0.0f;
        *position_reference = position_lower_bound;
        return 1;
    }

    return result;
}
