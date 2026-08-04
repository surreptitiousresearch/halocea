#pragma once
// ws-engine (Saber) m3d math: RGBA color.
// DB-verified layout (types_members m3dCOLOR): r@0, g@4, b@8, a@12 — all float, size 16.

typedef struct m3dCOLOR {
    float r;
    float g;
    float b;
    float a;
} m3dCOLOR;
