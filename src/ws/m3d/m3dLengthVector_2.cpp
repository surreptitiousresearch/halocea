#include "m3d_boundary.h"

// m3dLengthVector_2(const m3dV*) @ 0x825418A8
// Returns the SQUARED Euclidean length of a vector.
//
// Deviation from the "LengthVector" name: the disassembly (0x825418F0-0x825418F8:
// fmuls/fmadds/fmadds, then blr) computes x*x + (z*z + y*y) and returns it directly.
// There is NO fsqrt — unlike the sibling m3dLengthVector @0x82561B10 which does sqrtf.
// Transcribed faithfully as the raw sum of squares.
float m3dLengthVector_2(const m3dV *v)
{
    _m3dCheckValid(v);

    float x = v->x;
    _m3dCheckValid(x);
    float y = v->y;
    _m3dCheckValid(y);
    float z = v->z;
    _m3dCheckValid(z);

    return x * x + (z * z + y * y);
}
