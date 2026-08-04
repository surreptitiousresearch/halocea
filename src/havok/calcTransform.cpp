/* =========================================================================
   NOTE / CAVEAT (read before trusting this file):
   Reconstructed from a decompile that Hex-Rays itself flagged
   "local variable allocation has failed, the output may be wrong". The dense
   VMX128 4x4 matrix-concatenation blocks referenced unresolvable garbage
   registers (v89..v93), and the `rand` parameter was aliased by the decompiler
   with a scratch scale matrix (it is really an hkPseudoRandomGenerator whose
   32-bit seed the LCG 1664525*s+1013904223 advances). What is traced faithfully
   here: the per-axis random scale factors, the random shear/shift offsets and
   which SplittingData fields feed them, the calculatePlaneTransform orientation,
   the near-degenerate fallback, and the final splitter-AABB translation. The
   exact ordering of the intermediate VMX matrix multiplies is BEST-EFFORT and
   should be re-derived from disasm (0x8384DE38) before being relied upon.
   ========================================================================= */
#include "../headers/havok/hkdWoodFracture_SplittingData.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkAabb.h"
#include "../headers/havok/hkMatrix4.h"

typedef struct hkPseudoRandomGenerator hkPseudoRandomGenerator;

extern const hkVector4 hkVector4Zero;
extern void hkdFractureUtil_calculatePlaneTransform(const hkVector4 *origin, const hkVector4 *normalAxis, hkMatrix4 *out); /* hkdFractureUtil::calculatePlaneTransform */

/* The generator holds its 32-bit seed at byte offset 4 (matches
   *(unsigned int *)(HIDWORD(rand) + 4) in the decompile). */
static unsigned int rng_next(hkPseudoRandomGenerator *gen)
{
    unsigned int *seed = (unsigned int *)((char *)gen + 4);
    *seed = 1664525u * *seed + 1013904223u;
    return *seed;
}

/* Uniform random in [-1, 1): seed * 2^-32 in [0,1), then *2 - 1. */
static float rng_signed(hkPseudoRandomGenerator *gen)
{
    return (float)rng_next(gen) * 2.3283064e-10f * 2.0f - 1.0f;
}

/* Asymmetric blend used for every scale/shear term: negative deltas are damped
   harder (x*0.25) than positive ones (x*0.5). */
static float damp(float x)
{
    return (x <= 0.0f) ? x * 0.25f : x * 0.5f;
}

void calcTransform(const hkdWoodFracture_SplittingData *splittingData, const hkVector4 *normalAxis,
                   const hkAabb *objectAabb, const hkAabb *splitterAabb,
                   hkPseudoRandomGenerator *rand, hkMatrix4 *matrixOut)
{
    hkMatrix4 shear;
    float scale[3];
    float extents[4];
    int i;

    /* per-axis randomized scale: scale[i] = (damp(rand[-1,1] * scaleRange[i]) + 1) * scale[i] */
    for (i = 0; i < 3; ++i)
        scale[i] = (damp(rng_signed(rand) * splittingData->m_scaleRange.m_quad.___u0.v[i]) + 1.0f) * splittingData->m_scale.m_quad.___u0.v[i];

    /* object AABB half-extents feed the shift terms below */
    for (i = 0; i < 4; ++i)
        extents[i] = objectAabb->m_max.m_quad.___u0.v[i] - objectAabb->m_min.m_quad.___u0.v[i];

    /* start the shear/scale matrix as identity with the randomized scale diagonal */
    for (i = 0; i < 4; ++i)
    {
        shear.m_col0.m_quad.___u0.v[i] = 0.0f;
        shear.m_col1.m_quad.___u0.v[i] = 0.0f;
        shear.m_col2.m_quad.___u0.v[i] = 0.0f;
        shear.m_col3.m_quad.___u0.v[i] = 0.0f;
    }
    shear.m_col0.m_quad.___u0.v[0] = scale[0];
    shear.m_col1.m_quad.___u0.v[1] = scale[1];
    shear.m_col2.m_quad.___u0.v[2] = scale[2];
    shear.m_col3.m_quad.___u0.v[3] = 1.0f;

    /* randomized shear entries. DEVIATION: exact matrix slots are approximate
       (reg-alloc garbage); values/fields are from the traced expressions. */
    shear.m_col3.m_quad.___u0.v[1] = damp(rng_signed(rand) * splittingData->m_splitGeomShiftRangeZ) * scale[1] * scale[2];
    shear.m_col3.m_quad.___u0.v[2] = damp(rng_signed(rand) * splittingData->m_splitGeomShiftRangeY) * scale[0] * scale[2];
    shear.m_col1.m_quad.___u0.v[0] = rng_signed(rand) * splittingData->m_fractureLineShearingRange;
    shear.m_col2.m_quad.___u0.v[0] = rng_signed(rand) * splittingData->m_surfaceNormalShearingRange;
    shear.m_col2.m_quad.___u0.v[1] = rng_signed(rand) * splittingData->m_fractureNormalShearingRange;

    /* orient the shear frame to the fracture normal */
    hkdFractureUtil_calculatePlaneTransform(&hkVector4Zero, normalAxis, &shear);

    /* near-degenerate guard: if the in-plane axes are tiny relative to the
       fracture-line axis, fall back toward an identity orientation */
    {
        float a = __fabs(shear.m_col2.m_quad.___u0.v[0]) + __fabs(shear.m_col3.m_quad.___u0.v[0]);
        float b = (__fabs(shear.m_col1.m_quad.___u0.v[0]) + __fabs(shear.m_col2.m_quad.___u0.v[1])) * 0.1f;
        if (a < b)
        {
            shear.m_col0.m_quad.___u0.v[0] = 1.0f;
            shear.m_col3.m_quad.___u0.v[3] = 1.0f;
        }
    }

    /* compose into the output and translate by the splitter AABB */
    *matrixOut = shear;
    for (i = 0; i < 4; ++i)
        matrixOut->m_col3.m_quad.___u0.v[i] += splitterAabb->m_min.m_quad.___u0.v[i];
    matrixOut->m_col3.m_quad.___u0.v[3] = 1.0f;
}
