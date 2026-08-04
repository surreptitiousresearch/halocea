#pragma once
#include "../m3d/m3dMATR.h"
#include "../m3d/m3dV.h"
#include "../m3d/m2dPOLY.h"
#include "camPOV_QUAL.h"
#include "camFRUSTUM.h"
// ws-engine (Saber) cam subsystem: render camera (horizontal-FOV setter, world-to-camera
// matrix, viewport). Source path per STRONG_ASSERT file literal in SetParameters:
// D:\Projects\code\common\src.sys\m3d\cam.cpp
// DB-verified layout (types_members camCAMERA), size 896:
//   matrC2W@0 (m3dMATR), matrW2C@64 (m3dMATR), nearPlane@128, farPlane@132,
//   polyViewport@136 (m2dPOLY), xViewPortOffset@300, yViewPortOffset@304, dxViewPort@308,
//   dyViewPort@312, xViewWinOffset@316, yViewWinOffset@320, dxViewWin@324, dyViewWin@328,
//   angleHor@332, angleVert@336, aspectRatioHW@340, ratioDXW2DXViewWin@344,
//   ratioDYW2DYViewWin@348, interest@352 (m3dV), povQual@364 (camPOV_QUAL),
//   pSplFOV@508, pSplRoll@512, pSplPos@516, pSplInterest@520 (m3dSPL*), index@524 (int),
//   frustum@528 (camFRUSTUM).

struct m3dSPL; // opaque — only referenced by pointer here, external to this batch.

typedef struct camCAMERA {
    m3dMATR matrC2W;
    m3dMATR matrW2C;
    float   nearPlane;
    float   farPlane;
    m2dPOLY polyViewport;
    float   xViewPortOffset;
    float   yViewPortOffset;
    float   dxViewPort;
    float   dyViewPort;
    float   xViewWinOffset;
    float   yViewWinOffset;
    float   dxViewWin;
    float   dyViewWin;
    float   angleHor;
    float   angleVert;
    float   aspectRatioHW;
    float   ratioDXW2DXViewWin;
    float   ratioDYW2DYViewWin;
    m3dV    interest;
    camPOV_QUAL povQual;
    m3dSPL  *pSplFOV;
    m3dSPL  *pSplRoll;
    m3dSPL  *pSplPos;
    m3dSPL  *pSplInterest;
    int     index;
    camFRUSTUM frustum;

    // 0x82702468 — set horizontal FOV (degrees); derives angleVert from angleHor + aspect,
    // then recomputes the viewport.
    void SetAngleHor(float _angleHor);

    // 0x82703060 — (position, up, right, forward) basis setup; source param names per the
    // STRONG_ASSERT strings in cam.cpp: pOrg, pUp, pRight, pForw.
    void SetParameters(m3dV *pOrg, m3dV *pUp, m3dV *pRight, m3dV *pForw);

    // set the hardware aspect ratio (aspectRatioHW) and recompute the viewport.  boundary
    void SetAspectRatioHW(float ratio);

    // set the viewport window rectangle (offset + extents) and recompute derived ratios.  boundary
    void SetViewWindow(float xOffset, float yOffset, float dxWin, float dyWin);

    // ?GetMatrC2WLeft@camCAMERA@@QAAXPAVm3dMATR@@@Z — copy the left-handed camera-to-world matrix
    // into `pMatr` (origin in row 4, forward/z axis in row 3).  boundary.
    void GetMatrC2WLeft(m3dMATR *pMatr);

private:
    // 0x82702A88 — private (mangle `AAAX`): normalize matrC2W's 3x3 rotation columns, copy
    // into matrW2C, then invert matrW2C in place via camMatrInvert.
    void AdjustMatrW2C();

    // 0x82702358 — private (mangle `AAAX`): recompute the near-plane viewport quad and
    // window/viewport ratios from angleHor/angleVert/nearPlane/dxViewWin/dyViewWin.
    void RecomputeViewport();
} camCAMERA;

// ?gsCameraDebug@@YAPAVcamCAMERA@@XZ @ 0x82704548 — the active debug/free camera.  boundary.
extern camCAMERA *gsCameraDebug();
