#pragma once
// ws-engine ai08: a brain's "trash bin" — deferred-destruction bookkeeping.
// DB-verified layout (types_members aiTRASH_BIN) — size 44 (0x2C):
//   arrBhv@0 (dsVECTOR<bhvBEHAV*,8>, 20B), arrEne@0x14 (dsVECTOR<aiENEMY_BASE*,8>, 20B),
//   pBrain@0x28 (aiBRAIN*).
#include "../ds/dsVECTOR.h"

struct bhvBEHAV;      // ai08 — behaviour object                     (fwd)
struct aiENEMY_BASE;  // ai08 — enemy-tracking module                (fwd)
struct aiBRAIN;       // ai08 — owning brain                         (fwd)

struct aiTRASH_BIN {
    dsVECTOR<bhvBEHAV *, 8>     arrBhv; // 0x00 behaviours queued for deferred destruction
    dsVECTOR<aiENEMY_BASE *, 8> arrEne; // 0x14 enemy modules queued for deferred destruction
    aiBRAIN                    *pBrain; // 0x28 owning brain

    // 0x832AB3F8 (??0aiTRASH_BIN@@QAA@PAVaiBRAIN@@@Z) — construct bound to `pBrain`. boundary.
    aiTRASH_BIN(aiBRAIN *pBrain);
    // 0x83178140 (??1aiTRASH_BIN@@QAA@XZ) — destroy. boundary.
    ~aiTRASH_BIN();

    // 0x832AB008 (?Update@aiTRASH_BIN@@QAAXXZ) — flush the deferred-destruction queues: if the brain
    // is flagged for removal terminate/kill it, then destroy all queued behaviours and enemy modules.
    // REVERSED: src/ws/ai/aiTRASH_BIN__Update.cpp.
    void Update();

    // 0x832AB4B0 (?Trash@aiTRASH_BIN@@QAAXPAVbhvBEHAV@@@Z) — queue a behaviour for deferred
    // destruction. boundary.
    void Trash(bhvBEHAV *pBehav);
    // 0x832AB4D8 (?Trash@aiTRASH_BIN@@QAAXPAVaiENEMY_BASE@@@Z) — queue an enemy module for deferred
    // destruction. boundary.
    void Trash(aiENEMY_BASE *pEnemy);
    // 0x832AB508 (?Trash@aiTRASH_BIN@@QAAXPAVaiBRAIN@@@Z) — mark the brain itself for deferred removal
    // and (if it is suspended) flush immediately. REVERSED: src/ws/ai/aiTRASH_BIN__Trash.cpp.
    void Trash(aiBRAIN *pBrain);
};
