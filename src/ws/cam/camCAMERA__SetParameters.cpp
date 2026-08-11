#include "../../headers/ws/cam/camCAMERA.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// 0x82703060 — builds matrC2W's orientation basis from (position, up, right, forward), then
// validates all four inputs and the resulting matrix via STRONG_ASSERT. Parameter names
// (pOrg, pUp, pRight, pForw) and the source path/line numbers are taken verbatim from the
// STRONG_ASSERT condition/file strings embedded in the decompile (D:\Projects\code\common\
// src.sys\m3d\cam.cpp, lines 792-796).
void camCAMERA::SetParameters(m3dV *pOrg, m3dV *pUp, m3dV *pRight, m3dV *pForw)
{
    matrC2W.Identity();

    // Basis rows: right (row0), up (row1), forward (row2) — consumed by AdjustMatrW2C's
    // per-row axis normalization.
    matrC2W.elements[0] = pRight->x;
    matrC2W.elements[4] = pUp->x;
    matrC2W.elements[8] = pForw->x;
    matrC2W.elements[1] = pRight->y;
    matrC2W.elements[5] = pUp->y;
    matrC2W.elements[9] = pForw->y;
    matrC2W.elements[2] = pRight->z;
    matrC2W.elements[6] = pUp->z;
    matrC2W.elements[10] = pForw->z;

    // Translation row (row3 / matrC2W.v[3]): DB shows `matrC2W.v[3].m3dV = *pOrg`, i.e. only
    // the leading m3dV base (x,y,z) of the m4dV row is assigned from *pOrg — reconstructed
    // as a base-subobject slice-assignment; v[3].w is left as Identity() set it (1.0f).
    static_cast<m3dV &>(matrC2W.v[3]) = *pOrg;

    AdjustMatrW2C();

    // STRONG_ASSERT(_m3dCheckValid(pOrg)) / (pUp) / (pRight) / (pForw) / (matrC2W.Check()).
    // The decompiler renders the macro's `if (!IGNORE_STRONG_ASSERT)` guard as a deepening
    // nest (one per assert); that is a straight-line chain of 5 independent STRONG_ASSERT
    // invocations, reconstructed flat here. Crash is invoked through a null this-pointer in
    // the decompile (`STRONG_ASSERT_DUMMY::Crash(nullptr, ...)`), matching the macro's
    // dummy-singleton pattern — reproduced verbatim rather than default-constructing an
    // instance.
    if (!IGNORE_STRONG_ASSERT && !_m3dCheckValid(pOrg))
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("_m3dCheckValid(pOrg)", "D:\\Projects\\code\\common\\src.sys\\m3d\\cam.cpp", 792, m3dStrongAssertMessage);
    if (!IGNORE_STRONG_ASSERT && !_m3dCheckValid(pUp))
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("_m3dCheckValid(pUp)", "D:\\Projects\\code\\common\\src.sys\\m3d\\cam.cpp", 793, m3dStrongAssertMessage);
    if (!IGNORE_STRONG_ASSERT && !_m3dCheckValid(pRight))
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("_m3dCheckValid(pRight)", "D:\\Projects\\code\\common\\src.sys\\m3d\\cam.cpp", 794, m3dStrongAssertMessage);
    if (!IGNORE_STRONG_ASSERT && !_m3dCheckValid(pForw))
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("_m3dCheckValid(pForw)", "D:\\Projects\\code\\common\\src.sys\\m3d\\cam.cpp", 795, m3dStrongAssertMessage);
    if (!IGNORE_STRONG_ASSERT && !matrC2W.Check())
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("pMatrC2W->Check()", "D:\\Projects\\code\\common\\src.sys\\m3d\\cam.cpp", 796, m3dStrongAssertMessage);
}
