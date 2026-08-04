#include "../headers/havok/hkMatrix3.h"

/* hkMatrix3::setMulInverse(hkMatrix3 aTb, hkRotation cTb)
   Sets this = aTb * cTb^-1. cTb is a rotation, so its inverse is its transpose and
   the product is computed column-by-column:

       this.col_k = aTb.col0 * cTb.col0[k]
                  + aTb.col1 * cTb.col1[k]
                  + aTb.col2 * cTb.col2[k]      (k = 0,1,2)

   i.e. each output column combines aTb's columns weighted by the k-th row of cTb.
   In the binary this is the three-iteration vmaddfp chain (vspltw of a per-column
   weight vector assembled from cTb, times aTb's columns), the `back_chain` stack
   temp being the transposed-row scratch.

   DEVIATION: the VMX splat/vmaddfp accumulation is reproduced as an explicit scalar
   4-lane multiply-add over the three source columns. hkRotation is layout-identical
   to hkMatrix3 (three hkVector4 columns). */
void hkMatrix3_setMulInverse(hkMatrix3 *self, const hkMatrix3 *aTb, const hkMatrix3 *cTb)
{
    const hkVector4 *acol[3];
    const hkVector4 *ccol[3];
    hkVector4 *ocol[3];
    int k, lane;

    acol[0] = &aTb->m_col0;  acol[1] = &aTb->m_col1;  acol[2] = &aTb->m_col2;
    ccol[0] = &cTb->m_col0;  ccol[1] = &cTb->m_col1;  ccol[2] = &cTb->m_col2;
    ocol[0] = &self->m_col0; ocol[1] = &self->m_col1; ocol[2] = &self->m_col2;

    for (k = 0; k < 3; ++k)
        for (lane = 0; lane < 4; ++lane)
            ocol[k]->m_quad.___u0.v[lane] =
                  acol[0]->m_quad.___u0.v[lane] * ccol[0]->m_quad.___u0.v[k]
                + acol[1]->m_quad.___u0.v[lane] * ccol[1]->m_quad.___u0.v[k]
                + acol[2]->m_quad.___u0.v[lane] * ccol[2]->m_quad.___u0.v[k];
}
