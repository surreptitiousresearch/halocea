#include "../headers/havok/hkTransform.h"

/* this = inverse(bTa) * bTc  (Havok hkTransform::setMulInverseMul).
   For a rigid transform the inverse rotation is the transpose, so each output
   rotation column is bTa^T applied to the matching bTc column (the three dot
   products of bTa's columns with that column), and the translation is bTa^T
   applied to (bTc.translation - bTa.translation).
   DEVIATION: the original is VMX128 (vmsum3fp128 3-lane dot products, columns
   processed high->low, merged with vmrghw); reconstructed as explicit
   column-dot loops. */
static float hk_dot3(const hkVector4 *a, const hkVector4 *b)
{
    return a->m_quad.___u0.v[0] * b->m_quad.___u0.v[0]
         + a->m_quad.___u0.v[1] * b->m_quad.___u0.v[1]
         + a->m_quad.___u0.v[2] * b->m_quad.___u0.v[2];
}

void hkTransform_setMulInverseMul(hkTransform *out, const hkTransform *bTa, const hkTransform *bTc)
{
    const hkVector4 *aCols[3];
    hkVector4 *outCols[3];
    hkVector4 relTrans;
    int c;

    aCols[0] = &bTa->m_rotation.m_col0;
    aCols[1] = &bTa->m_rotation.m_col1;
    aCols[2] = &bTa->m_rotation.m_col2;
    outCols[0] = &out->m_rotation.m_col0;
    outCols[1] = &out->m_rotation.m_col1;
    outCols[2] = &out->m_rotation.m_col2;

    {
        const hkVector4 *inCols[3];
        inCols[0] = &bTc->m_rotation.m_col0;
        inCols[1] = &bTc->m_rotation.m_col1;
        inCols[2] = &bTc->m_rotation.m_col2;
        for (c = 0; c < 3; ++c)
        {
            outCols[c]->m_quad.___u0.v[0] = hk_dot3(aCols[0], inCols[c]);
            outCols[c]->m_quad.___u0.v[1] = hk_dot3(aCols[1], inCols[c]);
            outCols[c]->m_quad.___u0.v[2] = hk_dot3(aCols[2], inCols[c]);
            outCols[c]->m_quad.___u0.v[3] = 0.0f;
        }
    }

    for (c = 0; c < 3; ++c)
        relTrans.m_quad.___u0.v[c] = bTc->m_translation.m_quad.___u0.v[c]
                                   - bTa->m_translation.m_quad.___u0.v[c];
    relTrans.m_quad.___u0.v[3] = 0.0f;

    out->m_translation.m_quad.___u0.v[0] = hk_dot3(aCols[0], &relTrans);
    out->m_translation.m_quad.___u0.v[1] = hk_dot3(aCols[1], &relTrans);
    out->m_translation.m_quad.___u0.v[2] = hk_dot3(aCols[2], &relTrans);
    out->m_translation.m_quad.___u0.v[3] = 0.0f;
}
