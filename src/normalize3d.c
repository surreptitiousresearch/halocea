/* normalize3d @ 0x836A7688 — scale a 3D vector to unit length in place and return its original length.
 * A length below ~1e-4 is treated as zero: the vector is left unchanged and 0 is returned. */

#include "headers/real_vector3d.h"

extern float __fsqrts(float x);
extern float __fabs(float x);

float normalize3d(real_vector3d *v)
{
    float length = __fsqrts(v->n[2] * v->n[2] + (v->n[0] * v->n[0] + v->n[1] * v->n[1]));
    if ( __fabs(length) < 0.000099999997f )
        return 0.0f;

    float inverse_length = 1.0f / length;
    v->n[0] = v->n[0] * inverse_length;
    v->n[1] = v->n[1] * inverse_length;
    v->n[2] = v->n[2] * inverse_length;
    return length;
}
