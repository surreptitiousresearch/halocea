#pragma once
#include "../m3d/m3dSPL_LINEAR1D.h"
// ws-engine ai08: per-mind script-reader tunables block (aiMIND::scr). DB-verified layout
// (types_members aiSCRIPT_READER) — size 28 (0x1C): pBrain@0, curveDamage@4, weightBanWPs@24.

struct aiBRAIN; // ai08 — owning brain  boundary (fwd)

struct aiSCRIPT_READER {
    aiBRAIN        *pBrain;       // 0x00 owning brain
    m3dSPL_LINEAR1D curveDamage;  // 0x04 distance->damage response curve
    float           weightBanWPs; // 0x18 banned-waypoint weight penalty
};
