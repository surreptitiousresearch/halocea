#pragma once
#include "aiTASK_DATA.h"
#include "../ds/dsSTRID.h"

// ws-engine ai08 (a8_task_sys) — a "wait for a named flag" task descriptor. DB-verified layout
// (types_members aiTASK_DATA_WAITFLAG): base aiTASK_DATA@0, flag@16 — size 20.
struct aiTASK_DATA_WAITFLAG : aiTASK_DATA {
    const dsSTRID flag; // 0x10 flag being waited on

    // 0x83263B88 (?aiTASK_DATA_WAITFLAG@@QAA@ABV?$dsTSTRING@D@@@Z) — construct from a flag name.
    aiTASK_DATA_WAITFLAG(const dsTSTRING<char> &flagName);

    // 0x832609C0 (?IsDone@aiTASK_DATA_WAITFLAG@@UAA_NXZ) — virtual: done once the planner's active
    // task-system flag set contains `flag`. REVERSED: aiTASK_DATA_WAITFLAG__IsDone.cpp.
    bool IsDone();

    // 0x832611D8 (?DbgPrintInfo@aiTASK_DATA_WAITFLAG@@MAAXAAV?$dsTSTRING@D@@@Z) — protected virtual:
    // append "'<flag>'" to `out`. REVERSED: aiTASK_DATA_WAITFLAG__DbgPrintInfo.cpp.
    void DbgPrintInfo(dsTSTRING<char> *out);
};
