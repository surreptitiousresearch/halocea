/* ?RecomputeViewport@camCAMERA@@AAAXXZ @0x82702358 */
#include "../../headers/ws/cam/camCAMERA.h"
#include <math.h>

// 0x82702358 — private. Rebuilds the near-plane viewport quad (polyViewport) and the
// window/viewport ratios from angleHor/angleVert/nearPlane/dxViewWin/dyViewWin.
// Verified against disasm_func(0x82702358): no second argument is read (the decompiler's
// phantom `long double a2` parameter is a leftover fp scratch register, not a real arg).
void camCAMERA::RecomputeViewport()
{
    double halfAngleHorRad = (double)(angleHor * 0.017453292f * 0.5f);
    double tanHalfAngleHor = tan(halfAngleHorRad);
    float fullViewportWidth = (float)(nearPlane * (float)tanHalfAngleHor) * 2.0f; // dxViewPort

    double halfAngleVertRad = (double)(angleVert * 0.017453292f * 0.5f);
    double tanHalfAngleVert = tan(halfAngleVertRad);
    float halfViewportHeight = nearPlane * (float)tanHalfAngleVert; // half of dyViewPort

    float halfViewportWidth = fullViewportWidth * 0.5f;

    polyViewport.vert[1].x = halfViewportWidth;
    polyViewport.vert[2].x = halfViewportWidth;
    polyViewport.nVert = 4;
    polyViewport.vert[0].x = -halfViewportWidth;
    polyViewport.vert[3].x = -halfViewportWidth;

    float ratioDXW2DXViewWinLocal = dxViewWin / fullViewportWidth;

    xViewPortOffset = -halfViewportWidth;
    dxViewPort = fullViewportWidth;
    ratioDXW2DXViewWin = ratioDXW2DXViewWinLocal;
    polyViewport.nVert = 4; // redundant re-store present in disasm (stw r10,0x88 twice); kept for fidelity

    float fullViewportHeight = halfViewportHeight * 2.0f; // dyViewPort

    polyViewport.vert[0].y = -halfViewportHeight;
    polyViewport.vert[1].y = -halfViewportHeight;
    polyViewport.vert[2].y = halfViewportHeight;
    polyViewport.vert[3].y = halfViewportHeight;

    float ratioDYW2DYViewWinLocal = dyViewWin / fullViewportHeight;

    yViewPortOffset = -halfViewportHeight;
    dyViewPort = fullViewportHeight;
    ratioDYW2DYViewWin = ratioDYW2DYViewWinLocal;
}
