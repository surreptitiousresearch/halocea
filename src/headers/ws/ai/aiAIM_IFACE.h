#pragma once
#include "aiAIM_BASE.h"
// ws-engine ai08: aim module bound to a specific brain. DB-verified layout (types_members
// aiAIM_IFACE): aiAIM_BASE@0, pBrain@4 — size 8.
//   slot 8 (+0x20): FillResults  (added here; base slot is pure/absent)

struct aiBRAIN;
struct m3dV;

struct aiAIM_IFACE : aiAIM_BASE {
    aiBRAIN *pBrain; // 0x04 owning brain

    // 0x83299C58 (?FillResults@aiAIM_IFACE@@UAAXABUm3dV@@0@Z) — publish the resolved aim/weapon-target
    // positions into the brain command block. REVERSED: aiAIM_IFACE__FillResults.cpp.
    void FillResults(const m3dV &newPosAim, const m3dV &newPosWpnTgt);
};
