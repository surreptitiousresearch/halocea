#include "../headers/havok/hkMatrix3.h"
#include "../headers/havok/hkSimdReal.h"

/* hkMatrix3::mul — scale each of the three columns of a 3x3 rotation/matrix in
   place by a broadcast scalar.
   DEVIATION: the original loads the splatted hkSimdReal quad once (lvx128) and
   issues three vmulfp128 column multiplies; reconstructed as scalar loops over
   the four lanes of each column, reading the scalar from lane i of the splatted
   value (all lanes hold the same value). */
void hkMatrix3_mul(hkMatrix3 *this, const hkSimdReal *scale)
{
    int i;
    for (i = 0; i < 4; ++i)
    {
        this->m_col0.m_quad.___u0.v[i] *= scale->m_real.___u0.v[i];
        this->m_col1.m_quad.___u0.v[i] *= scale->m_real.___u0.v[i];
        this->m_col2.m_quad.___u0.v[i] *= scale->m_real.___u0.v[i];
    }
}
