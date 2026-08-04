#include <math.h>
#include "m3d_boundary.h"

// m3dLengthVector(const m3dV*) @ 0x82561B10
// Euclidean length of a vector.
float m3dLengthVector(const m3dV *v)
{
    _m3dCheckValid(v);

    float x = v->x;
    _m3dCheckValid(x);
    float y = v->y;
    _m3dCheckValid(y);
    _m3dCheckValid(v->z);

    float lengthSq = x * x + v->z * v->z + y * y;
    _m3dCheckValid(lengthSq);

    return sqrtf(lengthSq);
}
