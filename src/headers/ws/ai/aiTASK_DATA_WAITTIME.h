#pragma once
#include "aiTASK_DATA.h"

// ws-engine ai08 (a8_task_sys) — a "wait a fixed time" task descriptor. DB-verified layout
// (types_members aiTASK_DATA_WAITTIME): base aiTASK_DATA@0, timer@16 — size 20.
struct aiTASK_DATA_WAITTIME : aiTASK_DATA {
    float timer; // 0x10 remaining wait time (seconds)

    // Inlined at aiTASK::AddTaskWaitTime (0x83263A58): zero the after-fire base fields and seed
    // `timer` with the wait duration.
    aiTASK_DATA_WAITTIME(float time);

    // 0x83261100 (?DbgPrintInfo@aiTASK_DATA_WAITTIME@@MAAXAAV?$dsTSTRING@D@@@Z) — protected virtual:
    // append "%.1f" of `timer` to `out`. REVERSED: aiTASK_DATA_WAITTIME__DbgPrintInfo.cpp.
    void DbgPrintInfo(dsTSTRING<char> *out);
};
