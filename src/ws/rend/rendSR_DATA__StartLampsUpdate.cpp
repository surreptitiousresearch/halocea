// rendSR_DATA__StartLampsUpdate @0x829EC380
#include "../../headers/ws/rend/rendSR_DATA.h"
#include "../../headers/ws/rend/rendSR_LAMP_DATA.h"
#include "../../headers/ws/m3d/m3dV.h"
#include <math.h>

// rendSR_DATA::StartLampsUpdate @ 0x829EC3DC
// Begin a per-frame static-lamp visibility pass at sample point `_pos`. If the point has not
// moved (within an epsilon on every axis), the cached lamp table is left untouched. Otherwise
// the sample position is stored and every lamp slot has its "seen-this-frame" (bit 25,
// 0x02000000) and "visible" (bit 27, 0x08000000) flags cleared so the pass can re-accumulate.

static const float kLampPosEpsilon = 0.000001f;

void rendSR_DATA::StartLampsUpdate(const m3dV *_pos)
{
    if (fabsf(pos.x - _pos->x) < kLampPosEpsilon
        && fabsf(pos.y - _pos->y) < kLampPosEpsilon
        && fabsf(pos.z - _pos->z) < kLampPosEpsilon)
        return;

    int nElem = lampsData.nElem;
    pos = *_pos;

    for (int i = 0; i < nElem; ++i) {
        lampsData[i].data &= ~0x02000000u; // clear "seen this frame"
        lampsData[i].data &= ~0x08000000u; // clear "visible"
    }
}
