#pragma once
#include "m3dV.h"
// ws-engine m3d: capped cylinder primitive (two end centres + radius). DB-verified layout
// (headers_ref m3dCYL) — size 0x1C.

struct m3dCYL {
    m3dV  from;   // 0x00 one cap centre
    m3dV  to;     // 0x0C other cap centre
    float radius; // 0x18
};
