#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkTransform.h"

/* hkTransform::setMul — this = aTb * bTc (compose two rigid transforms).
   Each of the three rotation columns and the translation column of bTc is
   rotated by aTb's rotation; the translation additionally gets aTb's
   translation added.
   DEVIATION: original is VMX128 (a 4-iteration loop that broadcasts each
   source column's x/y/z and vmaddfp's the three aTb columns, plus a trailing
   vaddfp for the translation); reconstructed as scalar float. */
void hkTransform_setMul(hkTransform *this, const hkTransform *aTb, const hkTransform *bTc)
{
    const hkVector4 *rot_col0 = &aTb->m_rotation.m_col0;
    const hkVector4 *rot_col1 = &aTb->m_rotation.m_col1;
    const hkVector4 *rot_col2 = &aTb->m_rotation.m_col2;
    const hkVector4 *srcCols[4];
    hkVector4 *dstCols[4];
    int c, i;

    srcCols[0] = &bTc->m_rotation.m_col0;
    srcCols[1] = &bTc->m_rotation.m_col1;
    srcCols[2] = &bTc->m_rotation.m_col2;
    srcCols[3] = &bTc->m_translation;
    dstCols[0] = &this->m_rotation.m_col0;
    dstCols[1] = &this->m_rotation.m_col1;
    dstCols[2] = &this->m_rotation.m_col2;
    dstCols[3] = &this->m_translation;

    for (c = 0; c < 4; ++c)
    {
        float cx = srcCols[c]->m_quad.___u0.v[0];
        float cy = srcCols[c]->m_quad.___u0.v[1];
        float cz = srcCols[c]->m_quad.___u0.v[2];
        for (i = 0; i < 4; ++i)
            dstCols[c]->m_quad.___u0.v[i] = rot_col0->m_quad.___u0.v[i]*cx
                                          + rot_col1->m_quad.___u0.v[i]*cy
                                          + rot_col2->m_quad.___u0.v[i]*cz;
    }
    for (i = 0; i < 4; ++i)
        this->m_translation.m_quad.___u0.v[i] += aTb->m_translation.m_quad.___u0.v[i];
}
