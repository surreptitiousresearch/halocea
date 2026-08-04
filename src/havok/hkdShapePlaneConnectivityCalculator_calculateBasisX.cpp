#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkTransform.h"

extern float sqrtf(float x);

/* Build an orthonormal transform whose first rotation axis is `normal`:
   col0 = normal, col1 = a normalized perpendicular (tangent), col2 =
   normal x tangent, translation = pos. The perpendicular is the classic
   swap-and-negate of two components, skipping the smallest-magnitude axis for
   numerical stability. DEVIATION: the original is VMX128 (vpermwi128 cross-
   product shuffles 0x63=yzx / 0x87=zxy, and vrsqrtefp with two Newton-Raphson
   refinements for the normalize); reconstructed as scalar math with a sqrt plus
   the zero-length guard that matches the original's vsel. The integer lane
   selection is preserved exactly from the decompiler. */
extern "C" void hkdShapePlaneConnectivityCalculator_calculateBasisX(const hkVector4 *normal,
                                                          const hkVector4 *pos,
                                                          hkTransform *trans)
{
    const float *n = normal->m_quad.___u0.v; /* hkVector4 lanes via m_quad.___u0.v */
    int argmin = 0, laneA = 1, laneB = 2; /* v4, v7, v9 */
    float smallest = __fabs(n[0]);
    float perp[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float dot, invLen;
    hkVector4 *tangent = &trans->m_rotation.m_col1;
    int r;

    if (__fabs(n[1]) < smallest) { laneA = 0; smallest = __fabs(n[1]); argmin = 1; }
    if (__fabs(n[2]) < smallest) laneB = argmin;

    /* perpendicular: swap two lanes and negate one, zero the rest */
    perp[laneA] = n[laneB];
    perp[laneB] = -n[laneA];

    dot = perp[0] * perp[0] + perp[1] * perp[1] + perp[2] * perp[2];
    invLen = (dot != 0.0f) ? 1.0f / sqrtf(dot) : 0.0f;

    trans->m_rotation.m_col0 = *normal;                 /* col0 = normal */
    for (r = 0; r < 4; ++r)
        tangent->m_quad.___u0.v[r] = perp[r] * invLen;               /* col1 = tangent */

    trans->m_rotation.m_col2.m_quad.___u0.v[0] = n[1] * tangent->m_quad.___u0.v[2] - n[2] * tangent->m_quad.___u0.v[1]; /* col2 = normal x tangent */
    trans->m_rotation.m_col2.m_quad.___u0.v[1] = n[2] * tangent->m_quad.___u0.v[0] - n[0] * tangent->m_quad.___u0.v[2];
    trans->m_rotation.m_col2.m_quad.___u0.v[2] = n[0] * tangent->m_quad.___u0.v[1] - n[1] * tangent->m_quad.___u0.v[0];
    trans->m_rotation.m_col2.m_quad.___u0.v[3] = 0.0f;

    trans->m_translation = *pos;                        /* translation = pos */
}
