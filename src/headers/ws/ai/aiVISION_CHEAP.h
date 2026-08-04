#pragma once
#include "aiVISION_BATTLE.h"
// ws-engine ai08: aiVISION_CHEAP — the reduced-cost battle vision arm (single cheap-vision cache
// lookup instead of a full CDT probe). DB-verified layout (types_members aiVISION_CHEAP): a bare
// aiVISION_BATTLE base @0x00, size 0x10C (no added data).

struct aiVISION_CHEAP : aiVISION_BATTLE {
    // (??0aiVISION_CHEAP@@QAA@PAVaiBRAIN@@@Z) — construct a cheap vision arm for `brain`.
    // boundary — body external to this batch (repoints the vtable in the ctor tail).
    aiVISION_CHEAP(aiBRAIN *brain);

    // 0x83291A48 (?UpdateLatestTest@aiVISION_CHEAP@@MAAXXZ) — protected virtual: refresh the latest
    // visibility test from the planner's cheap-vision cache.
    // REVERSED: src/ws/ai/aiVISION_CHEAP__UpdateLatestTest.cpp
    void UpdateLatestTest();
};
