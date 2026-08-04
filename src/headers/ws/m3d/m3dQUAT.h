#pragma once
#include "m3dV.h"
// ws-engine m3d math: quaternion (scalar + vector). DB-verified layout (types_members m3dQUAT):
//   s@0 (float), v@4 (m3dV, 12) — size 16.

typedef struct m3dQUAT {
    float s;  // 0x00 scalar part
    m3dV  v;  // 0x04 vector part

    // 0x82... (??_0/?Identity@m3dQUAT@@QAAXXZ) — set to the identity rotation (s=1, v=0). boundary
    void Identity();
} m3dQUAT;
