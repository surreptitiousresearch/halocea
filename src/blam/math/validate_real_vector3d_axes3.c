/* validate_real_vector3d_axes3 @0x83819A50 */
/* validate_real_vector3d_axes3 0x83819A50 — repair a forward/left/up basis into a valid right-handed
 * orthonormal frame. Forward and up are normalized (falling back to the world defaults if degenerate), then
 * left is rebuilt as up x forward, up as left x forward, and left once more as up x forward, each normalized
 * with the same world-default fallback when the result collapses to zero length. The degenerate threshold is
 * 1e-4; a vector is also treated as degenerate when its computed length is exactly zero.
 *
 * DEVIATION: the decompiler folds each normalize-in-place into the conditional via the comma operator (the
 * division is performed, then `length == 0.0` is tested); reproduced verbatim so the fallback semantics match.
 * Intermediate doubles mirror the decompiler's float temporaries that cache vector components across the
 * in-place writes. */

#include "headers/real_vector3d.h"
#include "headers/ppc_intrinsics.h"
#include "headers/blam_data_globals.h"


void validate_real_vector3d_axes3(real_vector3d *forward, real_vector3d *left, real_vector3d *up)
{
    double forward_y, forward_z;
    double up_y, up_z;
    double forward_len, up_len, left_len;
    double left_x_cached, left_y_cached, left_z_cached;
    double up_x_cached, up_y_cached, up_z_cached;
    double a, b, c, d, e, f;

    /* normalize forward */
    forward_y = forward->n[1];
    forward_z = forward->n[2];
    forward_len = __fsqrts(((forward->n[2] * forward->n[2])
                         + ((forward->n[0] * forward->n[0]) + (forward->n[1] * forward->n[1]))));
    if ( __fabs(forward_len) < 0.000099999997
      || (forward->n[0] = forward->n[0] * ((float)1.0 / (float)forward_len),
          forward->n[1] = (float)forward_y * ((float)1.0 / (float)forward_len),
          forward->n[2] = (float)forward_z * ((float)1.0 / (float)forward_len),
          forward_len == 0.0) )
    {
        *forward = *global_forward3d;
    }

    /* normalize up */
    up_y = up->n[1];
    up_z = up->n[2];
    up_len = __fsqrts(((up->n[2] * up->n[2])
                    + ((up->n[0] * up->n[0]) + (up->n[1] * up->n[1]))));
    if ( __fabs(up_len) < 0.000099999997
      || (up->n[0] = up->n[0] * ((float)1.0 / (float)up_len),
          up->n[1] = (float)up_y * ((float)1.0 / (float)up_len),
          up->n[2] = (float)up_z * ((float)1.0 / (float)up_len),
          up_len == 0.0) )
    {
        *up = *global_up3d;
    }

    /* left = up x forward */
    a = forward->n[2];
    b = up->n[0];
    c = forward->n[1];
    d = (up->n[2] * forward->n[1]);
    e = up->n[1];
    f = (up->n[1] * forward->n[0]);
    left_x_cached = ((up->n[2] * forward->n[0]) - (up->n[0] * forward->n[2]));
    left->n[1] = (up->n[2] * forward->n[0]) - (up->n[0] * forward->n[2]);
    left_y_cached = (((float)e * (float)a) - (float)d);
    left->n[0] = ((float)e * (float)a) - (float)d;
    left_z_cached = (((float)b * (float)c) - (float)f);
    left->n[2] = left_z_cached;
    left_len = __fsqrts((((float)left_z_cached * (float)left_z_cached)
                      + (((float)left_y_cached * (float)left_y_cached)
                              + ((float)left_x_cached * (float)left_x_cached))));
    if ( __fabs(left_len) < 0.000099999997
      || (left->n[0] = (float)left_y_cached * ((float)1.0 / (float)left_len),
          left->n[1] = ((float)1.0 / (float)left_len) * (float)left_x_cached,
          left->n[2] = ((float)1.0 / (float)left_len) * (float)left_z_cached,
          left_len == 0.0) )
    {
        *left = *global_left3d;
    }

    /* up = left x forward */
    a = left->n[2];
    b = forward->n[0];
    c = left->n[1];
    d = (left->n[1] * forward->n[2]);
    e = forward->n[1];
    f = (left->n[0] * forward->n[1]);
    up_x_cached = ((left->n[0] * forward->n[2]) - (left->n[2] * forward->n[0]));
    up->n[1] = (left->n[0] * forward->n[2]) - (left->n[2] * forward->n[0]);
    up_y_cached = (((float)a * (float)e) - (float)d);
    up->n[0] = ((float)a * (float)e) - (float)d;
    up_z_cached = (((float)c * (float)b) - (float)f);
    up->n[2] = up_z_cached;
    up_len = __fsqrts((((float)up_z_cached * (float)up_z_cached)
                    + (((float)up_y_cached * (float)up_y_cached)
                            + ((float)up_x_cached * (float)up_x_cached))));
    if ( __fabs(up_len) < 0.000099999997
      || (up->n[0] = (float)up_y_cached * ((float)1.0 / (float)up_len),
          up->n[1] = ((float)1.0 / (float)up_len) * (float)up_x_cached,
          up->n[2] = ((float)1.0 / (float)up_len) * (float)up_z_cached,
          up_len == 0.0) )
    {
        *up = *global_up3d;
    }

    /* left = up x forward (final) */
    a = up->n[0];
    b = forward->n[2];
    c = forward->n[1];
    d = (up->n[2] * forward->n[1]);
    e = up->n[1];
    f = (up->n[1] * forward->n[0]);
    left_x_cached = ((up->n[2] * forward->n[0]) - (up->n[0] * forward->n[2]));
    left->n[1] = (up->n[2] * forward->n[0]) - (up->n[0] * forward->n[2]);
    left_y_cached = (((float)e * (float)b) - (float)d);
    left->n[0] = ((float)e * (float)b) - (float)d;
    left_z_cached = (((float)a * (float)c) - (float)f);
    left->n[2] = left_z_cached;
    left_len = __fsqrts((((float)left_z_cached * (float)left_z_cached)
                      + (((float)left_y_cached * (float)left_y_cached)
                              + ((float)left_x_cached * (float)left_x_cached))));
    if ( __fabs(left_len) < 0.000099999997
      || (left->n[0] = (((float)e * (float)b) - (float)d) * ((float)1.0 / (float)left_len),
          left->n[1] = ((float)1.0 / (float)left_len) * (float)left_x_cached,
          left->n[2] = ((float)1.0 / (float)left_len) * (float)left_z_cached,
          left_len == 0.0) )
    {
        *left = *global_left3d;
    }
}
