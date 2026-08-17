/* ?SetAngleHor@camCAMERA@@QAAXM@Z @0x82702468 */
#include "../../headers/ws/cam/camCAMERA.h"
#include <math.h>

// 0x82702468
void camCAMERA::SetAngleHor(float _angleHor)
{
    angleHor = _angleHor;

    // Derive the vertical FOV from the horizontal FOV and the viewport aspect ratio:
    // tan(halfVert) = aspectRatioHW * tan(halfHor).
    double halfAngleHorRad = (double)(angleHor * 0.5f * 0.017453292f); // deg -> rad, halved
    double tanHalfAngleHor = tan(halfAngleHorRad);
    double tanHalfAngleVert = (double)(aspectRatioHW * (float)tanHalfAngleHor);
    double halfAngleVertRad = atan(tanHalfAngleVert);
    angleVert = (float)(halfAngleVertRad * 2.0) * 57.295776f; // rad -> deg

    RecomputeViewport();
}
