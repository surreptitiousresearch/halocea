/* ?setMul@hkMatrix3@@QAAXABVhkSimdReal@@ABV1@@Z @0x838E2D10 */
#include "../headers/havok/hkMatrix3.h"

/* hkMatrix3::setMul(hkSimdReal scale, hkMatrix3 a)
   Scales every element of a by the broadcast scalar `scale` and stores the result
   in `this`. In the binary `scale` is an hkSimdReal — a __vector4 with the scalar
   replicated across all four lanes — so the VMX vmulfp128 is a per-lane multiply of
   each source column by that broadcast value.

   DEVIATION: the three vmulfp128 column multiplies are reproduced as scalar per-lane
   multiplies (4 lanes x 3 columns). */
void hkMatrix3_setMul(hkMatrix3 *self, const hkVector4 *scale, const hkMatrix3 *a)
{
    const hkVector4 *acol[3];
    hkVector4 *ocol[3];
    int c, lane;

    acol[0] = &a->m_col0;   acol[1] = &a->m_col1;   acol[2] = &a->m_col2;
    ocol[0] = &self->m_col0; ocol[1] = &self->m_col1; ocol[2] = &self->m_col2;

    for (c = 0; c < 3; ++c)
        for (lane = 0; lane < 4; ++lane)
            ocol[c]->m_quad.___u0.v[lane] =
                scale->m_quad.___u0.v[lane] * acol[c]->m_quad.___u0.v[lane];
}
