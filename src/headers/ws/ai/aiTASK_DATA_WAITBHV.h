#pragma once
#include "aiTASK_DATA.h"
#include "../ds/dsTSTRING.h"

struct aiBRAIN; // ai/aiBRAIN.h — owning brain  boundary (fwd)

// ws-engine ai08 (a8_task_sys) — a "wait for a named behaviour to end" task descriptor. DB-verified
// layout (types_members aiTASK_DATA_WAITBHV): base aiTASK_DATA@0, waitBhvName@16, isDone@20 — size 24.
struct aiTASK_DATA_WAITBHV : aiTASK_DATA {
    const dsTSTRING<char> waitBhvName; // 0x10 behaviour name being waited on
    bool                  isDone;      // 0x14 set once the awaited behaviour ends

    // 0x8325FEF8 (??0aiTASK_DATA_WAITBHV@@QAA@ABV?$dsTSTRING@D@@@Z) — construct from a behaviour name.
    aiTASK_DATA_WAITBHV(const dsTSTRING<char> &bhvName);

    // 0x8325FF50 (?CheckDone@aiTASK_DATA_WAITBHV@@IAA_NABV?$dsTSTRING@D@@@Z) — mark done and return
    // true when `bhvName` matches the awaited behaviour.
    bool CheckDone(const dsTSTRING<char> &bhvName);

    // 0x832603C8 (?NoticeBhvEndSuccess@...) — virtual: on a behaviour's successful end. Always allows
    // the SSL event; marks done and returns true when the name matches.
    bool NoticeBhvEndSuccess(aiBRAIN *pBrain, unsigned int bhvSNum, const dsTSTRING<char> &bhvName,
                             bool &allowSSLEvent);
    // 0x83260428 (?NoticeBhvEndFail@...) — virtual: on a behaviour's failed end (same predicate).
    bool NoticeBhvEndFail(aiBRAIN *pBrain, unsigned int bhvSNum, const dsTSTRING<char> &bhvName,
                          bool &allowSSLEvent);
    // 0x83260488 (?NoticeBhvEndTerm@...) — virtual: on a behaviour's terminated end (same predicate).
    bool NoticeBhvEndTerm(aiBRAIN *pBrain, unsigned int bhvSNum, const dsTSTRING<char> &bhvName,
                          bool &allowSSLEvent);

    // 0x83261018 (?DbgPrintInfo@aiTASK_DATA_WAITBHV@@MAAXAAV?$dsTSTRING@D@@@Z) — protected virtual:
    // append "'<waitBhvName>'" to `out`. REVERSED: aiTASK_DATA_WAITBHV__DbgPrintInfo.cpp.
    void DbgPrintInfo(dsTSTRING<char> *out);
};
