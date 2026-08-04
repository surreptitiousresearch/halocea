#include "../headers/havok/hkMatrix3.h"

/* hkMatrix3::setTranspose — this = transpose(s). Column-major 3x3: out column c
   receives source row c, i.e. out_col[c][r] = s_col[r][c] for r,c in 0..2. The
   padded w lane of each output column is cleared.
   DEVIATION: the original is VMX128 (per-lane loads/stores of the nine elements);
   reconstructed as scalar lane assignments with the same net result. */
void hkMatrix3_setTranspose(hkMatrix3 *self, const hkMatrix3 *s)
{
    const hkVector4 *s_col[3]   = { &s->m_col0, &s->m_col1, &s->m_col2 };
    hkVector4 *out_col[3]       = { &self->m_col0, &self->m_col1, &self->m_col2 };
    int c, r;

    for (c = 0; c < 3; ++c)
    {
        for (r = 0; r < 3; ++r)
            out_col[c]->m_quad.___u0.vector4_f32[r] = s_col[r]->m_quad.___u0.vector4_f32[c];
        out_col[c]->m_quad.___u0.vector4_f32[3] = 0.0f;
    }
}
