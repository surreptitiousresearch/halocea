#pragma once
#include "../m3d/m3dV.h"
#include "aiCMD_GOAL.h"
// ws-engine ai08: the per-frame command block a brain's mind fills in for its body to execute.
// DB-verified layout (types_members aiCMD2BODY): pBrain@0, goal@4 (aiCMD_GOAL, 48), posWpnTgt@0x34,
// posAlert@0x40, posLookAt@0x4C, isSetPosLookAt@0x58, posAim@0x5C — size 104.

struct aiBRAIN; // ai08 — owning brain (fwd)

struct aiCMD2BODY {
    aiBRAIN   *pBrain;         // 0x00 owning brain
    aiCMD_GOAL goal;           // 0x04 movement goal
    m3dV       posWpnTgt;      // 0x34 weapon target position
    m3dV       posAlert;       // 0x40 alert position
    m3dV       posLookAt;      // 0x4C explicit look-at position
    bool       isSetPosLookAt; // 0x58 whether posLookAt is active this frame
    m3dV       posAim;         // 0x5C aim position

    // 0x83178870 (?FillDefault@aiCMD2BODY@@QAAXXZ) — reset the command block to defaults: look
    // straight ahead, zero the goal position, and seed the goal reach values from the brain's
    // descriptor.
    void FillDefault();

    // 0x83175F68 (?LookStraight@aiCMD2BODY@@QAAXXZ) — set posLookAt to a point straight ahead of
    // the brain (body pos + 1.35*up + 2.0*forward), but only if a look-at wasn't already set this
    // frame. REVERSED: src/ws/ai/aiCMD2BODY__LookStraight.cpp.
    void LookStraight();

    // 0x83175F28 (?SetPosLookAt@aiCMD2BODY@@QAAXABUm3dV@@_N@Z) — set the explicit look-at position;
    // `force` overrides an already-set look-at this frame.  boundary.
    void SetPosLookAt(const m3dV &pos, bool force);
};
