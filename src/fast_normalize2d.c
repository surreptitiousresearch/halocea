/* fast_normalize2d @0x836FBE48 — normalize a 2D vector in place using a fast reciprocal-sqrt of the
 * squared length. A zero-length vector is left unchanged. Returns the (mutated) vector. */

#include "headers/real_vector2d.h"

extern float __fsqrts(float x); /* PPC single-precision sqrt intrinsic */

real_vector2d *fast_normalize2d(real_vector2d *v)
{
    float length_squared = (v->n[0] * v->n[0]) + (v->n[1] * v->n[1]);

    if ( length_squared != 0.0f )
    {
        float inverse_length = 1.0f / __fsqrts(length_squared);
        v->n[0] = v->n[0] * inverse_length;
        v->n[1] = v->n[1] * inverse_length;
    }
    return v;
}
