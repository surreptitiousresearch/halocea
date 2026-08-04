#pragma once

// ws-engine ai08 (a8_task_sys) — base descriptor for a scripted task entry. DB-verified layout
// (types_members aiTASK_DATA): __vftable@0, afCount@4, afDelay@8, afForceEnabled@12 — size 16.

template<class T> struct dsTSTRING; // ds/dsTSTRING.h  boundary (fwd)
struct aiBRAIN;                     // ai/aiBRAIN.h  boundary (fwd)
struct aiTASK_BASE;                 // ai/aiTASK_BASE.h  boundary (fwd)
struct dsDATA;                      // ds/dsDATA.h  boundary (fwd)

// Virtual surface (DB aiTASK_DATA_vtbl). Bodies live in the per-subclass TUs; these decls let the
// aiTASK dispatch sites bind. Slots that are pure interface here are declared, not defined.
struct aiTASK_DATA {
    void  *__vftable;       // 0x00 aiTASK_DATA_vtbl*
    int    afCount;         // 0x04 after-fire repeat count
    float  afDelay;         // 0x08 after-fire delay (seconds)
    bool   afForceEnabled;  // 0x0C force-AF enabled flag

    // vtbl @0x00..0x34 — the shared task-descriptor virtual surface.
    void Destroy();                                                            // dtr_aiTASK_DATA
    bool Execute(aiBRAIN *pBrain, aiTASK_BASE *owner);                         // @0x04
    bool IsDone();                                                            // @0x08
    bool IsBlocker();                                                         // @0x0C
    void UpdateTimers(float dt);                                             // @0x10
    void NoticeStartSubBhv(unsigned int parentBhvSNum, unsigned int bhvSNum); // @0x14
    bool NoticeBhvEndSuccess(aiBRAIN *pBrain, unsigned int bhvSNum,
                             const dsTSTRING<char> *bhvName, bool *allowSSLEvent); // @0x18
    bool NoticeBhvEndFail(aiBRAIN *pBrain, unsigned int bhvSNum,
                          const dsTSTRING<char> *bhvName, bool *allowSSLEvent);    // @0x1C
    bool NoticeBhvEndTerm(aiBRAIN *pBrain, unsigned int bhvSNum,
                          const dsTSTRING<char> *bhvName, bool *allowSSLEvent);    // @0x20
    bool CanRestartBhv(unsigned int bhvSNum);                                // @0x24
    bool CanCreateBhv();                                                     // @0x28
    void DbgPrintType(dsTSTRING<char> *out);                                 // @0x2C
    void DbgPrintInfo(dsTSTRING<char> *out);                                 // @0x30
    void DbgPrintInfo2(dsTSTRING<char> *out);                               // @0x34

    // 0x832612B8 (?DbgBuildTaskInfo@aiTASK_DATA@@QAAXAAV?$dsTSTRING@D@@ABV2@@Z) — non-virtual:
    // append this task's debug line (with `runTag` prefix) to `out`.
    void DbgBuildTaskInfo(dsTSTRING<char> &out, const dsTSTRING<char> &runTag);
};
