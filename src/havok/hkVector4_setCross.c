#include "../headers/havok/hkVector4.h"

/* hkVector4::setCross — this = x cross y (3-lane cross product; the w lane
   evaluates to 0 through the same permuted multiply/subtract).
   DEVIATION: original is VMX128 (vpermwi128 yzx/zxy shuffles + vmulfp128 +
   vsubfp128); reconstructed as scalar float math. */
void hkVector4_setCross(hkVector4 *this, const hkVector4 *x, const hkVector4 *y)
{
    float xx = x->m_quad.___u0.v[0], xy = x->m_quad.___u0.v[1], xz = x->m_quad.___u0.v[2];
    float yx = y->m_quad.___u0.v[0], yy = y->m_quad.___u0.v[1], yz = y->m_quad.___u0.v[2];
    this->m_quad.___u0.v[0] = xy * yz - xz * yy;
    this->m_quad.___u0.v[1] = xz * yx - xx * yz;
    this->m_quad.___u0.v[2] = xx * yy - xy * yx;
    this->m_quad.___u0.v[3] = x->m_quad.___u0.v[3] * y->m_quad.___u0.v[3]
                            - x->m_quad.___u0.v[3] * y->m_quad.___u0.v[3]; /* == 0 */
}
