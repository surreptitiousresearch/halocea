/* interpolate_normal @0x836E5FC8 — move each component of `current` toward `desired`, limiting the per-call
 * change to +-maximum_speed, then renormalize the result back to unit length (leaving it untouched if it has
 * collapsed to near-zero). */

#include <math.h>
#include "headers/real_vector3d.h"

void interpolate_normal(real_vector3d *current, const real_vector3d *desired, float maximum_speed)
{
    for ( int axis = 0; axis < 3; axis++ )
    {
        float delta = desired->n[axis] - current->n[axis];
        if ( delta < -maximum_speed )
            delta = -maximum_speed;
        else if ( delta > maximum_speed )
            delta = maximum_speed;
        current->n[axis] = current->n[axis] + delta;
    }

    float magnitude = sqrtf((current->n[0] * current->n[0])
                          + ((current->n[1] * current->n[1]) + (current->n[2] * current->n[2])));
    if ( fabsf(magnitude) >= 0.000099999997f )
    {
        float inverse = 1.0f / magnitude;
        current->n[0] = current->n[0] * inverse;
        current->n[1] = current->n[1] * inverse;
        current->n[2] = current->n[2] * inverse;
    }
}
