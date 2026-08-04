#include "../headers/havok/hkVector4.h"

/* hkVector4::setMul4 — this = scalar * x over all four lanes. Corpus callers
   (e.g. decorateConnection) spell this as hkVector4_setScaled with a plain
   float; the underlying op takes an hkSimdReal broadcast register.
   DEVIATION: original is a single VMX128 vmulfp128 of x by the broadcast
   scalar quad; reconstructed as scalar float. */
void hkVector4_setScaled(hkVector4 *this, const hkVector4 *x, float scale)
{
    int i;
    for (i = 0; i < 4; ++i)
        this->m_quad.___u0.v[i] = x->m_quad.___u0.v[i] * scale;
}
