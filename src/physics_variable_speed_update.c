/* physics_variable_speed_update @0x838106C8 — advance a variable-speed scalar by one step. `magnitude`
 * scales the per-step accel/decel (its sign selects direction). Accelerating: while still moving the wrong
 * way the deceleration rate is applied (with a smooth blend through zero), otherwise the acceleration rate,
 * clamped to the forward/reverse maximum. Zero magnitude is a no-op.
 * DEVIATION: soft-float doubles in the decompiler are reproduced as float math. */

#include "headers/physics_variable_speed.h"

extern double __fabs(double x);

void physics_variable_speed_update(float *speed, const physics_variable_speed *definition, float magnitude)
{
    float scale = (float)__fabs(magnitude);
    float decel_step = definition->deceleration * scale;
    float accel_step = definition->acceleration * scale;

    if ( magnitude <= 0.0f )
    {
        if ( magnitude >= 0.0f )
            return;   /* magnitude == 0 */
        float current = *speed;
        if ( current < decel_step )
        {
            if ( current > 0.0f )
                *speed = (*speed / decel_step - 1.0f) * accel_step;
            else
                *speed = *speed - accel_step;
        }
        else
        {
            *speed = *speed - decel_step;
        }
        float clamped = *speed;
        float reverse_limit = -(definition->maximum_reverse_speed * scale);
        if ( clamped < reverse_limit )
            clamped = reverse_limit;
        *speed = clamped;
    }
    else
    {
        float current = *speed;
        if ( current > -decel_step )
        {
            if ( current < 0.0f )
                *speed = (*speed / decel_step + 1.0f) * accel_step;
            else
                *speed = *speed + accel_step;
        }
        else
        {
            *speed = *speed + decel_step;
        }
        float forward_limit = definition->maximum_forward_speed * scale;
        if ( *speed <= forward_limit )
            return;
        *speed = forward_limit;
    }
}
