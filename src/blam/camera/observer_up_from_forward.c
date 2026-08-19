/* observer_up_from_forward @0x8370EB50 — derive an "up" vector for a camera/observer from its forward
 * vector: build a horizontal "right" vector perpendicular to forward's ground projection (falling back
 * to a fixed axis when forward is near-vertical), then up = forward x right.
 *
 * DEVIATION: the components are computed in floats but routed through doubles by the soft-float ABI. */

#include "headers/real_vector3d.h"
#include <math.h>

void observer_up_from_forward(const real_vector3d *forward, real_vector3d *up)
{
    float negated_x = -forward->n[0];
    float horizontal_magnitude = sqrtf(negated_x * negated_x + forward->n[1] * forward->n[1]);

    float right_x, right_y, right_z;
    if ( fabsf(horizontal_magnitude) < 0.000099999997f || horizontal_magnitude == 0.0f )
    {
        right_z = 1.0f;
        right_y = 0.0f;
        right_x = 0.0f;
    }
    else
    {
        float inverse = 1.0f / horizontal_magnitude;
        right_x = inverse * negated_x;
        right_y = inverse * 0.0f;
        right_z = inverse * forward->n[1];
    }

    up->n[2] = forward->n[1] * right_z - forward->n[0] * right_x;
    up->n[0] = forward->n[2] * right_x - forward->n[1] * right_y;
    up->n[1] = forward->n[0] * right_y - forward->n[2] * right_z;
}
