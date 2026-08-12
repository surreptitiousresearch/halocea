/* FUNCTION_INDEX entry: hkVector4_allComponentsWithin @0x82ECC070 (?equals3@hkVector4@@QBAHABV1@M@Z) */
#include "../headers/havok/hkVector4.h"

#ifndef HK_MATH_FABSF
#define HK_MATH_FABSF(x) ((x) < 0.0f ? -(x) : (x))
#endif

/* Returns non-zero iff every xyz component of a is within epsilon of the
   matching component of b (|a[i]-b[i]| <= epsilon). This is hkVector4::equals3;
   the out-of-line body is the VMX128 form (vsubfp128, vand128 with
   hkVector4::setAbs4's positiveMask, vcmpgefp128 against the splatted epsilon,
   then vor128 with hkVector4Comparison::s_invMaskFromBits[0xE0/16] — the mask
   that ignores the w lane — and an all-lanes-true vcmpequw128.). Only the xyz
   lanes are compared, which is what selects the 0xE0 mask over equals4's 0xF0.
   DEVIATION: reconstructed as a scalar loop; the frontier caller
   (decorateConnection, comparing decoration normals) reaches it via the
   extern. */
int hkVector4_allComponentsWithin(const hkVector4 *a, const hkVector4 *b, float epsilon)
{
    int i;
    for (i = 0; i < 3; ++i)
    {
        float delta = a->m_quad.___u0.v[i] - b->m_quad.___u0.v[i];
        if (HK_MATH_FABSF(delta) > epsilon)
            return 0;
    }
    return 1;
}
