#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsCONST_ARRAY.h"
// ws-engine ai08: per-weapon trajectory checker. Owns a fixed pool of grouped strike-trajectory
// records it ages every frame. DB-verified layout (types_members aiTRAJ_CHECKER): pBrain@0,
// wpn@4, groupStrikes@8 (dsCONST_ARRAY<aiTRAJ_CHECKER::STRIKE_GROUP,10>, 204) — size 212.
// STRIKE_GROUP is DB-verified (types_members aiTRAJ_CHECKER::STRIKE_GROUP, size 20): ties@0.

struct aiBRAIN;   // ai08 — owning brain (fwd)
struct aiWEAPON;  // ai08 — owning weapon module (fwd)
struct aiS_TIE;   // aiS_TIE.h — strike tie record (fwd; vector element via pointer)

struct aiTRAJ_CHECKER {
    // A group of strike ties checked against one trajectory. DB: aiTRAJ_CHECKER::STRIKE_GROUP.
    struct STRIKE_GROUP {
        dsVECTOR<aiS_TIE, 8> ties; // 0x00 member strike ties
    };

    aiBRAIN      *pBrain;            // 0x00 owning brain
    aiWEAPON     *wpn;              // 0x04 owning weapon module
    dsCONST_ARRAY<STRIKE_GROUP, 10> groupStrikes; // 0x08 grouped strike-trajectory pool (204)

    // ?UpdateTimers@aiTRAJ_CHECKER@@QAAXM@Z — age every grouped strike's freshness/validity
    // timers by `dt`. boundary — body external to this batch.
    void UpdateTimers(float dt);

    // Debug trajectory-checker entry points (a8_debug.cpp). boundary — bodies external to batch.
    aiTRAJ_CHECKER(aiBRAIN *brain, aiWEAPON *weapon); // 0x831999xx ctor
    void RebuildStrikeGroups();
    void ProcessFrame();
};
