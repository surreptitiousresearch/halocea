#include "../headers/havok/hkMatrix4.h"

/* Lane accessor for a column vector of the 4x4 matrix. */
#define M(col, lane) ((col).m_quad.___u0.vector4_f32[(lane)])

/* hkMatrix4_storeScaledAdjugate — inline-only helper reconstructed from the
   tail of ?invert@hkMatrix4@@QAA?AW4hkResult@@M@Z (@0x838E72C0). It writes the
   invDet-scaled adjugate (classical adjoint) of the 4x4 back into m's columns.
   Cited block: 0x838E7408..0x838E75BC (success path of invert).

   The determinant expansion in invert() leaves twelve complementary 2x2
   minors on the stack (var_100..var_130); this helper recomputes them from m
   (it is factored out with only m + invDet as inputs) and forms the sixteen
   cofactor entries with the exact fmadds/fmsubs/fnmsubs sequence at
   0x838E7484..0x838E7588. Each output column is then multiplied by an invDet
   splat (0x838E759C vmulfp128 v*,v*,v0 with v0 = vspltw(invDet)) and stored
   whole with stvx128 (0x838E75A4..0x838E75B8): the var_E0 group -> m_col0,
   var_D0 -> m_col1, var_C0 -> m_col2, var_B0 -> m_col3.

   DEVIATION: the DB body keeps this inlined and expresses the final scale/store
   as VMX128 lane ops (lvx128/vmulfp128/stvx128); those are equivalent to a
   per-lane scalar multiply by invDet, reproduced here as scalar stores. All
   sixteen cofactor formulas are transcribed verbatim from the disasm; matrix
   entry naming is e<col><lane> (column-major, matching M(col,lane) above). */
void hkMatrix4_storeScaledAdjugate(hkMatrix4 *m, float invDet)
{
    /* Column-major matrix entries e<col><lane>. */
    float e00 = M(m->m_col0, 0), e01 = M(m->m_col0, 1), e02 = M(m->m_col0, 2), e03 = M(m->m_col0, 3);
    float e10 = M(m->m_col1, 0), e11 = M(m->m_col1, 1), e12 = M(m->m_col1, 2), e13 = M(m->m_col1, 3);
    float e20 = M(m->m_col2, 0), e21 = M(m->m_col2, 1), e22 = M(m->m_col2, 2), e23 = M(m->m_col2, 3);
    float e30 = M(m->m_col3, 0), e31 = M(m->m_col3, 1), e32 = M(m->m_col3, 2), e33 = M(m->m_col3, 3);

    /* Twelve complementary 2x2 column-pair minors (var_100..var_130). */
    float pg_oh = e33 * e12 - e32 * e13; /* var_120 */
    float af_be = e00 * e11 - e01 * e10; /* var_114 */
    float ja_ib = e21 * e00 - e20 * e01; /* var_FC  */
    float pk_ol = e33 * e22 - e32 * e23; /* var_118 */
    float je_if = e21 * e10 - e20 * e11; /* var_F4  */
    float pc_od = e33 * e02 - e32 * e03; /* var_130 */
    float na_mb = e31 * e00 - e30 * e01; /* var_108 */
    float ne_mf = e31 * e10 - e30 * e11; /* var_104 */
    float lc_kd = e23 * e02 - e22 * e03; /* var_110 */
    float lg_kh = e23 * e12 - e22 * e13; /* var_10C */
    float ni_mj = e31 * e20 - e30 * e21; /* var_100 */
    float ch_dg = e02 * e13 - e03 * e12; /* var_F8  */

    /* Sixteen scaled cofactor entries (adjugate * invDet). */
    /* m_col0 (var_E0 group) */
    float adj00 = (e31 * lg_kh + e11 * pk_ol - e21 * pg_oh) * invDet; /* var_E0 */
    float adj01 = (e21 * pc_od - e01 * pk_ol - e31 * lc_kd) * invDet; /* var_DC */
    float adj02 = (e31 * ch_dg + e01 * pg_oh - e11 * pc_od) * invDet; /* var_D8 */
    float adj03 = (e11 * lc_kd - e01 * lg_kh - e21 * ch_dg) * invDet; /* var_D4 */
    /* m_col1 (var_D0 group) */
    float adj10 = (e20 * pg_oh - e10 * pk_ol - e30 * lg_kh) * invDet; /* var_D0 */
    float adj11 = (e30 * lc_kd + e00 * pk_ol - e20 * pc_od) * invDet; /* var_CC */
    float adj12 = (e10 * pc_od - e00 * pg_oh - e30 * ch_dg) * invDet; /* var_C8 */
    float adj13 = (e20 * ch_dg + e00 * lg_kh - e10 * lc_kd) * invDet; /* var_C4 */
    /* m_col2 (var_C0 group) */
    float adj20 = (e33 * je_if + e13 * ni_mj - e23 * ne_mf) * invDet; /* var_C0 */
    float adj21 = (e23 * na_mb - e03 * ni_mj - e33 * ja_ib) * invDet; /* var_BC */
    float adj22 = (e33 * af_be + e03 * ne_mf - e13 * na_mb) * invDet; /* var_B8 */
    float adj23 = (e13 * ja_ib - e03 * je_if - e23 * af_be) * invDet; /* var_B4 */
    /* m_col3 (var_B0 group) */
    float adj30 = (e22 * ne_mf - e12 * ni_mj - e32 * je_if) * invDet; /* var_B0 */
    float adj31 = (e32 * ja_ib + e02 * ni_mj - e22 * na_mb) * invDet; /* var_AC */
    float adj32 = (e12 * na_mb - e02 * ne_mf - e32 * af_be) * invDet; /* var_A8 */
    float adj33 = (e22 * af_be + e02 * je_if - e12 * ja_ib) * invDet; /* var_A4 */

    /* stvx128 whole-column stores (0x838E75A4..0x838E75B8). */
    M(m->m_col0, 0) = adj00; M(m->m_col0, 1) = adj01; M(m->m_col0, 2) = adj02; M(m->m_col0, 3) = adj03;
    M(m->m_col1, 0) = adj10; M(m->m_col1, 1) = adj11; M(m->m_col1, 2) = adj12; M(m->m_col1, 3) = adj13;
    M(m->m_col2, 0) = adj20; M(m->m_col2, 1) = adj21; M(m->m_col2, 2) = adj22; M(m->m_col2, 3) = adj23;
    M(m->m_col3, 0) = adj30; M(m->m_col3, 1) = adj31; M(m->m_col3, 2) = adj32; M(m->m_col3, 3) = adj33;
}
