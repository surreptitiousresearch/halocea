#include "../../headers/ws/ai/aiVIS_RESULTS.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // m3dVZero

// aiVIS_RESULTS::FakeFill @ 0x83291CE8
// ?FakeFill@aiVIS_RESULTS@@QAAXABUm3dV@@@Z
//
// Synthesise a "the enemy is right here" result at posFake: all world sight points collapse to
// posFake, the LCS points and free-space list collapse to the origin, and the result is marked as
// defined. isSucceeded is intentionally left untouched (matches the binary).
void aiVIS_RESULTS::FakeFill(const m3dV &posFake)
{
    points.posRandom    = posFake;
    points.posBottom    = posFake;
    points.posTop       = posFake;
    points.posRandomLCS = m3dVZero;
    points.posBasic     = posFake;
    points.posBasicLCS  = m3dVZero;

    m3dV zero = m3dVZero;
    pointsFree.Clear(zero);
    isUndef = false;
}
