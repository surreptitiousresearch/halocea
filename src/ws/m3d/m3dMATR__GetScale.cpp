#include <math.h>
#include "m3dMATR.h"

// m3dMATR::GetScale @ 0x825423A8
// Recover the per-axis scale factor (Euclidean length of each of the three basis rows).
//
// CAVEAT: the shipped routine is VMX. It computes each row's squared length with
// vmsum3fp128, forms 1/length with vrsqrtefp128 + one Newton-Raphson refinement step,
// multiplies by the squared length to get the length, and selects the raw squared length
// (== 0) for zero-length rows via vcmpeqfp128/vsel to dodge the reciprocal-sqrt of zero.
// Modelled here as scalar sqrtf (sqrtf(0)==0 reproduces the zero guard exactly).
// The handedness fix (below) is reproduced literally from the fabs/fsel/fcmpu disassembly.

// Negate `*scale` when the dominant (largest-magnitude) row component is negative.
// The binary selects max(|c0|,|c1|,|c2|) via two fsels and compares it against 0.0; because
// that maximum of absolute values is never < 0 the negation is unreachable as written, but
// it is preserved verbatim to match 0x825424D4..0x8254256C.
static void FlipScaleIfDominantNegative(float *scale, float c0, float c1, float c2)
{
    float dominant = (fabsf(c1) - fabsf(c2) >= 0.0f) ? fabsf(c1) : fabsf(c2);
    dominant = (fabsf(c0) - dominant >= 0.0f) ? fabsf(c0) : dominant;
    if (dominant < 0.0f)
        *scale = *scale * -1.0f;
}

void m3dMATR::GetScale(float *scaleX, float *scaleY, float *scaleZ) const
{
    float lenSqX = elements[0] * elements[0] + elements[1] * elements[1] + elements[2] * elements[2];
    float lenSqY = elements[4] * elements[4] + elements[5] * elements[5] + elements[6] * elements[6];
    float lenSqZ = elements[8] * elements[8] + elements[9] * elements[9] + elements[10] * elements[10];

    *scaleX = sqrtf(lenSqX);
    *scaleY = sqrtf(lenSqY);
    *scaleZ = sqrtf(lenSqZ);

    // Triple product of the three basis rows == determinant of the upper-left 3x3.
    float determinant =
        elements[0] * (elements[5] * elements[10] - elements[6] * elements[9]) +
        elements[1] * (elements[6] * elements[8]  - elements[4] * elements[10]) +
        elements[2] * (elements[4] * elements[9]  - elements[5] * elements[8]);

    if (determinant <= 0.0f)
    {
        FlipScaleIfDominantNegative(scaleX, elements[0], elements[1], elements[2]);
        FlipScaleIfDominantNegative(scaleY, elements[4], elements[5], elements[6]);
        FlipScaleIfDominantNegative(scaleZ, elements[8], elements[9], elements[10]);
    }
}
