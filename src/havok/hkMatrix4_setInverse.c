/* ?setInverse@hkMatrix4@@QAA?AW4hkResult@@ABV1@M@Z @0x838E75D0 */
#include "../headers/havok/hkMatrix4.h"
#include "../headers/havok/hkResult.h"

/* Deeper callee kept as a boundary extern (2-level descent limit): the actual
   4x4 matrix inversion. */
extern hkResult hkMatrix4_invert(hkMatrix4 *this, float epsilon); /* hkMatrix4::invert */

/* hkMatrix4::setInverse — copy the four columns of m into this, then invert this
   in place, returning the invert result code.
   DEVIATION: the original copies the four quadwords with VMX128 lvx128/stvx128;
   reconstructed as four column struct copies. */
hkResult hkMatrix4_setInverse(hkMatrix4 *this, const hkMatrix4 *m, float epsilon)
{
    this->m_col0 = m->m_col0;
    this->m_col1 = m->m_col1;
    this->m_col2 = m->m_col2;
    this->m_col3 = m->m_col3;
    return hkMatrix4_invert(this, epsilon);
}
