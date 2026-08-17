/* ?setNeg4@hkVector4@@QAAXABV1@@Z @0x83601D50 */
#include "../headers/havok/hkVector4.h"

/* dst = -x, all four lanes negated. DEVIATION: the original is VMX128
   (vxor with a 0x80000000 sign-mask quadword); reconstructed as a scalar
   lane loop that flips the sign of every component (the padded w lane is
   negated exactly as the vector code does). */
void hkVector4_setNeg4(hkVector4 *dst, const hkVector4 *x)
{
    int i;
    for (i = 0; i < 4; ++i)
        dst->m_quad.___u0.v[i] = -x->m_quad.___u0.v[i];
}
