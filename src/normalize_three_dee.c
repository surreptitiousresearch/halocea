/* normalize_three_dee @0x8381B594 — normalize a 3D vector in place; returns its original length, or 0.0 if
 * the reciprocal length is NaN or the normalized result fails valid_real_normal3d.
 *
 * DEVIATION: the decompiler fabricated a phantom second parameter ("long double a2") purely to hold the
 * computed reciprocal length across a stack round-trip; the real prototype (funcs.prototype) takes only
 * `vector`. Rewritten without it. */

#include <stdint.h>
#include "headers/real_vector3d.h"

extern float __fsqrts(float x);
extern int _isnan(double x);
extern uint8_t valid_real_normal3d(const real_vector3d *n);

float normalize_three_dee(real_vector3d *vector)
{
    float length = __fsqrts(vector->n[0] * vector->n[0] + vector->n[1] * vector->n[1] + vector->n[2] * vector->n[2]);
    float inv_length = 1.0f / length;

    if ( _isnan(inv_length) )
        return 0.0f;

    vector->n[0] = vector->n[0] * inv_length;
    vector->n[1] = vector->n[1] * inv_length;
    vector->n[2] = vector->n[2] * inv_length;

    return valid_real_normal3d(vector) ? length : 0.0f;
}
