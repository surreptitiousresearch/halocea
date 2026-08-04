#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine dbg: per-actor debug record stored in the cinematic-actor-capture map
// (ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,...>, keyed by the actor's interned name).
// DB-verified layout (types_members DBG_CINE_ACTOR_INFO):
//   filePath@0 (dsTSTRING<char>), wasUpdatedCurFrame@4 (bool) — size 8 (padded).

typedef struct DBG_CINE_ACTOR_INFO {
    dsTSTRING<char> filePath;           // 0x00 capture output path for this actor
    bool            wasUpdatedCurFrame; // 0x04 set once the actor's capture record is refreshed this frame
} DBG_CINE_ACTOR_INFO;
