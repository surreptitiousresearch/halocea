#pragma once
#include "aiSELENE_BASE.h"
#include "aiSELENE_IFACE.h" // canonical aiSELENE_IFACE (base of aiSELENE, carries pBrain)
// Canonical declarations for the concrete "selene" (enemy-selection) mind arms.
// DB-verified (mangled ctors/dtors): both take an owning aiBRAIN*. aiSELENE_ZERO has a virtual dtor
// (??1aiSELENE_ZERO@@UAA@XZ) and IS-A aiSELENE_BASE (stored into dsSHARED_PTR<aiSELENE_BASE>).

struct aiBRAIN; // ai08 — per-unit AI brain   boundary (fwd)

// The concrete "real" selene arm. DB-verified layout (types_members aiSELENE): aiSELENE_IFACE@0,
// timerSelectBestEnemy@8, timerWaitCDTResult@12 — size 16. 0x832B3968 (??0aiSELENE@@QAA@PAVaiBRAIN@@@Z).
struct aiSELENE : aiSELENE_IFACE {
    float timerSelectBestEnemy; // 0x08 countdown to the next best-enemy selection
    float timerWaitCDTResult;   // 0x0C countdown to the pending find-enemy CDT result

    aiSELENE(aiBRAIN *pBrain); // boundary ctor

    // 0x832B39D0 (?UpdateTimers@aiSELENE@@UAAXM@Z) — virtual: age the enemy-selection timers by `dt`.
    // REVERSED: aiSELENE__UpdateTimers.cpp.
    void UpdateTimers(float dt);

    // 0x832B3A18 (?PrepareFEForBestVisible@aiSELENE@@MAAXXZ) — protected virtual: (re)build and narrow
    // the planner's enemy filter down to the shootable, potentially-visible candidates, sorted by
    // distance. REVERSED: aiSELENE__PrepareFEForBestVisible.cpp.
    void PrepareFEForBestVisible();
};

// The null ("zero") selene arm. 0x83... (??0aiSELENE_ZERO@@QAA@PAVaiBRAIN@@@Z); virtual dtor
// ??1aiSELENE_ZERO@@UAA@XZ inherited. Its vtable object (??_7aiSELENE_ZERO@@6B@ @0x820C2790) is
// written into an aiSELENE_IFACE's __vftable to demote it to the no-op selene (aiMIND::ZeroSelene).
struct aiSELENE_ZERO : aiSELENE_BASE {
    aiSELENE_ZERO(aiBRAIN *pBrain); // boundary ctor (virtual dtor inherited)
    static const void *vftable;     // `aiSELENE_ZERO::`vftable'' — boundary
};
