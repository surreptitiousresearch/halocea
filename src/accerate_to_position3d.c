/* accerate_to_position3d @0x837006C8 — accelerate `velocity` toward `position_desired`: compute the
 * direction and distance to the target, derive a desired velocity magnitude of sqrt(2*distance*
 * acceleration_maximum) along that direction (the speed that lets a decelerating approach arrive exactly
 * at the target), and hand it to accelerate_to_velocity3d. If the target is already reached (distance below
 * epsilon) and the accel step also reports arrival, snaps `position` directly to `position_desired` and
 * returns 1 (arrived); otherwise integrates `position` by the current `velocity` and returns 0.
 *
 * DEVIATION: the decompiler loses track of which locals are the `position`/`velocity`/`position_desired`
 * pointer parameters (`v12`/`v13`/`v14`, used but never assigned in its own view) — resolved via disasm
 * register tracing (r7=position, r6=velocity, r5=position_desired, preserved untouched across the
 * intervening calls). `velocity_maximum` is confirmed genuinely unused: accelerate_to_velocity3d's own DB
 * signature has no such parameter, and disasm shows f1 (where it would be passed) is never read here. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern float __fsqrts(float x);
extern float __fabs(float x);
extern uint8_t accelerate_to_velocity3d(real_vector3d *velocity, const real_vector3d *velocity_desired, float acceleration_maximum);

unsigned __int8 accerate_to_position3d(real_point3d *position, real_vector3d *velocity,
    real_point3d *position_desired, float velocity_maximum, float acceleration_maximum)
{
    (void)velocity_maximum; /* confirmed unused: accelerate_to_velocity3d has no such parameter */

    float dx = position_desired->n[0] - position->n[0];
    float dy = position_desired->n[1] - position->n[1];
    float dz = position_desired->n[2] - position->n[2];
    float distance = __fsqrts(dx * dx + dz * dz + dy * dy);

    real_vector3d velocity_desired;

    if ( __fabs(distance) < 0.0001f )
    {
        velocity_desired.n[0] = 0.0f;
        velocity_desired.n[1] = 0.0f;
        velocity_desired.n[2] = 0.0f;
        distance = 0.0f;
    }
    else
    {
        float inverse_distance = 1.0f / distance;
        float direction_x = inverse_distance * dx;
        float direction_y = inverse_distance * dy;
        float direction_z = dz * inverse_distance;

        if ( distance != 0.0f )
        {
            float approach_speed = __fsqrts(distance * acceleration_maximum * 2.0f);
            velocity_desired.n[0] = direction_x * approach_speed;
            velocity_desired.n[1] = approach_speed * direction_y;
            velocity_desired.n[2] = direction_z * approach_speed;
        }
        else
        {
            velocity_desired.n[0] = 0.0f;
            velocity_desired.n[1] = 0.0f;
            velocity_desired.n[2] = 0.0f;
        }
    }

    if ( accelerate_to_velocity3d(velocity, &velocity_desired, acceleration_maximum) && distance == 0.0f )
    {
        *position = *position_desired;
        return 1;
    }

    position->n[0] += velocity->n[0];
    position->n[1] += velocity->n[1];
    position->n[2] += velocity->n[2];
    return 0;
}
