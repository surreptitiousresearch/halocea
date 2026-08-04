#include <math.h>
#include "m3d_boundary.h"

// m3dAngleVector_COS(const m3dV*, const m3dV*) @ 0x8265A3F8
// Cosine of the angle between two vectors: dot(vecA,vecB) / (|vecA|*|vecB|), clamped to
// [-1,1] to guard acos() callers against FP rounding pushing the ratio slightly out of
// range. Falls back to 1.0 (angle == 0) when either vector's magnitude is (near) zero.
float m3dAngleVector_COS(const m3dV *vecA, const m3dV *vecB)
{
    float magProduct = m3dLengthVector(vecA) * m3dLengthVector(vecB);

    if (fabsf(magProduct) >= 0.000001f)
    {
        float dot = vecA->x * vecB->x + vecA->z * vecB->z + vecA->y * vecB->y;
        return _m3dClamp(-1.0f, 1.0f, dot / magProduct);
    }

    return 1.0f;
}
