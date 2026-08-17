/* ?set@hkMatrix4@@QAAXABVhkTransform@@@Z @0x838E7B50 */
#include "../headers/havok/hkMatrix4.h"
#include "../headers/havok/hkTransform.h"

/* Widen a 3x4 hkTransform (three rotation columns + translation) into a 4x4
   hkMatrix4: copy the four column vectors verbatim, then stamp the homogeneous
   bottom row into each column's w lane (0,0,0 for the basis columns, 1 for the
   translation column). DEVIATION: the original copies the four quadwords with
   VMX128 lvx128/stvx128 then overwrites the four w lanes scalar; reconstructed
   as struct copies plus the four scalar stores. */
void hkMatrix4_set(hkMatrix4 *dst, const hkTransform *t)
{
    dst->m_col0 = t->m_rotation.m_col0;
    dst->m_col1 = t->m_rotation.m_col1;
    dst->m_col2 = t->m_rotation.m_col2;
    dst->m_col3 = t->m_translation;

    dst->m_col0.m_quad.___u0.v[3] = 0.0f;
    dst->m_col1.m_quad.___u0.v[3] = 0.0f;
    dst->m_col2.m_quad.___u0.v[3] = 0.0f;
    dst->m_col3.m_quad.___u0.v[3] = 1.0f;
}
