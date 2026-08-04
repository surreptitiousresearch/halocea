#pragma once
// ws-engine m3d math: 2-component texture coordinate. DB-verified layout
// (types_members m3dVTX): s@0, t@4 — both float, size 8.

typedef struct m3dVTX {
    float s;
    float t;
} m3dVTX;
