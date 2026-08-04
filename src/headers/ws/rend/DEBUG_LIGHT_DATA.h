#pragma once
#include "../m3d/m3dV.h"
// ws-engine rend: debug record pushed into dbgPlayerLights when dbg_lightSetShowAllLights is on.
// DB-verified layout (types_members DEBUG_LIGHT_DATA): pos@0(m3dV,12), atten@12(float) — size 16.

typedef struct DEBUG_LIGHT_DATA {
    m3dV  pos;   // 0x00 world position (light's L2W translation)
    float atten; // 0x0C attenuation
} DEBUG_LIGHT_DATA;
