#pragma once
#include "../ds/dsVECTOR.h"

// ws-engine ai08 — base of a brain's task mind module (aiMIND::task pointee). DB-verified layout
// (types_members aiTASK_BASE): __vftable@0, startedBhvList@4, bhvNotNeed@24 — size 28. The full
// virtual surface below is only partially reached; remaining bodies are the next frontier. boundary.

struct dsDATA; // ds/dsDATA.h — task-fn arguments  boundary (fwd)
template<class T> struct dsTSTRING; // ds/dsTSTRING.h — behaviour name  boundary (fwd)

struct aiTASK_BASE {
    void *__vftable;                          // 0x00 aiTASK_BASE_vtbl*
    dsVECTOR<unsigned long, 8> startedBhvList; // 0x04 behaviours started during the current task-fn call
    bool  bhvNotNeed;                         // 0x18 set when a started behaviour is not needed

    // Nested kind enums (DB types_enum_values aiTASK_BASE::aiTASK_FN_TYPE / aiTASK_OBJ_TYPE).
    enum aiTASK_FN_TYPE {
        AI_TASK_FN_SIMPLE = 0,
        AI_TASK_FN_WAIT   = 1,
    };
    enum aiTASK_OBJ_TYPE {
        AI_TASK_OBJ_SELF = 0,
        AI_TASK_OBJ_OBJ  = 1,
        AI_TASK_OBJ_NAME = 2,
    };

    // ?UpdateTimers@aiTASK_BASE@@UAAXM@Z — virtual: advance this module's timers by `dt` seconds.
    // Used by aiMIND::UpdateTimers.  boundary.
    void UpdateTimers(float dt);

    // vtbl — per-frame task update. Used by aiMIND::ProcessFrameAfterBody.  boundary.
    void ProcessFrame();

    // 0x83177F38 (?IsNeedFillEmptyBhvStack@aiTASK_BASE@@UBA_NXZ) — virtual const: true when an empty
    // behaviour stack should be defaulted/filled. Used by aiBEST::EnsureNonEmpty.  boundary.
    bool IsNeedFillEmptyBhvStack() const;

    // 0x83177EE8 (?AddTaskFn@aiTASK_BASE@@UAA_NW4aiTASK_FN_TYPE@1@W4aiTASK_OBJ_TYPE@1@HPAVdsDATA@@@Z)
    // virtual: append a scripted task-fn of the given fn/obj kind, built from `argc`/`argv`; returns
    // success. Used by aiBRAIN::cbtDo/cbtDoWait/cbtNDo/cbtNDoWait.  boundary.
    bool AddTaskFn(aiTASK_FN_TYPE fnType, aiTASK_OBJ_TYPE objType, int argc, dsDATA *argv);

    // 0x83177F20 (?EnableForceAF@aiTASK_BASE@@UAAX_N@Z) — virtual: enable/disable force-AF on the task
    // module. Used by aiBRAIN::impltEnableForceAF.  boundary.
    void EnableForceAF(bool enable);

    // vtbl SSL "t*" task-op surface driven by the aiBRAIN::implt* callbacks (0x8325FBD8-0x8325FDB0).
    // Bodies are the next frontier.  boundary.
    void Clear();                                    // "tClear"
    void AddTaskWaitBHV(const dsTSTRING<char> &bhv); // "tWaitBHV"
    // vtbl — append a wait-for-flag task ("tWait" with a flag-name string). Used by aiBRAIN::cbtWait.
    // boundary (concrete body in aiTASK::AddTaskWaitFlag).
    void AddTaskWaitFlag(const dsTSTRING<char> &flagName);
    // vtbl — append a wait-for-time task ("tWait" with a float). Used by aiBRAIN::cbtWait.
    // boundary (concrete body in aiTASK::AddTaskWaitTime).
    void AddTaskWaitTime(float time);
    void Lock(int count);                            // "tLock"
    void Unlock(int count);                          // "tUnlock"
    void SetAFCount(int count);                      // "tSetAFCount"
    void SetAFDelay(float delayTime);                // "tSetAFDelay"

    // ?NoticeBhvEndSuccess@aiTASK_BASE@@UAA_NKABV?$dsTSTRING@D@@@Z — virtual: notify the task module
    // that behaviour `bhvName` (with scripted-task selector `bhvSNum`) completed successfully.
    bool NoticeBhvEndSuccess(unsigned long bhvSNum, const dsTSTRING<char> &bhvName);
    // ?NoticeBhvEndFail@aiTASK_BASE@@UAA_NKABV?$dsTSTRING@D@@@Z — virtual: notify the task module that
    // behaviour `bhvName` (with scripted-task selector `bhvSNum`) ended in failure.  boundary.
    bool NoticeBhvEndFail(unsigned long bhvSNum, const dsTSTRING<char> &bhvName);
};
