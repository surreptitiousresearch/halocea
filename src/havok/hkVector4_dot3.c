/* ?dot3@hkVector4@@QBA?AVhkSimdReal@@ABV1@@Z @0x8293CF08 */
#include "../headers/havok/hkVector4.h"

/* hkVector4::dot3 — 3-lane dot product of two vectors, broadcast into the
   destination quad which is returned.
   DEVIATION: the original is VMX128 (vmsum3fp128 over the two source quads
   loaded from r4 and r5, storing the broadcast sum to r3); reconstructed here
   as scalar float math. */
hkVector4 *hkVector4_dot3(hkVector4 *result, const hkVector4 *a, const hkVector4 *b)
{
    float d = a->m_quad.___u0.v[0] * b->m_quad.___u0.v[0]
            + a->m_quad.___u0.v[1] * b->m_quad.___u0.v[1]
            + a->m_quad.___u0.v[2] * b->m_quad.___u0.v[2];
    int i;
    for (i = 0; i < 4; ++i)
        result->m_quad.___u0.v[i] = d;
    return result;
}
