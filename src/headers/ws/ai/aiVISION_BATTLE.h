#pragma once
#include "aiVISION_COMMON.h"
// ws-engine ai08: aiVISION_BATTLE — the full-fidelity combat vision arm. DB-verified layout
// (types_members aiVISION_BATTLE): a bare aiVISION_COMMON base @0x00, size 0x10C (no added data).

struct aiVISION_BATTLE : aiVISION_COMMON {
    // 0x?  (??0aiVISION_BATTLE@@QAA@PAVaiBRAIN@@@Z) — construct a battle vision arm for `brain`.
    // boundary — body external to this batch (repoints the vtable in the ctor tail).
    aiVISION_BATTLE(aiBRAIN *brain);

    // 0x83292880 (?ProcessInit@aiVISION_BATTLE@@UAAXXZ) — virtual: base init + copy the descriptor's
    // battle sense ranges into the status perception block.
    // REVERSED: src/ws/ai/aiVISION_BATTLE__ProcessInit.cpp
    void ProcessInit();
    // 0x83291988 (?CalculateLostSightTime@aiVISION_BATTLE@@MBAMM@Z) — protected virtual const: how
    // long a target stays "seen" after it breaks visibility, as a function of distance.
    // REVERSED: src/ws/ai/aiVISION_BATTLE__CalculateLostSightTime.cpp
    float CalculateLostSightTime(float dist) const;
};
