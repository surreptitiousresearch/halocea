#include "../headers/havok/hkVector4.h"

#ifndef HK_MATH_FABSF
#define HK_MATH_FABSF(x) ((x) < 0.0f ? -(x) : (x))
#endif

/* Returns non-zero iff every xyz component of a is within epsilon of the
   matching component of b (|a[i]-b[i]| <= epsilon). Like addScaledDir this
   op is inlined at its VMX128 call sites (vsubfp / vabs / vcmpgtfp +
   all-lanes-true test), so this is the canonical scalar definition the
   frontier caller (decorateConnection, comparing decoration normals)
   references through the extern. Only the xyz lanes are compared — the
   inputs are direction/normal vectors whose w lane is not significant. */
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
