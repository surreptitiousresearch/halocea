/* accelerate_to_velocity @0x83700A30 — one-tick velocity servo for a scalar position/velocity pair (used
 * by device machines etc.): when `instantaneous` is set, step the velocity toward velocity_desired by at
 * most acceleration_maximum, advance the position by the trapezoidal step, and wrap it back into
 * [lower, upper] via fmod when it escapes the bounds. Otherwise delegate to accelerate_to_position with
 * the bound in the direction of travel as the desired position and |velocity_desired| as the speed cap.
 *
 * DEVIATION: the decompiler fails local-variable allocation here (raw _FP0/fsel/asm blocks, 11 phantom
 * params). Rebuilt from the disassembly: real signature is 2 pointers + 4 floats (f1..f4) + a byte flag
 * in r9 (FPR-shadow ABI). The fsel idioms are the branchless clamp-to-±accel and pick-bound-by-sign.
 * The wrap path computes fmod(new_pos - lower, upper - lower) MINUS lower (fsub f0,f1,f31) — shipped
 * code, reproduced verbatim even though + lower looks intended. */

#include <stdint.h>

extern double __fabs(double x);
extern double fmod(double x, double y);
extern uint8_t accelerate_to_position(float *position_reference, float *velocity_reference, float position_desired, float acceleration_maximum, float velocity_maximum, float position_lower_bound, float position_upper_bound, uint8_t circular_position);

void accelerate_to_velocity(float *position_reference, float *velocity_reference, float acceleration_maximum, float velocity_desired, float position_lower_bound, float position_upper_bound, uint8_t instantaneous)
{
    if ( instantaneous )
    {
        float velocity_delta = velocity_desired - *velocity_reference;
        if ( __fabs(velocity_delta) > acceleration_maximum )
            /* fsel: clamp the delta to ±acceleration_maximum, keeping its sign */
            velocity_delta = velocity_delta >= 0.0f ? acceleration_maximum : -acceleration_maximum;

        float new_velocity = velocity_delta + *velocity_reference;
        float new_position = velocity_delta * 0.5f + new_velocity + *position_reference;

        if ( new_position < position_lower_bound || new_position > position_upper_bound )
            new_position = (float)(fmod(new_position - position_lower_bound,
                                        position_upper_bound - position_lower_bound)
                                 - position_lower_bound);   /* shipped: minus, not plus */

        *position_reference = new_position;
        *velocity_reference = new_velocity;
    }
    else
    {
        /* fsel: head for the bound in the direction of the desired velocity */
        float position_desired = velocity_desired >= 0.0f ? position_upper_bound : position_lower_bound;
        accelerate_to_position(position_reference, velocity_reference, position_desired,
                               acceleration_maximum, (float)__fabs(velocity_desired),
                               position_lower_bound, position_upper_bound, 0);
    }
}
