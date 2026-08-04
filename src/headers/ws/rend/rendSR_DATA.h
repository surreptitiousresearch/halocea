#pragma once
#include "../m3d/m3dV.h"
#include "../ds/dsVECTOR.h"
#include "../lgt/lgtID.h"
// ws-engine rend: shader-replacement / surface-render data record.
// DB-verified layout (types_members rendSR_DATA) — size 44.

struct rendSR_LAMP_DATA; // rend subsystem per-lamp record (vector element)  boundary
struct osLOCK;
struct lgtID;            // lgtID.h — packed light identifier

typedef struct rendSR_DATA {
    m3dV                             pos;        // 0x00
    dsVECTOR<rendSR_LAMP_DATA, 8>    lampsData;  // 0x0C
    osLOCK                          *pLock;      // 0x20
    int                              priorClass; // 0x24
    int                              priorCur;   // 0x28

    // 0x829ECED0 — copy `src`'s pos and lampsData into this record (pLock/priorClass/priorCur
    // are per-instance runtime state, not copied). Mangle ABV1@@Z confirms a reference param;
    // disasm shows a plain 2-word field copy plus a tail-call into dsVECTOR::operator=.
    void Copy(const rendSR_DATA &src);

    // 0x8252EA88 — copy-assign (distinct from Copy() above; a separate symbol). Body not
    // reversed here.  boundary
    rendSR_DATA &operator=(const rendSR_DATA &src);

    // rend per-frame lamp-visibility bookkeeping, used by rendLIGHT_SET::UpdateStatic.
    // Reversed in src/ws/rend/rendSR_DATA__*.cpp.
    // 0x829EC3DC — if the sample point moved, clear the "seen"/"visible" bits on every lamp slot.
    void StartLampsUpdate(const m3dV *pos);
    // 0x829ECFCC — compact the lamp table, dropping any slot not marked visible this frame.
    void FinishLampsUpdate();
    // 0x829ED060 — mark `lampID` visible: return its cached visibility bit, or push a new slot.
    int IsLampVisible(const lgtID &lampID);
} rendSR_DATA;
