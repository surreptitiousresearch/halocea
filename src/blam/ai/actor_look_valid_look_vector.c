/* actor_look_valid_look_vector @0x837FAD30 — tests whether a candidate (attempted) look direction is an
 * acceptable place for an actor to look. The reference look_vector defines a unit frame; the attempted
 * direction must (a) project onto the aiming vector beyond yaw_deviation, and (b) lie inside the cone
 * whose left/right cosine limits are cone_limits->n[0]/n[1] (selected by which side of the frame the
 * attempt falls on). Returns 1 if valid, 0 otherwise.
 *
 * DEVIATION: yaw_deviation is passed single-precision (caller lfs f1,0x134(r30)), so it is a float; the
 * ABI still skips the r3 GPR slot for the leading float, and the decompiler invented a phantom r3
 * "look_delta_deviations" parameter — dropped. The COERCE_FLOAT("nit") artifacts are plain float loads
 * of look_vector's components. */

#include <stdint.h>
#include "headers/real_vector2d.h"

extern float __fsqrts(float x);
extern double __fabs(double x);
extern float normalize2d(real_vector2d *v);

uint8_t actor_look_valid_look_vector(float yaw_deviation, const real_vector2d *cone_limits,
                                 const real_vector2d *aiming_vector,
                                 const real_vector2d *attempted_looking_vector,
                                 const real_vector2d *look_vector)
{
    float magnitude = __fsqrts(look_vector->n[0] * look_vector->n[0]
                               + look_vector->n[1] * look_vector->n[1]);
    if (__fabs(magnitude) < 0.000099999997f)
        return 0;

    float unit_x = (1.0f / magnitude) * look_vector->n[0];
    float unit_y = (1.0f / magnitude) * look_vector->n[1];
    if (magnitude <= 0.0f)
        return 0;

    if (aiming_vector->n[0] * unit_x + aiming_vector->n[1] * unit_y <= yaw_deviation)
        return 0;

    real_vector2d attempted = *attempted_looking_vector;
    if (normalize2d(&attempted) <= 0.0f)
        return 0;

    float cross = unit_x * attempted.n[1] - attempted.n[0] * unit_y;
    int side = (cross > 0.0f) ? 1 : 0;
    float threshold = cone_limits->n[side];

    float alignment = attempted.n[0] * unit_x + attempted.n[1] * unit_y;
    if (alignment <= threshold)
        return 0;
    return 1;
}
