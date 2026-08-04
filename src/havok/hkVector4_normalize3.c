#include "../headers/havok/hkVector4.h"

extern float sqrtf(float);

/* hkVector4::normalize3 — scale all four lanes by 1/sqrt(dot3(this,this)); a
   zero-length vector is left as all-zero (the vsel guard selects 0 where the
   squared length compared equal to 0).
   DEVIATION: original is VMX128 with a vrsqrtefp seed refined by two
   Newton-Raphson iterations against hkQuadRealHalf; reconstructed here as a
   direct 1/sqrtf, which matches the converged result. All four lanes
   (including w) are multiplied by the broadcast reciprocal, as the vspltw of
   lane 0 followed by a full-quad vmulfp128 does. */
void hkVector4_normalize3(hkVector4 *this)
{
    float len2 = this->m_quad.___u0.v[0] * this->m_quad.___u0.v[0]
               + this->m_quad.___u0.v[1] * this->m_quad.___u0.v[1]
               + this->m_quad.___u0.v[2] * this->m_quad.___u0.v[2];
    float invLen = (len2 == 0.0f) ? 0.0f : (1.0f / sqrtf(len2));
    int i;
    for (i = 0; i < 4; ++i)
        this->m_quad.___u0.v[i] *= invLen;
}
