#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsPAIR.h"
#include "../ds/dsSTRID.h"
#include "sslERROR.h"
#include "sslOBJ_REF.h"
#include "sslCLASS_REF.h"
#include "sslSTACK.h"
#include "sslCLASS_MAP_ELEM.h"
#include "sslBREAKPOINT.h"
// ssl subsystem: the SSL script virtual machine / system object. Full DB-verified layout
// (types_members sslSYSTEM) — every field named. The dsVECTOR members store T* internally, so the
// (few) still-opaque element types below only need forward declarations.

struct sslDELAYED_FUNCTION;       // ssl deferred-call record (full definition in sslDELAYED_FUNCTION.h)
struct sslSCRIPTED_FUNCTION_BASE; // ssl compiled function base (full definition in sslSCRIPTED_FUNCTION_BASE.h)
struct dsDATA_TYPE;               // ds runtime type descriptor (full definition in ds/dsDATA_TYPE.h)
struct sslUTIL;                   // boundary — script utility helper
struct sslSOURCE_REF;             // ref to a script source-text record (full definition in sslSOURCE_REF.h)
struct sslCB_HOST;                // boundary — native callback host object (full definition in sslCB_HOST.h)

typedef struct sslSYSTEM {
    // Action applied to matching delayed-function records by DelayedFuncAction.
    // DB-verified (types_enum_values sslSYSTEM::DELACT).
    enum DELACT {
        DELACT_PAUSE   = 0, // increment the record's pause-lock count
        DELACT_RESUME  = 1, // decrement the pause-lock count (clamped at 0)
        DELACT_KILL    = 2, // clear the record (cancel the pending call)
        DELACT_CALLNOW = 3, // force the call to fire this frame, then clear the record
    };

    sslERROR                                            lastErr;           // 0x00 last error raised
    dsVECTOR<sslOBJ_REF, 8>                             allObjects;        // 0x0C every live object
    dsVECTOR<sslOBJ_REF, 8>                             nameToObject;      // 0x20 name-sorted objects
    dsVECTOR<sslCLASS_MAP_ELEM, 8>                      namedClasses;      // 0x34 name-sorted classes
    sslSTACK                                            stack;             // 0x48 evaluation stack
    sslOBJ_REF                                          globalObj;         // 0x98 shared "global" object
    dsVECTOR<dsPAIR<int, dsTSTRING<char> >, 8>          classNames;        // 0x9C uid -> class name
    dsVECTOR<dsPAIR<int, dsTSTRING<char> >, 8>          objectNames;       // 0xB0 uid -> object name
    dsVECTOR<sslBREAKPOINT, 8>                          bpData;            // 0xC4 registered breakpoints
    int                                                 dbgMode;           // 0xD8
    sslSCRIPTED_FUNCTION_BASE                          *dbgFunc;           // 0xDC
    int                                                 dbgBCPosition;     // 0xE0
    int                                                 dbgParamStackPos;  // 0xE4
    int                                                 dbgObject;         // 0xE8
    bool                                                showErrors;        // 0xEC
    unsigned char                                       _pad0[3];          // 0xED db-verified padding
    dsVECTOR<dsTSTRING<char>, 8>                        errorCallStack;    // 0xF0
    bool                                                disFuncLog;        // 0x104
    bool                                                disEventLog;       // 0x105
    unsigned char                                       _pad1[2];          // 0x106 db-verified padding
    dsVECTOR<sslDELAYED_FUNCTION *, 8>                  delayedFuncs;      // 0x108
    dsVECTOR<dsPAIR<dsSTRID, const dsDATA_TYPE *>, 8>   stdTypes;          // 0x11C
    sslUTIL                                            *curUtil;           // 0x130
    dsTSTRING<char>                                     logPref;           // 0x134
    dsTSTRING<char>                                     logPost;           // 0x138
    int                                                 logFrames;         // 0x13C
    float                                               logTime;           // 0x140
    int                                                 logFramesTotal;    // 0x144
    float                                               logTimeTotal;      // 0x148
    int                                                 logFuncActive;     // 0x14C
    void                                              (*pfnObjectAdded)(sslOBJ_REF);   // 0x150
    void                                              (*pfnObjectRemoved)(sslOBJ_REF); // 0x154
    bool                                                isDelayedFuncCall; // 0x158
    bool                                                isSuspendAdd;      // 0x159

    // 0x823CDAF0 — const getter for the "suspend script-object additions" flag. Reversed in
    // sslSYSTEM__IsSuspendAdd.cpp.
    bool IsSuspendAdd() const;

    // 0x825CC270 — write a NUL-terminated log line: emit a pending frame/timing banner, then a line
    // of the form <indent><logPref><text><logPost> through the global sslLogCb; clears logPref/logPost.
    void Log(const char *text);
    // 0x825CD610 — write a dsTSTRING log line (forwards to Log(const char*)).
    void Log(const dsTSTRING<char> &str);
    // 0x825CA340 — increase the global log indentation depth.
    void LogTab();
    // 0x825CA358 — decrease the global log indentation depth (clamped at 0).
    void LogUntab();
    // 0x825C9A48 — copy of the shared global object reference (returns sslOBJ_REF by value). QAA.
    sslOBJ_REF GetGlobalObj();

    // (?AddClass@sslSYSTEM@@QAA?AVsslCLASS_REF@@V2@@Z) — register a new script class derived from
    // `parent` (an empty ref for a root class); returns the new class by value (sret). Body not
    // reversed in this batch. // boundary
    sslCLASS_REF AddClass(sslCLASS_REF parent);
    // (?AddObject@sslSYSTEM@@QAA?AVsslOBJ_REF@@PBDVsslCLASS_REF@@PAVsslCB_HOST@@@Z) — instantiate a
    // new object named `name` of class `cls`, hosted by the native `pCbHost`; returns the new object
    // by value (sret). Body external to this batch. // boundary
    sslOBJ_REF AddObject(const char *name, sslCLASS_REF cls, sslCB_HOST *pCbHost);

    // 0x825CEB38 — find a class by interned name; returns an sslCLASS_REF by value (empty on miss). QAA.
    sslCLASS_REF FindClass(dsSTRID name);
    // 0x825CE508 — find a live object by uid; returns an sslOBJ_REF by value (empty on miss). Resets
    // lastErr to no-error first. QAA.
    sslOBJ_REF FindObject(int uid);
    // 0x825CE368 — find a live object by name (binary search of the name-sorted table); returns an
    // sslOBJ_REF by value (empty on miss, with lastErr set to an SSL_ERR_UNKNOWN_NAME error). QAA.
    sslOBJ_REF FindObject(const dsTSTRING<char> &name);
    // 0x825CECD8 — advance `curIdx` to the next live object whose class is (a subclass of) `cls`
    // (any class when `cls` is empty), skipping objects still under construction; returns that
    // object by value (empty ref when the table is exhausted). QAA.
    sslOBJ_REF FindNextObject(int &curIdx, sslCLASS_REF cls);

    // 0x825CDC30 (?RemoveObject@sslSYSTEM@@QAAXVsslOBJ_REF@@@Z) — remove `obj` from the system's live
    // object tables (takes the ref by value). boundary.
    void RemoveObject(sslOBJ_REF obj);

    // 0x825D2870 (?NameClass@sslSYSTEM@@QAA?AVsslERROR@@VsslCLASS_REF@@VdsSTRID@@@Z) — register
    // `cls` under the interned name `name`. boundary.
    sslERROR NameClass(sslCLASS_REF cls, dsSTRID name);

    // 0x825D0358 — queue a delayed call of function `funcIdx` on `obj` (invoked by `caller`) after
    // `timeOfs` seconds, capturing `argc` arguments from `stack` and tagging it with `group`; returns
    // an sslERROR by value (a runtime error when the object is null or does not support delayed
    // calls). QAA.
    sslERROR AddDelayedFunc(sslOBJ_REF obj, int funcIdx, sslOBJ_REF caller, float timeOfs, int argc,
                            sslSTACK &stack, dsSTRID group);
    // 0x825CE620 — apply `action` (pause/resume/kill/call-now) to every queued delayed call matching
    // `obj` (any object when empty) and `group` (any group when empty). QAA.
    void DelayedFuncAction(DELACT action, dsSTRID group, sslOBJ_REF obj);
    // 0x825CF2B8 — name registered for the class `c` refers to; returns a dsTSTRING<char> by value
    // (the shared empty string on miss). const (QBA).
    dsTSTRING<char> GetClassName(sslCLASS_REF c) const;

    // 0x825D1528 — assemble the concatenated source text of an object's class hierarchy and map
    // breakpoint lines; behaviour selected by `mode` (0 = full text + BP-line lists, 1 = locate
    // source by adjusted line, >=2 = adjusted line for a raw line). Returns an int line/-1. QAA.
    int BuildSource(int class_uid, int obj_uid, int line, int mode, sslSOURCE_REF *src,
                    dsVECTOR<int, 8> *arBPLines, dsVECTOR<int, 8> *arBP, dsTSTRING<char> *src_text);

    // 0x825CDEE0 — object-deleted callback: drop `uid` from the objectNames registry. QAA.
    void _OnObjectDelete(int uid);
    // 0x825CDF40 — class-deleted callback: drop `uid` from the classNames registry. QAA.
    void _OnClassDelete(int uid);
} sslSYSTEM;

// Global log-indentation depth shared by LogTab/LogUntab (DB symbol sslLogTabs).
extern int sslLogTabs;

// Global log sink: when set, receives each formatted log line as (0, text, 0). boundary — installed
// elsewhere; Log() no-ops when it is null. DB symbol sslLogCb.
extern void (*sslLogCb)(int channel, const char *text, int flags);
