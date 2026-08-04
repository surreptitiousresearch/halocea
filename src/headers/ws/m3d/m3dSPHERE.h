#pragma once
#include "m3dV.h"
// ws-engine m3d: bounding sphere (center + radius). DB-verified (headers_ref m3dSPHERE) — size 0x10.

struct m3dSPHERE {
    m3dV  org;    // 0x00 center
    float radius; // 0x0C
};
