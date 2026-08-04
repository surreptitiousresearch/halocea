/* accelerate_to_velocity3d @0x83700628 — move `velocity` toward `velocity_desired` at up to
 * acceleration_maximum per tick. Returns 1 and snaps directly to velocity_desired if the remaining distance
 * is within one tick's acceleration; otherwise steps velocity by acceleration_maximum along the direction to
 * velocity_desired and returns 0. */

#include <stdint.h>
#include "headers/real_vector3d.h"

extern float __fsqrts(float x);

uint8_t accelerate_to_velocity3d(real_vector3d *velocity, const real_vector3d *velocity_desired, float acceleration_maximum)
{
    float dx = velocity_desired->n[0] - velocity->n[0];
    float dy = velocity_desired->n[1] - velocity->n[1];
    float dz = velocity_desired->n[2] - velocity->n[2];
    float distance_squared = dx * dx + dy * dy + dz * dz;

    if ( distance_squared <= acceleration_maximum * acceleration_maximum )
    {
        *velocity = *velocity_desired;
        return 1;
    }

    float distance = __fsqrts(distance_squared);
    float scale = acceleration_maximum / distance;

    velocity->n[0] += scale * dx;
    velocity->n[1] += scale * dy;
    velocity->n[2] += scale * dz;
    return 0;
}
