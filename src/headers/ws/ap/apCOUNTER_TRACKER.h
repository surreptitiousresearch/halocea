#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
// ws-engine ap (platform): records per-frame counter activity to a file. Owns an event log
// of counter-add/remove events observed while tracking is running.
// DB-verified layout (types_members apCOUNTER_TRACKER) — size 40.

struct apCOUNTER; // boundary — ap subsystem counter (only referenced by pointer here)

// Nested event log. DB-verified (types_members apCOUNTER_TRACKER_EVENT_LOG) — size 24.
typedef struct apCOUNTER_TRACKER_EVENT_LOG {
    struct LOG_ITEM; // boundary — opaque log entry (stored by pointer in the vector)
    dsVECTOR<LOG_ITEM, 8> items;    // 0x00
    int                   sizeText; // 0x14
} apCOUNTER_TRACKER_EVENT_LOG;

typedef struct apCOUNTER_TRACKER {
    bool                        started;     // 0x00
    bool                        stopped;     // 0x01
    unsigned char               _pad0[2];    // 0x02 db-verified padding
    int                         nFrameAll;   // 0x04
    bool                        cntAdded;    // 0x08 set when a counter was added mid-run
    unsigned char               _pad1[3];    // 0x09 db-verified padding
    dsTSTRING<char>             cntFileName; // 0x0C
    apCOUNTER_TRACKER_EVENT_LOG eventLog;    // 0x10

    // 0x82715088 — default ctor: cleared flags/counters, empty `cntFileName`, empty `eventLog`.
    apCOUNTER_TRACKER();

    // 0x82718398 — flag every unit of `counter` with the 0x40 "late-added" state bit (it was
    // registered while tracking was already running) and record that a counter was added.
    void AddLateCounter(apCOUNTER *counter);
} apCOUNTER_TRACKER;
