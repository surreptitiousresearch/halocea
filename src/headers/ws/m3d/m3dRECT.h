#pragma once
// ws-engine (Saber) m3d math: axis-aligned float rectangle (left/top/right/bottom).
// DB-verified layout (types_members m3dRECT): l@0, t@4, r@8, b@12 — all float, size 16.

typedef struct m3dRECT {
    float l;
    float t;
    float r;
    float b;
} m3dRECT;
