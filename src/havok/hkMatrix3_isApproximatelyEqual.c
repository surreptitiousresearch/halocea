#include "../headers/havok/hkMatrix3.h"
#include "../headers/havok/hkBool.h"
#include <math.h>

/* hkMatrix3::isApproximatelyEqual @ 0x838E2DA0
   True iff every element of |a - b| is <= epsilon (epsilon broadcast to all
   lanes). The binary tests three columns in sequence and short-circuits on the
   first that exceeds the threshold.
   DEVIATION: the source computes the per-column comparison with VMX
   (vsubfp / abs-mask / vcmpgefp); reconstructed here as a scalar three-lane
   compare over the two operand matrices with epsilon.
   The hkBool is returned through the r3 sret pointer `result` (disasm 0x838E2DA0
   writes the byte to 0(r3) and the DB prototype/sole caller use this sret ABI). */
hkBool *hkMatrix3_isApproximatelyEqual(hkBool *result, const hkMatrix3 *a, const hkMatrix3 *b, float epsilon)
{
    const hkVector4 *ca[3];
    const hkVector4 *cb[3];
    int col, lane;

    ca[0] = &a->m_col0; ca[1] = &a->m_col1; ca[2] = &a->m_col2;
    cb[0] = &b->m_col0; cb[1] = &b->m_col1; cb[2] = &b->m_col2;

    for (col = 0; col < 3; ++col)
    {
        for (lane = 0; lane < 3; ++lane)
        {
            float d = ca[col]->m_quad.___u0.vector4_f32[lane] - cb[col]->m_quad.___u0.vector4_f32[lane];
            if (fabsf(d) > epsilon)
            {
                result->m_bool = 0;
                return result;
            }
        }
    }

    result->m_bool = 1;
    return result;
}
