#pragma once
#include "m3dV.h"
// ws-engine m3d math: a plane as (point, normal). DB-verified layout (types_members
// m3dPLANE): point@0 (m3dV), normal@12 (m3dV) — size 24.
typedef struct m3dPLANE {
    m3dV point;  /* 0x00 */
    m3dV normal; /* 0x0C */
} m3dPLANE;
