#pragma once
#include "aiVISION_BATTLE.h"
#include "../ds/dsCONST_ARRAY.h"
// ws-engine ai08: aiVISION_SNIPER — the long-range sniper vision arm. DB-verified layout
// (types_members aiVISION_SNIPER): aiVISION_BATTLE base @0x00 (0x10C), then a rolling
// VISION_STATS window @0x10C — size 0x13C.

struct aiVISION_SNIPER : aiVISION_BATTLE {
    // aiVISION_SNIPER::VISION_STATS — a fixed 5-slot value/weight ring used to smooth the sniper's
    // reported view-percent. DB-verified layout (types_members aiVISION_SNIPER::VISION_STATS),
    // size 0x30.
    struct VISION_STATS {
        dsCONST_ARRAY<float, 5> values;  // 0x00
        dsCONST_ARRAY<float, 5> weights; // 0x18

        // 0x83291F50 (??0VISION_STATS@aiVISION_SNIPER@@QAA@XZ). boundary.
        VISION_STATS();
        // 0x83291070 (??1VISION_STATS@aiVISION_SNIPER@@QAA@XZ). boundary.
        ~VISION_STATS();
        // 0x83292038 (?PushFront@VISION_STATS@aiVISION_SNIPER@@QAAAAMABM@Z) — push a fresh sample.
        // boundary.
        float &PushFront(const float &value);
        // 0x832920D0 (?CalcAverage@VISION_STATS@aiVISION_SNIPER@@QBAMXZ) — weighted average of the
        // window. boundary.
        float CalcAverage() const;
    };

    VISION_STATS viewLatestStats; // 0x10C rolling window of the latest test results

    // (??0aiVISION_SNIPER@@QAA@PAVaiBRAIN@@@Z) — construct a sniper vision arm for `brain`.
    // boundary — body external to this batch (repoints the vtable in the ctor tail).
    aiVISION_SNIPER(aiBRAIN *brain);

    // 0x832921E0 (?MixesLatestToOut@aiVISION_SNIPER@@MAAXXZ) — protected virtual: blend the latest
    // stored test into the reported view-percent using the smoothing window.
    // REVERSED: src/ws/ai/aiVISION_SNIPER__MixesLatestToOut.cpp
    void MixesLatestToOut();
};
