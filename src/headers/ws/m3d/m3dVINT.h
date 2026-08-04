#pragma once
// ws-engine m3d math: integer 3-component vector. DB-verified (types_members m3dVINT):
// x@0, y@4, z@8 — all int, size 12.

typedef struct m3dVINT {
    int x;
    int y;
    int z;
} m3dVINT;
