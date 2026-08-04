#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsVECTOR.h"

// ws-engine ai08 (a8_task_sys) — static descriptor table row mapping an SSL task-fn name to its
// after-fire semantics. DB-verified layout (types_members AI_SSL_TASK_FN_INFO): fnName@0,
// checkFnResult@4, checkBhvResults@5, bhvNameParamIdx@8, fnChecker@0xC — size 0x1C.

struct aiBRAIN; // ai/aiBRAIN.h  boundary (fwd)
struct dsDATA;  // ds/dsDATA.h  boundary (fwd)

struct AI_SSL_TASK_FN_INFO {
    dsSTRID       fnName;          // 0x00 interned task-fn name
    bool          checkFnResult;   // 0x04 gate after-fire on the fn result
    bool          checkBhvResults; // 0x05 gate after-fire on behaviour results
    int           bhvNameParamIdx; // 0x08 param index carrying the behaviour name (-1 = none)
    // 0x0C __int128 — force-AF checker delegate: bool (aiBRAIN::*)(const dsVECTOR<dsDATA,8>&) const.
    unsigned char fnChecker[16];
};

// 0x8416BA28 — the built-in task-fn descriptor table. Its scan is bounded by the address of the
// RTTI descriptor emitted right after it (??_R0?AVaiDOM_SPW_IFACE@@@8 @ 0x8416BAD0); the resulting
// span (0xA8 bytes / 0x1C stride) is exactly 6 rows. boundary (defined in a8_task_sys data segment).
enum { AI_SSL_TASK_FN_INFO_COUNT = 6 };
extern const AI_SSL_TASK_FN_INFO sslTaskFnInfo[AI_SSL_TASK_FN_INFO_COUNT];
