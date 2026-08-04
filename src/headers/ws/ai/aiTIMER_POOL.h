#pragma once
#include "../ds/dsVECTOR.h"
// ws-engine ai08: a brain's pool of countdown timers.
// DB-verified layout (types_members aiTIMER_POOL): timers@0 (dsVECTOR<aiTIMER,8>, 20),
// lastID@0x14 (int) — size 24. Each aiTIMER derives from aiTIMER_DATA (the event payload copied
// out into the fired-events vector when the timer elapses).

struct aiTIMER_POOL {
    // DB-verified layout (types_members aiTIMER_POOL::aiTIMER_DATA) — size 8.
    struct aiTIMER_DATA {
        int id;      // 0x00 timer identity
        int context; // 0x04 caller-supplied behaviour context
    };

    // DB-verified layout (types_members aiTIMER_POOL::aiTIMER):
    //   aiTIMER_DATA@0 (base, 8), id@8, timer@0xC, isCycle@0x10, defVal@0x14, context@0x18 — size 28.
    // The derived id/context shadow the base fields (both present in the DB layout).
    struct aiTIMER : aiTIMER_DATA {
        int          id;      // 0x08 (shadows aiTIMER_DATA::id)
        float        timer;   // 0x0C current remaining time
        bool         isCycle; // 0x10 auto-reload when it hits zero
        float        defVal;  // 0x14 reload value
        int          context; // 0x18 (shadows aiTIMER_DATA::context)

        // 0x83176E40 (?CheckDone@aiTIMER@aiTIMER_POOL@@QAA_NXZ) — true when the timer has
        // elapsed (timer < ~0). On a cycling timer, reload it to defVal.
        bool CheckDone();
    };

    dsVECTOR<aiTIMER, 8> timers; // 0x00 live timers
    int                  lastID; // 0x14 rolling id allocator

    // 0x8317AAB0 (?AddTimer@aiTIMER_POOL@@QAAHM_NH@Z) — append a timer (time seconds, cyclic if
    // isCycle) carrying `context`; returns the assigned id (wraps to 0 on int overflow).
    int  AddTimer(float time, bool isCycle, int context);

    // 0x8317AB18 — collect every elapsed timer's event payload into `timerEvents`, re-arming
    // cyclic ones and erasing one-shots.
    void CheckTimers(dsVECTOR<aiTIMER_DATA, 8> &timerEvents);

    // 0x83179D50 — advance every timer by `dt` seconds (m3dUpdateTimeField on each).
    void Update(float dt);

    // 0x83179E18 — remove the first timer whose aiTIMER_DATA::id == `id`; returns true if found.
    bool RemoveTimerByID(int id);

    // 0x83179E88 — remove every timer whose aiTIMER_DATA::context == `context` (back-to-front).
    void RemoveTimersByContext(int context);
};
