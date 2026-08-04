#pragma once
#include "aiVIS_POINTS.h"
#include "CONST_ARRAY_m3dV.h"
// ws-engine ai08: aiVIS_RESULTS — the outcome of an enemy-visibility probe (the 4 sampled sight
// points + free-space points + success/undef flags). DB-verified layout (types_members
// aiVIS_RESULTS) — size 0xE0.

struct m3dV; // ws m3d — 3-vector (full def m3d/m3dV.h)

struct aiVIS_RESULTS {
    aiVIS_POINTS         points;      // 0x00 sampled sight points of the result
    CONST_ARRAY_m3dV<10> pointsFree;  // 0x60 free-space sample points
    bool                 isSucceeded; // 0xDC target was visible
    bool                 isUndef;     // 0xDD result not yet computed

    // 0x83291CE8 (?FakeFill@aiVIS_RESULTS@@QAAXABUm3dV@@@Z) — synthesise a trivially-visible result
    // at `posFake` (all sight points = posFake, LCS/free points zeroed, isUndef cleared).
    // REVERSED: src/ws/ai/aiVIS_RESULTS__FakeFill.cpp
    void FakeFill(const m3dV &posFake);
};
