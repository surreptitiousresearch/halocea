/* hkMatrix3_setMulMat3Mat3 @0x838E3578 */
#include "../headers/havok/hkMatrix3.h"

/* out = aTb * bTc (column-major 3x3). Each output column is aTb applied to the
   corresponding column of bTc: col0*x + col1*y + col2*z. DEVIATION: the original
   is VMX128 (vmulfp/vmaddfp over full quadwords, columns processed 2->0);
   reconstructed as scalar loops over all four lanes (the padded w lane is
   carried through exactly as the vector code does). */
void hkMatrix3_setMulMat3Mat3(hkMatrix3 *out, const hkMatrix3 *aTb, const hkMatrix3 *bTc)
{
    const hkVector4 *in_cols[3]  = { &bTc->m_col0, &bTc->m_col1, &bTc->m_col2 };
    hkVector4 *out_cols[3] = { &out->m_col0, &out->m_col1, &out->m_col2 };
    int c, r;

    for (c = 0; c < 3; ++c)
    {
        for (r = 0; r < 4; ++r)
        {
            out_cols[c]->m_quad.___u0.v[r] = aTb->m_col0.m_quad.___u0.v[r] * in_cols[c]->m_quad.___u0.v[0]
                              + aTb->m_col1.m_quad.___u0.v[r] * in_cols[c]->m_quad.___u0.v[1]
                              + aTb->m_col2.m_quad.___u0.v[r] * in_cols[c]->m_quad.___u0.v[2];
        }
    }
}
