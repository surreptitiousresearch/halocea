/* physics_variable_update_seek @0x83810AD0 — advance a seek-toward-target scalar by one physics tick,
 * accelerating in whichever direction closes the gap to `desired_position` (for a cyclical range, taking
 * the shorter way around if the direct gap exceeds half the range), then clamping/wrapping `position` into
 * [minimum_position, maximum_position] exactly like physics_variable_update.c's plain (non-seeking)
 * sibling. If the direction needed to close the *remaining* gap after this tick's move is still the same
 * as the direction just used (i.e. the target wasn't reached or overshot this tick), returns 0; otherwise
 * (gap closed to zero, or the target was overshot/direction flipped) snaps `position` to
 * `desired_position`, zeroes `speed`, and returns 1.
 *
 * DEVIATION: the decompiler shows `position`/`speed`/`definition` as used-but-never-assigned locals
 * (`v7`/`v8`/`v14`) and a boolean `v13` with no visible assignment — same class of failure as
 * physics_variable_update.c's own already-documented deviation. Resolved via disasm register tracing:
 * r10=position, r9=speed, r5=definition (passed straight through, its +8 sub-view handed to
 * physics_variable_speed_update exactly as in the sibling), r8=(unsigned char)cyclical_position. */

#include <stdint.h>
#include "headers/physics_variable.h"
#include "headers/physics_variable_speed.h"

extern float __fabs(float x);
extern void physics_variable_speed_update(float *speed, const physics_variable_speed *definition, float magnitude);

uint8_t physics_variable_update_seek(float *position, float *speed, const physics_variable *definition, uint8_t cyclical_position, float desired_position, float magnitude)
{
    unsigned __int8 arrived = 1;
    float delta = desired_position - *position;

    if ( delta != 0.0f )
    {
        if ( cyclical_position
          && __fabs(delta) > (definition->maximum_position - definition->minimum_position) * 0.5f )
            delta = -delta;

        float direction = delta <= 0.0f ? -1.0f : 1.0f;

        physics_variable_speed_update(speed, (const physics_variable_speed *)&definition->maximum_speed,
            direction * magnitude);

        *position += *speed;

        if ( *position < definition->minimum_position )
        {
            if ( cyclical_position )
                *position += definition->maximum_position - definition->minimum_position;
            else
                *position = definition->minimum_position;
        }
        else if ( *position > definition->maximum_position )
        {
            if ( cyclical_position )
                *position -= definition->maximum_position - definition->minimum_position;
            else
                *position = definition->maximum_position;
        }

        float remaining = desired_position - *position;
        if ( remaining != 0.0f )
        {
            if ( cyclical_position
              && __fabs(remaining) > (definition->maximum_position - definition->minimum_position) * 0.5f )
                remaining = -remaining;

            float remaining_direction = remaining <= 0.0f ? -1.0f : 1.0f;

            if ( direction == remaining_direction )
                arrived = 0;
        }
    }

    if ( arrived )
    {
        *position = desired_position;
        *speed = 0.0f;
    }

    return arrived;
}
