#include <math.h>
#include "m3dMATR.h"

// m3dMATR::GetInvScale @ 0x82542F10
// Recover the reciprocal per-axis scale (1 / length of each of the three basis rows).
//
// CAVEAT: identical VMX shape to GetScale, but it stores the refined reciprocal-sqrt
// (1/length) directly instead of multiplying it back up to the length, and omits the
// zero-length vsel guard. Modelled here as scalar 1.0f/sqrtf. The handedness sign fix is
// reproduced literally from the fabs/fsel/fcmpu disassembly (0x82543008..0x825430A0).

// Negate `*invScale` when the dominant (largest-magnitude) row component is negative.
// max(|c0|,|c1|,|c2|) is never < 0, so the negation is unreachable as written; preserved
// verbatim to match the shipped fsel/fcmpu-with-0 sequence.
static void FlipScaleIfDominantNegative(float *invScale, float c0, float c1, float c2)
{
    float dominant = (fabsf(c1) - fabsf(c2) >= 0.0f) ? fabsf(c1) : fabsf(c2);
    dominant = (fabsf(c0) - dominant >= 0.0f) ? fabsf(c0) : dominant;
    if (dominant < 0.0f)
        *invScale = *invScale * -1.0f;
}

void m3dMATR::GetInvScale(float *invScaleX, float *invScaleY, float *invScaleZ) const
{
    float lenSqX = elements[0] * elements[0] + elements[1] * elements[1] + elements[2] * elements[2];
    float lenSqY = elements[4] * elements[4] + elements[5] * elements[5] + elements[6] * elements[6];
    float lenSqZ = elements[8] * elements[8] + elements[9] * elements[9] + elements[10] * elements[10];

    *invScaleX = 1.0f / sqrtf(lenSqX);
    *invScaleY = 1.0f / sqrtf(lenSqY);
    *invScaleZ = 1.0f / sqrtf(lenSqZ);

    // Triple product of the three basis rows == determinant of the upper-left 3x3.
    float determinant =
        elements[0] * (elements[5] * elements[10] - elements[6] * elements[9]) +
        elements[1] * (elements[6] * elements[8]  - elements[4] * elements[10]) +
        elements[2] * (elements[4] * elements[9]  - elements[5] * elements[8]);

    if (determinant <= 0.0f)
    {
        FlipScaleIfDominantNegative(invScaleX, elements[0], elements[1], elements[2]);
        FlipScaleIfDominantNegative(invScaleY, elements[4], elements[5], elements[6]);
        FlipScaleIfDominantNegative(invScaleZ, elements[8], elements[9], elements[10]);
    }
}
