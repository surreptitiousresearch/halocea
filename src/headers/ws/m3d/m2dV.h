#pragma once
// ws-engine (Saber) m3d math: 2-component vector (screen/viewport space).
// DB-verified layout (types_members m2dV): x@0, y@4 — all float, size 8.

typedef struct m2dV {
    float x;
    float y;
} m2dV;
