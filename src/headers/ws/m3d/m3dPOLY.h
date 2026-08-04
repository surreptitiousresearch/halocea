#pragma once
#include "m3dV.h"
// ws-engine m3d: a convex polygon with up to 20 vertices. DB-verified layout
// (types_members m3dPOLY): nVert@0 (int), vert@4 (m3dV[20]) — size 244.

typedef struct m3dPOLY {
    int  nVert;    // 0x00 number of live vertices
    m3dV vert[20]; // 0x04 vertex positions
} m3dPOLY;
