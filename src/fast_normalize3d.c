/* fast_normalize3d @ 0x836FBFC8 — scale a vector to unit length in place (left unchanged if it is the
 * zero vector).
 * Return attested void: no r3 write in the body and the only caller (sphere_test_vector3d) ignores r3
 * — the decompiler's "returns v" is r3 merely still holding the argument. */

#include "headers/real_vector3d.h"

extern double __fsqrts(double x);

void fast_normalize3d(real_vector3d *v)
{
    float length_squared = v->n[2] * v->n[2] + (v->n[0] * v->n[0] + v->n[1] * v->n[1]);

    if ( length_squared != 0.0f )
    {
        float inv_length = 1.0f / (float)__fsqrts(length_squared);
        v->n[0] = v->n[0] * inv_length;
        v->n[1] = v->n[1] * inv_length;
        v->n[2] = v->n[2] * inv_length;
    }
}
