/* projectile_distribute @0x836D9F18 — compute one projectile's angular offset within a symmetric spread
 * of `count` projectiles at position `index`, and (only when distribution_function == 1) rotate
 * forward/up about the spread axis by that angle. For odd counts, index 0 is the centered element (angle
 * 0); other indices alternate sign around ((index-1)/2) steps. For even counts, there's no exact center:
 * offset is (index/2 - 0.5) steps, negated when index is odd.
 *
 * DEVIATION: the decompiler fails local-variable allocation and renders this with two phantom __int64
 * parameters and a double distribution_angle — the DB's real 6-parameter prototype (distribution_function,
 * distribution_angle:float, index, count all __int16/float, no 64-bit params) is confirmed by disasm
 * register tracing: distribution_angle (f1) shadows r6, pushing index to r7 and count to r8. */

#include <stdint.h>
#include "headers/real_vector3d.h"
#include "headers/trigger_distribution_function.h"

extern double cos(double x);
extern double sin(double x);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);

void projectile_distribute(real_vector3d *forward, real_vector3d *up, int16_t distribution_function, float distribution_angle, int16_t index, int16_t count)
{
    float steps;

    if ( count & 1 )
    {
        if ( index == 0 )
        {
            steps = 0.0f;
        }
        else
        {
            __int16 offset = (index - 1) >> 1;
            steps = ((index - 1) & 1) == 0 ? -(float)offset : (float)offset;
        }
    }
    else
    {
        steps = (float)(index >> 1) - 0.5f;
        if ( (index & 1) != 0 )
            steps = -steps;
    }

    float angle = steps * distribution_angle;
    float cosine = (float)cos(angle);
    float sine = (float)sin(angle);

    if ( distribution_function == _trigger_distribution_horizontal_fan )
        rotate_vector_about_axis(forward, up, sine, cosine);
}
