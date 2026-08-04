#pragma once
// ws-engine "dip" spawn/domain point. DB-verified layout (types_members dipPOINT) — size 28 (0x1C):
//   pos@0 (m3dV), dir@0xC (m3dV), weight@0x18 (float).
#include "../m3d/m3dV.h"

struct dipPOINT {
    m3dV  pos;    // 0x00 world-space position
    m3dV  dir;    // 0x0C facing / orientation
    float weight; // 0x18 selection weight
};
