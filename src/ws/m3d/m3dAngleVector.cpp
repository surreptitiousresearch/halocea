#include <math.h>
#include "m3d_boundary.h"

// m3dAngleVector(const m3dV*, const m3dV*) @ 0x8265A4A0
// Angle in degrees between two vectors: acos(cosine) converted from radians to degrees.
float m3dAngleVector(const m3dV *vecA, const m3dV *vecB)
{
    float cosine = m3dAngleVector_COS(vecA, vecB);
    return static_cast<float>(acos(static_cast<double>(cosine)) * 57.295776);
}
