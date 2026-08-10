/* biped_build_flying_axes @0x837AED38 — build an orthonormal (left, up) basis for a flying biped from its
 * forward vector. left = global_up x forward, normalized; if that degenerates (forward nearly vertical) it
 * falls back to global_forward x forward. up is then rebuilt as forward x left and normalized. Matches the
 * classic "up reference, cross to get side, cross back to get true up" construction, with the tiny-length
 * (1e-4) guards preserving the previous vector when a cross product collapses. */

#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
#include "headers/ppc_intrinsics.h"

extern float normalize3d(real_vector3d *v);

void biped_build_flying_axes(const real_vector3d *forward_vector, real_vector3d *left_vector, real_vector3d *up_vector)
{
    up_vector->n[0] = global_up3d->n[0];
    up_vector->n[1] = global_up3d->n[1];
    up_vector->n[2] = global_up3d->n[2];
    float up_x = up_vector->n[0];
    float up_y = up_vector->n[1];

    left_vector->n[1] = (up_vector->n[2] * forward_vector->n[0]) - (forward_vector->n[2] * up_x);
    left_vector->n[0] = (forward_vector->n[2] * up_y) - (up_vector->n[2] * forward_vector->n[1]);
    left_vector->n[2] = (up_x * forward_vector->n[1]) - (forward_vector->n[0] * up_y);

    float left_length = __fsqrts((((left_vector->n[2] * left_vector->n[2])
        + ((left_vector->n[0] * left_vector->n[0]) + (left_vector->n[1] * left_vector->n[1])))));

    if ( __fabs(left_length) < 0.000099999997 )
    {
        /* forward nearly parallel to global up: fall back to global forward as the reference */
        up_vector->n[0] = global_forward3d->n[0];
        up_vector->n[1] = global_forward3d->n[1];
        up_vector->n[2] = global_forward3d->n[2];
        float ref_x = up_vector->n[0];
        float ref_y = up_vector->n[1];
        left_vector->n[2] = (forward_vector->n[1] * ref_x) - (forward_vector->n[0] * ref_y);
        left_vector->n[0] = (forward_vector->n[2] * ref_y) - (forward_vector->n[1] * up_vector->n[2]);
        left_vector->n[1] = (up_vector->n[2] * forward_vector->n[0]) - (forward_vector->n[2] * ref_x);
        normalize3d(left_vector);
    }
    else
    {
        float inverse_length = (float)1.0 / left_length;
        left_vector->n[0] = left_vector->n[0] * inverse_length;
        left_vector->n[1] = inverse_length * left_vector->n[1];
        left_vector->n[2] = inverse_length * left_vector->n[2];
    }

    up_vector->n[1] = (forward_vector->n[2] * left_vector->n[0]) - (left_vector->n[2] * forward_vector->n[0]);
    up_vector->n[0] = (forward_vector->n[1] * left_vector->n[2]) - (forward_vector->n[2] * left_vector->n[1]);
    up_vector->n[2] = (left_vector->n[1] * forward_vector->n[0]) - (forward_vector->n[1] * left_vector->n[0]);

    float up_length = __fsqrts((((up_vector->n[2] * up_vector->n[2])
        + ((up_vector->n[0] * up_vector->n[0]) + (up_vector->n[1] * up_vector->n[1])))));

    if ( __fabs(up_length) >= 0.000099999997 )
    {
        float inverse_length = (float)1.0 / up_length;
        up_vector->n[0] = up_vector->n[0] * inverse_length;
        up_vector->n[1] = inverse_length * up_vector->n[1];
        up_vector->n[2] = inverse_length * up_vector->n[2];
    }
}
