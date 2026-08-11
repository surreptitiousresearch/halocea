/* FUNCTION_INDEX entry: hkVector4_setSub4 @0x82918628 (?setSub4@hkVector4@@QAAXABV1@0@Z) */
#include "../headers/havok/hkVector4.h"

/* hkVector4::setSub4 — this = x - y over all four lanes.
   Corpus callers spell this as hkVector4_setSub.
   DEVIATION: original is a single VMX128 vsubfp128; reconstructed as scalar. */
void hkVector4_setSub(hkVector4 *this, const hkVector4 *x, const hkVector4 *y)
{
    int i;
    for (i = 0; i < 4; ++i)
        this->m_quad.___u0.v[i] = x->m_quad.___u0.v[i] - y->m_quad.___u0.v[i];
}
