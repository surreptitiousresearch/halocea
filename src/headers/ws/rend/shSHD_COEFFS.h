#pragma once
#include "../m3d/m4dV.h"
// ws-engine rend: directional spherical-harmonics coefficient bundle.
// DB-verified layout (types_members shSHD_COEFFS): cAr@0, cAg@16, cAb@32, cBr@48, cBg@64,
// cBb@80, cC@96 — all m4dV (16B), size 112.

typedef struct shSHD_COEFFS {
    m4dV cAr; // 0x00
    m4dV cAg; // 0x10
    m4dV cAb; // 0x20
    m4dV cBr; // 0x30
    m4dV cBg; // 0x40
    m4dV cBb; // 0x50
    m4dV cC;  // 0x60
} shSHD_COEFFS;
