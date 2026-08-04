#pragma once
#include "../m3d/m3dV.h"
#include "../m3d/m3dVTX.h"
// ws-engine rend: a single screen-space textured/colored vertex used by the immediate-mode UI
// vertex buffers (ui_new::UI_STAT_RENDER::vBuf etc).
// DB-verified layout (types_members rendV3D): v@0 (m3dV, 12B position), color@12 (unsigned int,
// packed ARGB), tc@16 (m3dVTX, 8B texture coordinate) — size 24.

typedef struct rendV3D {
    m3dV         v;      // 0x00 position
    unsigned int color;  // 0x0C packed ARGB
    m3dVTX       tc;     // 0x10 texture coordinate (s,t)
} rendV3D;
