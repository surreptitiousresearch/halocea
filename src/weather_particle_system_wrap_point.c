/* weather_particle_system_wrap_point @0x8373C548 — wrap `point` into a [0, box_width) cube, per axis:
 * result = fmod(point, box_width), plus box_width if point was negative (so the result lands in
 * [0, box_width) whenever box_width > 0, matching fmod's sign-of-dividend behavior).
 *
 * DEVIATION: the decompiler fabricated three extra parameters (a4/a5/a6) out of stack-spilled copies of the
 * single float `box_width` argument and PPC64 fmod-argument-pair renderings; disasm (0x8373C548-0x8373C600)
 * confirms the real signature is exactly the 3-parameter one in funcs.prototype (box_width in f1, point in
 * r4, result in r5), matching the decompiled body's actual reads (point->n[i]) and writes (result->n[i]). */

#include "headers/real_point3d.h"

extern double fmod(double x, double y);

void weather_particle_system_wrap_point(float box_width, const real_point3d *point, real_point3d *result)
{
    for ( int i = 0; i < 3; ++i )
    {
        float wrapped = (float)fmod(point->n[i], box_width);
        if ( point->n[i] < 0.0f )
            wrapped += box_width;
        result->n[i] = wrapped;
    }
}
