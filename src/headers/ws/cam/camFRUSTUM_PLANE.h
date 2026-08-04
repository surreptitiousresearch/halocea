#pragma once
#include "../m3d/m3dV.h"
// ws-engine (Saber) cam subsystem: one clip plane of a view frustum.
// DB-verified layout (types_members camFRUSTUM_PLANE): normal@0 (m3dV), dist@12 (float),
// nearest_vert@16 (int) — size 20.

typedef struct camFRUSTUM_PLANE {
    m3dV  normal;
    float dist;
    int   nearest_vert;
} camFRUSTUM_PLANE;
