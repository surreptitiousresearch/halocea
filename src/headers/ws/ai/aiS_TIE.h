#pragma once
#include "../ds/dsSTRID.h"
#include "aiW_IDX.h"
// ws-engine ai08: a "strike tie" — the binding of a chosen strike (weapon controller + descriptor
// + per-strike vars) that a weapon module currently has selected.
// DB-verified layout (types_members aiS_TIE): idxCtrl@0, desc@4, vars@8, idxGroup@0xC, name@0x10 —
// size 20.

struct propWPN_AI_USABLE; // prop subsystem — AI-usable weapon controller (fwd)
struct wpnS_DESC;         // wpn — weapon strike descriptor (fwd)
struct aiS_VARS;          // ai08 — per-strike cached vars (fwd)
struct STRIKE_GROUP;      // ai08 — strike group handle target (fwd)

struct aiS_TIE {
    aiW_IDX<propWPN_AI_USABLE> idxCtrl;  // 0x00 selected controller index
    const wpnS_DESC           *desc;     // 0x04 strike descriptor
    aiS_VARS                  *vars;     // 0x08 per-strike cached vars
    aiW_IDX<STRIKE_GROUP>      idxGroup; // 0x0C strike-group index
    dsSTRID                    name;     // 0x10 strike name id

    // ?IsNull@aiS_TIE@@QBA_NXZ — true when no strike is currently tied (empty selection).
    // boundary — body external to this batch.
    bool IsNull() const;
};
