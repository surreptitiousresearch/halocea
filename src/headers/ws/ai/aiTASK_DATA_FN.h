#pragma once
#include "aiTASK_DATA.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"

struct dsFUNC_PTR_BASE; // ds/dsFUNC_PTR_BASE.h — scripted function pointer  boundary (fwd)
struct aiBRAIN;         // ai08 — force-AF checker host  boundary (fwd)

// Force-AF result checker: a const aiBRAIN member predicate over a task's dsVECTOR<dsDATA,8> results.
// Occupies the __int128 forceAFfnChecker slot (16-byte PPC pointer-to-member-function).
typedef bool (aiBRAIN::*aiForceAFfnChecker)(const dsVECTOR<dsDATA, 8> &) const;

// ws-engine ai08 (a8_task_sys) — a "run scripted function" task descriptor. DB-verified layout
// (types_members aiTASK_DATA_FN): base aiTASK_DATA@0, objType@16, funcPtr@20, funcParams@24,
// objName@44, fnName@48, state@52, chanceToCreateBhv@56, checkFnResult@60, checkBhvResults@61,
// timerAF@64, afBhvList@68, forceAFfnChecker@88 — size 104.
struct aiTASK_DATA_FN : aiTASK_DATA {
    // Nested kinds (DB types_enum_values aiTASK_DATA_FN::*).
    enum aiTASK_DATA_OBJ_TYPE {
        AI_TASK_DATA_OBJ_GIVEN   = 0, // object resolved directly (sslOBJ_REF given)
        AI_TASK_DATA_OBJ_BY_NAME = 1, // object resolved by name at run time
    };
    enum aiTASK_DATA_FN_STATE {
        AI_TASK_DATA_FN_READY     = 0,
        AI_TASK_DATA_FN_WAIT_REPLY = 1,
        AI_TASK_DATA_FN_WAIT_AF   = 2,
        AI_TASK_DATA_FN_READY_AF  = 3,
        AI_TASK_DATA_FN_DONE      = 4,
    };

    aiTASK_DATA_OBJ_TYPE  objType;           // 0x10 how `objName`/funcPtr resolves the target object
    dsFUNC_PTR_BASE      *funcPtr;           // 0x14 resolved scripted function pointer
    dsVECTOR<dsDATA, 8>   funcParams;        // 0x18 call arguments
    dsTSTRING<char>       objName;           // 0x2C target object name
    dsSTRID               fnName;            // 0x30 scripted function name id
    aiTASK_DATA_FN_STATE  state;             // 0x34 run state
    int                   chanceToCreateBhv; // 0x38
    bool                  checkFnResult;     // 0x3C
    bool                  checkBhvResults;   // 0x3D
    float                 timerAF;           // 0x40 after-fire timer
    dsVECTOR<unsigned long, 8> afBhvList;    // 0x44 after-fire behaviour SNum list
    unsigned char         forceAFfnChecker[16]; // 0x58 __int128 opaque (force-AF fn checker delegate)

    // 0x8325FE98 (?CreateFnPtrByName@aiTASK_DATA_FN@@IAA_NXZ) — resolve `objName` through the SSL
    // system and build a dsFUNC_PTR_SSL for `fnName`; returns false only when the object is missing.
    bool CreateFnPtrByName();

    // 0x83260718 (?Init@aiTASK_DATA_FN@@UAAXPAVdsFUNC_PTR_BASE@@ABV?$dsTSTRING@D@@ABVdsSTRID@@@Z)
    // virtual: init with a pre-resolved function pointer + object name + fn-name id (OBJ_GIVEN).
    void Init(dsFUNC_PTR_BASE *funcPtr, const dsTSTRING<char> &objName, const dsSTRID &fnName);

    // 0x83260788 (?Init@aiTASK_DATA_FN@@UAAXABV?$dsTSTRING@D@@AAVdsSTRID@@@Z)
    // virtual: init to resolve the object by name at run time (OBJ_BY_NAME).
    void Init(const dsTSTRING<char> &objName, dsSTRID &fnName);

    // 0x832607F0 (?CanRestartBhv@aiTASK_DATA_FN@@UAA_NK@Z) — virtual: true when in WAIT_REPLY state and
    // `bhvSNum` is present in the after-fire behaviour list.
    bool CanRestartBhv(unsigned int bhvSNum);

    // 0x8325F810 (?InitAF@aiTASK_DATA_FN@@UAAX_N0P8aiBRAIN@@BA_NABV?$dsVECTOR@VdsDATA@@$07@@@Z@Z)
    // virtual: seed the after-fire result-check config — store the two check flags, clear
    // afForceEnabled when both are off, and store the force-AF checker delegate.
    // REVERSED: aiTASK_DATA_FN__InitAF.cpp.
    void InitAF(bool checkFnResult, bool checkBhvResults, aiForceAFfnChecker forceAFfnChecker);

    // vtbl @0x44 (?AddFnParam@aiTASK_DATA_FN@@UAAXABVdsDATA@@@Z) — virtual: append one scripted call
    // argument. boundary (body not in this batch).
    void AddFnParam(const dsDATA *arg);

    // 0x83260EC0 (?DbgTaskInfo@aiTASK_DATA_FN@@UAAXAAV?$dsTSTRING@D@@@Z) — virtual: print the task
    // type, a separating space, then the per-task info into `out`. REVERSED: aiTASK_DATA_FN__DbgTaskInfo.cpp.
    void DbgTaskInfo(dsTSTRING<char> *out);

    // 0x83260F58 (?DbgPrintInfo@aiTASK_DATA_FN@@MAAXAAV?$dsTSTRING@D@@@Z) — protected virtual: append
    // "$<objName>.<fnName>" to `out`. REVERSED: aiTASK_DATA_FN__DbgPrintInfo.cpp.
    void DbgPrintInfo(dsTSTRING<char> *out);

    // 0x83261430 (?DbgPrintInfo2@aiTASK_DATA_FN@@MAAXAAV?$dsTSTRING@D@@@Z) — protected virtual: append
    // the run-state name ("Ready"/"Wait Reply"/"Wait AF(%.1f)"/"Ready AF"/"Done") to `out`.
    // REVERSED: aiTASK_DATA_FN__DbgPrintInfo2.cpp.
    void DbgPrintInfo2(dsTSTRING<char> *out);

    // 0x83261970 (?HandleBhvSNum@aiTASK_DATA_FN@@IAA_NK@Z) — protected: consume `bhvSNum` from the
    // after-fire behaviour list; returns whether a firing behaviour was accounted. boundary.
    bool HandleBhvSNum(unsigned int bhvSNum);

    // 0x83260D20 (?AFRestart@aiTASK_DATA_FN@@IAAXPAVaiBRAIN@@@Z) — protected: re-fire the task's
    // after-fire pass through `pBrain`. boundary.
    void AFRestart(aiBRAIN *pBrain);

    // 0x83262A68 (?NoticeBhvEndSuccess@aiTASK_DATA_FN@@UAA_NPAVaiBRAIN@@KABV?$dsTSTRING@D@@AA_N@Z)
    // virtual: a behaviour finished successfully. REVERSED: aiTASK_DATA_FN__NoticeBhvEndSuccess.cpp.
    bool NoticeBhvEndSuccess(aiBRAIN *pBrain, unsigned int bhvSNum,
                             const dsTSTRING<char> &bhvName, bool &allowSSLEvent);

    // 0x83262AD0 (?NoticeBhvEndFail@aiTASK_DATA_FN@@UAA_NPAVaiBRAIN@@KABV?$dsTSTRING@D@@AA_N@Z)
    // virtual: a behaviour finished with failure. REVERSED: aiTASK_DATA_FN__NoticeBhvEndFail.cpp.
    bool NoticeBhvEndFail(aiBRAIN *pBrain, unsigned int bhvSNum,
                          const dsTSTRING<char> &bhvName, bool &allowSSLEvent);

    // 0x83263260 (?NoticeStartSubBhv@aiTASK_DATA_FN@@UAAXKK@Z) — virtual: a sub-behaviour started;
    // if `parentBhvSNum` is a firing behaviour, remember `bhvSNum` as one to reap on end.
    // REVERSED: aiTASK_DATA_FN__NoticeStartSubBhv.cpp.
    void NoticeStartSubBhv(unsigned int parentBhvSNum, unsigned int bhvSNum);

    // 0x832635F8 (??0aiTASK_DATA_FN@@QAA@HM_N@Z) — construct with the owning aiTASK's default
    // after-fire count/delay/force flag.  boundary.
    aiTASK_DATA_FN(int afCount, float afDelay, bool afForceEnabled);

    // 0x832632C8 (?CallFnPtr@aiTASK_DATA_FN@@IAA_NPAVaiBRAIN@@PAVaiTASK_BASE@@@Z) — protected:
    // invoke the resolved scripted function, applying the force-AF/result-check state machine.
    // REVERSED: aiTASK_DATA_FN__CallFnPtr.cpp.
    bool CallFnPtr(aiBRAIN *pBrain, aiTASK_BASE *taskSys);

    // 0x83263CE0 (?Execute@aiTASK_DATA_FN@@UAA_NPAVaiBRAIN@@PAVaiTASK_BASE@@@Z) — virtual: per-frame
    // task step; lazily resolves the function pointer, then dispatches on `state`.
    // REVERSED: aiTASK_DATA_FN__Execute.cpp.
    bool Execute(aiBRAIN *pBrain, aiTASK_BASE *taskSys);
};
