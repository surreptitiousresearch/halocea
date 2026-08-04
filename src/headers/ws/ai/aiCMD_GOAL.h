#pragma once
#include "../m3d/m3dV.h"
// ws-engine ai08: the movement goal a brain commands its body toward.
// DB-verified layout (types_members aiCMD_GOAL): posGoal@0, posChkReach@0xC, distReach@0x18,
// distReachMultiplier@0x1C, dirGoal@0x20, dirCosMaxAngle@0x2C — size 48.

struct aiBRAIN;      // ai08 — owning brain (fwd)
struct aiBRAIN_DESC; // aiBRAIN_DESC.h — brain descriptor (fwd)

struct aiCMD_GOAL {
    m3dV  posGoal;             // 0x00 world-space goal position
    m3dV  posChkReach;         // 0x0C position used for the reach test
    float distReach;           // 0x18 reach radius
    float distReachMultiplier; // 0x1C scale applied to the requested reach distance
    m3dV  dirGoal;             // 0x20 desired approach direction (unit)
    float dirCosMaxAngle;      // 0x2C

    // 0x831783F8 (?SetPos@aiCMD_GOAL@@QAAXPAVaiBRAIN@@ABUm3dV@@M@Z) — set the goal position (and,
    // when it moved, recompute the approach direction relative to `pBrain`'s body and flag a big
    // turn on the brain's status). `dist` < 0 means "use half the multiplier".
    void SetPos(aiBRAIN *pBrain, const m3dV &pos, float dist);

    // 0x83175DB0 (?FillDefault@aiCMD_GOAL@@QAAXPAVaiBRAIN_DESC@@@Z) — reset the goal to defaults:
    // zero the goal + reach-check positions, 0.5 reach radius, and the descriptor's reach
    // multiplier. REVERSED: src/ws/ai/aiCMD_GOAL__FillDefault.cpp.
    void FillDefault(aiBRAIN_DESC *desc);

    // 0x83175E08 (?DetectBigTurn@aiCMD_GOAL@@IAAHABUm3dV@@@Z) — protected: nonzero when `newDir`
    // differs enough from the current goal direction to count as a sharp turn (both directions
    // must be non-degenerate; the dot product must fall below dirCosMaxAngle).
    // REVERSED: src/ws/ai/aiCMD_GOAL__DetectBigTurn.cpp.
    int DetectBigTurn(const m3dV &newDir);
};
