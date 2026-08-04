#pragma once
#include "../m3d/m3dV.h"
#include "../ds/dsTSTRING.h"

// ws-engine ai08 — base of a brain's vision module (aiMIND::vision pointee). Only the two virtuals
// reached by aiFILT_ENEMY::PassPotentiallyVisible are declared; the full layout/bodies are the next
// frontier.  boundary.

struct aiVISION_BASE {
    void *__vftable; // 0x00 aiVISION_BASE_vtbl*

    // vtbl — write this brain's eye/viewpoint sample into `outPosEye`.  boundary.
    void CalcPosEye(m3dV *outPosEye);
    // vtbl — is `target` potentially visible from `eye`?  boundary.
    bool IsPotentiallyVisible(const m3dV *eye, const m3dV *target);

    // vtbl+0x24 — apply a named-tunable value. Returns true if consumed. Used by aiMIND::SetPar.
    // boundary.
    bool SetPar(const dsTSTRING<char> &key, float val);
    // vtbl — per-spawn init hook. Used by aiMIND::ProcessINITLive.  boundary.
    void ProcessInit();

    // vtbl — true for a "real" (battle/idle) vision arm, false for the null/zero sentinel. Used by
    // aiMIND::DecideAppropriateVision.  boundary.
    bool IsReal();

    // vtbl — per-frame vision update. Used by aiMIND::UpdateAllPerception.  boundary.
    void ProcessFrame();

    // ?UpdateTimers@aiVISION_BASE@@UAAXM@Z — virtual: advance this module's timers by `dt` seconds.
    // Used by aiMIND::UpdateTimers.  boundary.
    void UpdateTimers(float dt);

    // vtbl — true when this vision arm is a sniper's (long-range) vision. Used by
    // aiDEF_CDT_SYS::PostVisibilityRequest to pick the request variant.  boundary.
    bool IsSniper();
    // vtbl — true when a cheap (single-ray) visibility test suffices for this arm.  boundary.
    bool IsNeedSimplestVision();
};
