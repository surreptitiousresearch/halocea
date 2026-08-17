/* ?setTransformedPos@hkVector4@@QAAXABVhkTransform@@ABV1@@Z @0x838D7BF8 */
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkTransform.h"

/* hkVector4::setTransformedPos — this = t.rotation * v + t.translation
   (transform a point by an hkTransform).
   DEVIATION: original is VMX128 (broadcast of v.x/v.y/v.z, vmaddfp chain over
   the three rotation columns + translation); reconstructed as scalar float. */
void hkVector4_setTransformedPos(hkVector4 *this, const hkTransform *t, const hkVector4 *v)
{
    float vx = v->m_quad.___u0.v[0];
    float vy = v->m_quad.___u0.v[1];
    float vz = v->m_quad.___u0.v[2];
    const hkVector4 *col0 = &t->m_rotation.m_col0;
    const hkVector4 *col1 = &t->m_rotation.m_col1;
    const hkVector4 *col2 = &t->m_rotation.m_col2;
    const hkVector4 *tr   = &t->m_translation;
    int i;
    for (i = 0; i < 4; ++i)
        this->m_quad.___u0.v[i] = col0->m_quad.___u0.v[i] * vx
                                + col1->m_quad.___u0.v[i] * vy
                                + col2->m_quad.___u0.v[i] * vz
                                + tr->m_quad.___u0.v[i];
}
