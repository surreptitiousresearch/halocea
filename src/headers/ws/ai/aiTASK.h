#pragma once
#include "aiTASK_BASE.h"
#include "aiTASK_DATA.h"
#include "aiTASK_DATA_FN.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"

struct aiBRAIN; // ai/aiBRAIN.h — owning brain  boundary (fwd)

// ws-engine ai08 (a8_task_sys) — the SSL-facing task interface layer over aiTASK_BASE. DB-verified
// layout (types_members aiTASK_IFACE): base aiTASK_BASE@0, pBrain@28 — size 32.
struct aiTASK_IFACE : aiTASK_BASE {
    aiBRAIN *pBrain; // 0x1C owning brain
};

// ws-engine ai08 (a8_task_sys) — a brain's concrete task mind module: an ordered queue of
// aiTASK_DATA entries built by the SSL "t*" ops. DB-verified layout (types_members aiTASK):
// base aiTASK_IFACE@0, afCount@32, afDelay@36, afForceEnabled@40, tasks@44, nLocks@64,
// autoBhvCounter@68, tasksDelQueue@72 — size 92.
struct aiTASK : aiTASK_IFACE {
    int                          afCount;         // 0x20 default after-fire count applied to new task-fns
    float                        afDelay;         // 0x24 default after-fire delay
    bool                         afForceEnabled;  // 0x28 default force-AF flag
    dsVECTOR<aiTASK_DATA *, 8>   tasks;           // 0x2C live task queue
    int                          nLocks;          // 0x40 lock count (task processing suspended while > 0)
    int                          autoBhvCounter;  // 0x44 counter for auto-generated behaviour names
    dsVECTOR<aiTASK_DATA *, 8>   tasksDelQueue;   // 0x48 tasks pending destruction

    // 0x832601A8 (??0aiTASK@@QAA@PAVaiBRAIN@@@Z) — construct a brain's task mind module.
    aiTASK(aiBRAIN *brain);

    // 0x83263600 (?AddTaskFn@aiTASK@@UAA_NW4aiTASK_FN_TYPE@aiTASK_BASE@@W4aiTASK_OBJ_TYPE@3@HPAVdsDATA@@@Z)
    // virtual: build a scripted-function task (fn/obj kind, argc/argv), parse its name+params, append.
    bool AddTaskFn(aiTASK_FN_TYPE taskType, aiTASK_OBJ_TYPE objType, int argc, dsDATA *argv);

    // 0x832639B0 (?AddTaskWaitBHV@aiTASK@@UAA_NABV?$dsTSTRING@D@@@Z) — virtual: append a wait-for-behaviour task.
    bool AddTaskWaitBHV(const dsTSTRING<char> &bhvName);

    // 0x83263A58 (?AddTaskWaitTime@aiTASK@@UAA_NM@Z) — virtual: append a wait-for-time task.
    bool AddTaskWaitTime(float time);

    // 0x83263B20 (?AddTaskWaitFlag@aiTASK@@UAA_NABV?$dsTSTRING@D@@@Z) — virtual: append a wait-for-flag task.
    bool AddTaskWaitFlag(const dsTSTRING<char> &flagName);

    // 0x83263BC8 (?Clear@aiTASK@@UAAXXZ) — virtual: move all live tasks to the delete queue and empty the queue.
    void Clear();

    // 0x832619F0 (?UpdateTimers@aiTASK@@UAAXM@Z) — virtual: advance every live task's timers by `time`,
    // stopping at the first still-active blocking task. REVERSED: aiTASK__UpdateTimers.cpp.
    void UpdateTimers(float time);

    // 0x83261B00 (?IsNeedFillEmptyBhvStack@aiTASK@@UBA_NXZ) — virtual const: true when the empty
    // behaviour-stack should be refilled (no tasks / dead / locked / no runnable non-blocking task).
    // REVERSED: aiTASK__IsNeedFillEmptyBhvStack.cpp.
    bool IsNeedFillEmptyBhvStack() const;

    // 0x83261BF0 (?NoticeStartSubBhv@aiTASK@@UAAXKK@Z) — virtual: forward a sub-behaviour start to
    // every live task.  REVERSED: aiTASK__NoticeStartSubBhv.cpp.
    void NoticeStartSubBhv(unsigned int parentBhvSNum, unsigned int bhvSNum);

    // 0x83261CE0 / 0x83261E00 / 0x83261F20 — virtual: forward a behaviour end to live tasks; stops at
    // the first task that consumes it.  REVERSED: aiTASK__NoticeBhvEnd{Success,Fail,Term}.cpp.
    bool NoticeBhvEndSuccess(unsigned int bhvSNum, const dsTSTRING<char> &bhvName);
    bool NoticeBhvEndFail(unsigned int bhvSNum, const dsTSTRING<char> &bhvName);
    bool NoticeBhvEndTerm(unsigned int bhvSNum, const dsTSTRING<char> &bhvName);

    // 0x83262040 (?CanRestartBhv@aiTASK@@UAA_NK@Z) — virtual: true when some live task can restart
    // behaviour `bhvSNum` (module not dead/locked/awaiting init).  REVERSED: aiTASK__CanRestartBhv.cpp.
    bool CanRestartBhv(unsigned int bhvSNum);

    // 0x83262120 (?DbgBuildTaskList@aiTASK@@UAAXAAV?$dsTSTRING@D@@@Z) — virtual: build the debug
    // task-list string into `str`.  REVERSED: aiTASK__DbgBuildTaskList.cpp.
    void DbgBuildTaskList(dsTSTRING<char> &str);

    // 0x83262B60 (?ProcessFrame@aiTASK@@UAAXXZ) — virtual: run the task list this frame, reaping
    // done tasks and flushing the deferred-destruction queue.  REVERSED: aiTASK__ProcessFrame.cpp.
    void ProcessFrame();

    // 0x83262D60 (?ParseTaskFnName@aiTASK@@IAA_NW4aiTASK_OBJ_TYPE@aiTASK_BASE@@HPAVaiTASK_DATA_FN@@AAV?$dsTSTRING@D@@AAV?$dsVECTOR@VdsDATA@@$07@@@Z)
    // protected: resolve the SSL task-fn target object (self/obj/name), init `task`, then hand off to
    // ParseTaskFnSpec.  REVERSED: aiTASK__ParseTaskFnName.cpp.
    bool ParseTaskFnName(aiTASK_OBJ_TYPE objType, int paramsOffset, aiTASK_DATA_FN *task,
                         dsTSTRING<char> &bhvName, dsVECTOR<dsDATA, 8> &sslParams);

    // 0x832623C0 (?ParseTaskFnSpec@aiTASK@@IAAXHPAVaiTASK_DATA_FN@@AAVdsSTRID@@AAV?$dsTSTRING@D@@AAV?$dsVECTOR@VdsDATA@@$07@@@Z)
    // protected: apply the descriptor row for `fnName` (after-fire flags + behaviour-name param) to
    // `task`.  REVERSED: aiTASK__ParseTaskFnSpec.cpp.
    void ParseTaskFnSpec(int paramsOffset, aiTASK_DATA_FN *task, dsSTRID &fnName,
                         dsTSTRING<char> &bhvName, dsVECTOR<dsDATA, 8> &sslParams);

    // 0x832627A0 (?ParseTaskFnParams@aiTASK@@IAA_NHPAVaiTASK_DATA_FN@@AAV?$dsVECTOR@VdsDATA@@$07@@@Z)
    // protected: push the trailing non-null scripted params (from `paramsOffset`) onto `task`.
    // REVERSED: aiTASK__ParseTaskFnParams.cpp.
    bool ParseTaskFnParams(int paramsOffset, aiTASK_DATA_FN *task, dsVECTOR<dsDATA, 8> &sslParams);
};
