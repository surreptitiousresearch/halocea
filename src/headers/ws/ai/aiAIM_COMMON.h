#pragma once
#include "aiAIM_IFACE.h"
#include "../ent/entTRACKER.h"
#include "../m3d/m3dV.h"
// ws-engine ai08: the common concrete aim module (base of aiAIM_IDLE / aiAIM_BATTLE). DB-verified
// layout (types_members aiAIM_COMMON): aiAIM_IFACE@0, trkAim@8 (entTRACKER), isFrozen@0x68,
// posToFreeze@0x6C, posBasic@0x78, posForProjectile@0x84.
//   slot 9 (+0x24): ApplyModifiers (added here; base slot is _purecall)

struct aiAIM_COMMON : aiAIM_IFACE {
    entTRACKER trkAim;            // 0x08 the tracked aim target (0x60)
    bool       isFrozen;         // 0x68 aim frozen this frame
    unsigned char _pad69[3];     // 0x69 align
    m3dV       posToFreeze;      // 0x6C last position captured while frozen
    m3dV       posBasic;         // 0x78 basic (pre-modifier) aim position
    m3dV       posForProjectile; // 0x84 aim point used for projectile prediction

    // 0x83299A88 (?GetPos@aiAIM_COMMON@@UBA?AUm3dV@@XZ) — const: current aim position (sret).
    // REVERSED: aiAIM_COMMON__GetPos.cpp.
    m3dV GetPos() const;
    // 0x8329A358 (?GetGivenAim@aiAIM_COMMON@@UBA?AV?$dsTSTRING@D@@XZ) — const: the assigned tracker's
    // name, or "" when no aim is set. REVERSED: aiAIM_COMMON__GetGivenAim.cpp.
    dsTSTRING<char> GetGivenAim() const;
    // 0x8329A638 (?SetTrk@aiAIM_COMMON@@UAAXPAVentTRACKER@@@Z) — install the aim tracker and seed the
    // aim positions from its center. REVERSED: aiAIM_COMMON__SetTrk.cpp.
    void SetTrk(entTRACKER *pTrk);
    // 0x83299F18 (?ProcessFrame@aiAIM_COMMON@@UAAXXZ) — per-frame aim update.
    // REVERSED: aiAIM_COMMON__ProcessFrame.cpp.
    void ProcessFrame();
    // 0x83299FD0 (?FillResults@aiAIM_COMMON@@UAAXABUm3dV@@0@Z) — publish resolved aim/target positions
    // (and debug mirrors). REVERSED: aiAIM_COMMON__FillResults.cpp.
    void FillResults(const m3dV &newPosAim, const m3dV &newPosWpnTgt);
    // 0x83299CB8 (?UpdateBasicPos@aiAIM_COMMON@@IAAXXZ) — protected: refresh posBasic/posToFreeze from
    // CalcBasicPos, honouring the freeze flag. REVERSED: aiAIM_COMMON__UpdateBasicPos.cpp.
    void UpdateBasicPos();
    // 0x83299D58 (?CalcBasicPos@aiAIM_COMMON@@MAA_NAAUm3dV@@@Z) — protected: compute the basic aim
    // position into `newPos`; returns whether a position was produced.
    // REVERSED: aiAIM_COMMON__CalcBasicPos.cpp.
    bool CalcBasicPos(m3dV &newPos);
    // 0x8329A298 (?ApplyModifiers@aiAIM_COMMON@@MAAXAAUm3dV@@0@Z) — protected: apply aim modifiers to
    // the aim/projectile positions.  boundary (body external to this batch).
    void ApplyModifiers(m3dV &posAim, m3dV &posWpnTgt);
};
