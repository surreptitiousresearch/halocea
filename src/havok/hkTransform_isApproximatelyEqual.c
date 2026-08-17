/* ?isApproximatelyEqual@hkTransform@@QBA?AVhkBool@@ABV1@M@Z @0x838E07F8 */
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkMatrix3.h"
#include "../headers/havok/hkBool.h"

/* Deeper callee kept as a boundary extern (2-level descent limit): the 3x3
   rotation approximate-equality test. */
extern hkBool *hkMatrix3_isApproximatelyEqual(hkBool *out, const hkMatrix3 *a,
                                              const hkMatrix3 *b, float zero); /* hkMatrix3::isApproximatelyEqual */

/* hkTransform::isApproximatelyEqual — true when `this` equals `t` within the
   tolerance `zero`: rotations compare via hkMatrix3::isApproximatelyEqual and
   the translation matches when every lane's abs difference is <= zero. The
   computed hkBool is returned through `result` (the by-value hkBool return slot,
   r3), while r4/r5 are the two transforms being compared.
   DEVIATION 1: the decompiler mislabels the r3 return slot as a hkTransform* and
   emits the boolean store as HIBYTE(...) into a float lane; reconstructed as a
   plain hkBool write through `result`.
   DEVIATION 2: the original uses VMX abs/compare with a lane mask
   (s_invMaskFromBits) to ignore the padding lane; reconstructed as a scalar loop
   over the four translation lanes. */
hkBool *hkTransform_isApproximatelyEqual(hkBool *result, const hkTransform *this,
                                         const hkTransform *t, float zero)
{
    hkBool rotEqual;
    char equal = 1;

    if (!hkMatrix3_isApproximatelyEqual(&rotEqual, &this->m_rotation, &t->m_rotation, zero)->m_bool)
    {
        equal = 0;
    }
    else
    {
        int i;
        for (i = 0; i < 4; ++i)
        {
            float diff = this->m_translation.m_quad.___u0.v[i] - t->m_translation.m_quad.___u0.v[i];
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
