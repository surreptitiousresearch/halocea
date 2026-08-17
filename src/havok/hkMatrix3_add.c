/* ?add@hkMatrix3@@QAAXABV1@@Z @0x838E3170 */
#include "../headers/havok/hkMatrix3.h"

/* hkMatrix3::add — this += a, column-major 3x3 with each column padded to a full
   hkVector4. DEVIATION: original is VMX128 (lvx128/vaddfp/stvx128 over three full
   quadwords, columns processed 0->2); reconstructed as scalar loops over all four
   lanes (the padded w lane is carried through exactly as the vector code does). */
void hkMatrix3_add(hkMatrix3 *self, const hkMatrix3 *a)
{
    hkVector4 *out_cols[3]      = { &self->m_col0, &self->m_col1, &self->m_col2 };
    const hkVector4 *in_cols[3] = { &a->m_col0, &a->m_col1, &a->m_col2 };
    int c, r;

    for (c = 0; c < 3; ++c)
        for (r = 0; r < 4; ++r)
            out_cols[c]->m_quad.___u0.v[r] += in_cols[c]->m_quad.___u0.v[r];
}
