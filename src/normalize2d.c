/* normalize2d @0x8369DC80 — normalise a 2D vector in place and return its original length. A degenerate
 * (near-zero) vector is left unchanged and a length of 0 is returned. */

#include "headers/real_vector2d.h"
extern float __fsqrts(float);
extern float __fabs(float);

float normalize2d(real_vector2d *v)
{
    float length = __fsqrts(v->n[0] * v->n[0] + v->n[1] * v->n[1]);
    if ( __fabs(length) < 0.000099999997f )
        return 0.0f;
    v->n[0] = v->n[0] * (1.0f / length);
    v->n[1] = v->n[1] * (1.0f / length);
    return length;
}
