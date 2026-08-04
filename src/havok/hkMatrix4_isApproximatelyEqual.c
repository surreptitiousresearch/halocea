#include "../headers/havok/hkMatrix4.h"
#include "../headers/havok/hkBool.h"

/* hkMatrix4::isApproximatelyEqual — true when `this` equals `m` within the
   tolerance `zero`: for every one of the four columns, each lane's abs
   difference must be <= zero. The computed hkBool is returned through `result`
   (the by-value hkBool return slot, r3); r4/r5 are the two matrices.
   DEVIATION 1: the decompiler mislabels the r3 return slot as a hkMatrix4* and
   emits the boolean store as HIBYTE(...) into a float lane; reconstructed as a
   plain hkBool write through `result`.
   DEVIATION 2: the original uses VMX abs/compare with a lane mask
   (s_invMaskFromBits) to ignore the padding lane; reconstructed as scalar loops
   over the four lanes of each column. */
hkBool *hkMatrix4_isApproximatelyEqual(hkBool *result, const hkMatrix4 *this,
                                       const hkMatrix4 *m, float zero)
{
    const hkVector4 *thisCols = &this->m_col0;
    const hkVector4 *mCols = &m->m_col0;
    char equal = 1;
    int col;

    for (col = 0; col < 4 && equal; ++col)
    {
        int i;
        for (i = 0; i < 4; ++i)
        {
            float diff = thisCols[col].m_quad.___u0.v[i] - mCols[col].m_quad.___u0.v[i];
            if (diff < 0.0f)
                diff = -diff;
            if (diff > zero)
            {
                equal = 0;
                break;
            }
        }
    }

    result->m_bool = equal;
    return result;
}
