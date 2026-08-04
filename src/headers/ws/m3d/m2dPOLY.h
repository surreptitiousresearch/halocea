#pragma once
#include "m2dV.h"
// ws-engine (Saber) m3d math: 2D polygon (used by camCAMERA for the viewport quad).
// DB-verified layout (types_members m2dPOLY): nVert@0 (int), vert@4 (m2dV[20]) — size 164.

typedef struct m2dPOLY {
    int   nVert;
    m2dV  vert[20];
} m2dPOLY;
