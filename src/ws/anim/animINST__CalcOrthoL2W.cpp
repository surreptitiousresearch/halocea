#include "animINST.h"
#include "../m3d/m3dMATR.h"
#include "../m3d/m3dV.h"
#include "../ds/ds_assert_boundary.h"

// _m3dNormalize (?_m3dNormalize@@YAHPAVm3dV@@@Z) — normalize a vector in place; returns nonzero
// on success (a zero-length vector fails). boundary.
int _m3dNormalize(m3dV *v);

// ?CalcOrthoL2W@animINST@@AAAXPAVm3dMATR@@@Z  (0x825E2BE0) — private.
// Build an orthonormal local-to-world basis from the instance's (possibly scaled/skewed) matrix:
// take its up (row 2) and right (row 1) axes, normalize them, and rebuild a clean coordinate
// system anchored at the translation (row 3).
void animINST::CalcOrthoL2W(m3dMATR *matrOrthoL2W)
{
    m3dV upAxis;    // instance matrix row 2 (elements[8..10])
    m3dV rightAxis; // instance matrix row 1 (elements[4..6])
    m3dV origin;    // instance matrix translation (elements[12..14])

    upAxis.x = this->matrL2W.elements[8];
    upAxis.y = this->matrL2W.elements[9];
    upAxis.z = this->matrL2W.elements[10];
    rightAxis.x = this->matrL2W.elements[4];
    rightAxis.y = this->matrL2W.elements[5];
    rightAxis.z = this->matrL2W.elements[6];
    origin.x = this->matrL2W.elements[12];
    origin.y = this->matrL2W.elements[13];
    origin.z = this->matrL2W.elements[14];

    int normalizedOk = _m3dNormalize(&upAxis) & _m3dNormalize(&rightAxis);
    if (!IGNORE_STRONG_ASSERT && !normalizedOk)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "rc",
            "D:\\Projects\\code\\common\\src.sys\\animation\\anim.cpp",
            1550,
            empty_string);

    matrOrthoL2W->MakeLCS2WCS(&origin, nullptr, &rightAxis, &upAxis);
}
