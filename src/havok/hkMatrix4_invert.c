/* ?invert@hkMatrix4@@QAA?AW4hkResult@@M@Z @0x838E72C0 */
#include "../headers/havok/hkMatrix4.h"

/* Lane accessor for a column vector of the 4x4 matrix. */
#define M(col, lane) ((col).m_quad.___u0.vector4_f32[(lane)])

/* hkMatrix4::invert — invert the 4x4 matrix in place, returning 1 (nonzero) when
   the matrix is singular (|det| <= epsilon, matrix left unchanged) and 0 on
   success.

   The determinant is the generalized (2x2-block) Laplace expansion: it is the
   signed sum of products of the six pairs of complementary 2x2 minors of the
   column pairs. Those minors are computed exactly as in the DB body.

   DEVIATION: on the success path the DB body assembles the scaled adjugate
   columns and stores them back into m_col0..m_col3 with a VMX128 sequence
   (lvx128/vmulfp128/stvx128 against a 1/det splat) that the decompiler did not
   surface as scalar expressions. That store is represented here by the boundary
   helper hkMatrix4_storeScaledAdjugate, which writes the reciprocal-determinant
   scaled adjugate — reconstructing the exact per-lane column assignment requires
   disassembling that VMX block and is left for a follow-up pass. */
extern void hkMatrix4_storeScaledAdjugate(hkMatrix4 *m, float invDet); /* VMX128 adjugate store (un-recovered) */

int hkMatrix4_invert(hkMatrix4 *this, float epsilon)
{
    /* Six complementary 2x2 minors (column-pair sub-determinants). */
    float minor_c2c3_23 = M(this->m_col2, 2) * M(this->m_col3, 3) - M(this->m_col2, 3) * M(this->m_col3, 2);
    float minor_c0c1_01 = M(this->m_col0, 0) * M(this->m_col1, 1) - M(this->m_col0, 1) * M(this->m_col1, 0);
    float minor_c0c3_23 = M(this->m_col0, 2) * M(this->m_col3, 3) - M(this->m_col0, 3) * M(this->m_col3, 2);
    float minor_c1c3_23 = M(this->m_col1, 2) * M(this->m_col3, 3) - M(this->m_col1, 3) * M(this->m_col3, 2);
    float minor_c0c2_01 = M(this->m_col0, 0) * M(this->m_col2, 1) - M(this->m_col0, 1) * M(this->m_col2, 0);
    float minor_c1c2_01 = M(this->m_col1, 0) * M(this->m_col2, 1) - M(this->m_col1, 1) * M(this->m_col2, 0);
    float minor_c0c3_01 = M(this->m_col0, 0) * M(this->m_col3, 1) - M(this->m_col0, 1) * M(this->m_col3, 0);
    float minor_c1c3_01 = M(this->m_col1, 0) * M(this->m_col3, 1) - M(this->m_col1, 1) * M(this->m_col3, 0);
    float minor_c0c2_23 = M(this->m_col0, 2) * M(this->m_col2, 3) - M(this->m_col0, 3) * M(this->m_col2, 2);
    float minor_c1c2_23 = M(this->m_col1, 2) * M(this->m_col2, 3) - M(this->m_col1, 3) * M(this->m_col2, 2);
    float minor_c2c3_01 = M(this->m_col2, 0) * M(this->m_col3, 1) - M(this->m_col2, 1) * M(this->m_col3, 0);
    float minor_c0c1_23 = M(this->m_col0, 2) * M(this->m_col1, 3) - M(this->m_col0, 3) * M(this->m_col1, 2);

    float det = minor_c0c1_23 * minor_c2c3_01
              - (minor_c0c2_23 * minor_c1c3_01
                 - (minor_c1c2_23 * minor_c0c3_01
                    + (minor_c0c3_23 * minor_c1c2_01
                       + (minor_c2c3_23 * minor_c0c1_01 - minor_c1c3_23 * minor_c0c2_01))));

    if (det <= epsilon && det >= -epsilon) /* |det| <= epsilon */
        return 1;

    hkMatrix4_storeScaledAdjugate(this, 1.0f / det);
    return 0;
}
