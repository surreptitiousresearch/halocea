#pragma once
#include "../m3d/m3dV.h"
// ws-engine ai08 debug helper — a coloured point on the group WP-set gradient curve, revolved
// around the group axis to build the curve mesh. DB-verified layout (types_members
// aiDBG_SPLINE_POINT): pos@0 (m3dV), color@0xC (unsigned int) — size 16.

struct aiDBG_SPLINE_POINT {
    m3dV         pos;   // 0x00 point position (x = param, y = scaled value, z = 0)
    unsigned int color; // 0x0C ARGB gradient colour
};
