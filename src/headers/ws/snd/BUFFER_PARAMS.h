#pragma once
#include "../ds/dsTSTRING.h"
#include "../m3d/m3dV.h"
// ws-engine snd: parameter block passed to snd::BUFFER-creation calls (play position, timing,
// blocking flag, completion callback, and a debug name string).
// DB-verified layout (types_members snd::BUFFER_PARAMS): pos@0 (m3dV,12), timeOffset@12 (int),
// blocking@16 (bool), callback@20 (fn ptr), userData@24 (void*), dbgInfo@28 (dsTSTRING<char>) —
// size 32.

namespace snd {

struct NOTIFICATION_INFO; // boundary — external to this batch

typedef struct BUFFER_PARAMS {
    m3dV pos;                                                       // 0x00
    int timeOffset;                                                 // 0x0C
    bool blocking;                                                  // 0x10
    unsigned char _pad0[3]; /* db-verified padding */
    void (*callback)(const NOTIFICATION_INFO *, void *); // 0x14
    void *userData;                                                 // 0x18
    dsTSTRING<char> dbgInfo;                                        // 0x1C

    // 0x823D64D8 — default-construct: zero the position/timing/blocking/callback/userData fields
    // and initialize `dbgInfo` to the shared empty dsTSTRING singleton (via the dsTSTRING<char>
    // function-local-static UnsafeInitEmpty machinery), bumping its refCount.
    BUFFER_PARAMS();
} BUFFER_PARAMS;

} // namespace snd
