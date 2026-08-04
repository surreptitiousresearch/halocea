#pragma once
#include "aiAIM_IFACE.h"
#include "../m3d/m3dV.h"
// ws-engine ai08: the "null" aim module — aims straight ahead of the body. DB-verified layout
// (types_members aiAIM_ZERO): aiAIM_IFACE@0, posStraight@8, isFilled@0x14.

struct aiBRAIN;

struct aiAIM_ZERO : aiAIM_IFACE {
    m3dV          posStraight; // 0x08 straight-ahead aim point (computed once)
    bool          isFilled;    // 0x14 whether posStraight has been computed
    unsigned char _pad15[3];   // 0x15 align

    // 0x831749B0 (??0aiAIM_ZERO@@QAA@PAVaiBRAIN@@@Z) — construct bound to owning brain.
    aiAIM_ZERO(aiBRAIN *);

    // 0x83299E10 (?ProcessFrame@aiAIM_ZERO@@UAAXXZ) — per-frame update: aim 300 units ahead of the
    // body, then publish. REVERSED: aiAIM_ZERO__ProcessFrame.cpp.
    void ProcessFrame();
};
