/* ?setTransformedInversePos@hkVector4@@QAAXABVhkTransform@@ABV1@@Z @0x838D7C40 */
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkTransform.h"

/* hkVector4::setTransformedInversePos — this = transpose(t.rotation) * (v - t.translation)
   (map a world point into the transform's local frame). The w lane is set to 0
   (merged in from hkQuadReal0000).
   DEVIATION: original is VMX128 (vsubfp of the translation, three vmsum3fp128
   row dots, vmrghw lane merges); reconstructed as scalar float. */
void hkVector4_setTransformedInversePos(hkVector4 *this, const hkTransform *t, const hkVector4 *v)
{
    float dx = v->m_quad.___u0.v[0] - t->m_translation.m_quad.___u0.v[0];
    float dy = v->m_quad.___u0.v[1] - t->m_translation.m_quad.___u0.v[1];
    float dz = v->m_quad.___u0.v[2] - t->m_translation.m_quad.___u0.v[2];
    const hkVector4 *col0 = &t->m_rotation.m_col0;
    const hkVector4 *col1 = &t->m_rotation.m_col1;
    const hkVector4 *col2 = &t->m_rotation.m_col2;
    this->m_quad.___u0.v[0] = col0->m_quad.___u0.v[0]*dx + col0->m_quad.___u0.v[1]*dy + col0->m_quad.___u0.v[2]*dz;
    this->m_quad.___u0.v[1] = col1->m_quad.___u0.v[0]*dx + col1->m_quad.___u0.v[1]*dy + col1->m_quad.___u0.v[2]*dz;
    this->m_quad.___u0.v[2] = col2->m_quad.___u0.v[0]*dx + col2->m_quad.___u0.v[1]*dy + col2->m_quad.___u0.v[2]*dz;
    this->m_quad.___u0.v[3] = 0.0f;
}
