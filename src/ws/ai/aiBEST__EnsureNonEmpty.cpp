#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiTASK_BASE.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// _apLog @ engine logger. boundary.
extern void _apLog(const char *fmt, ...);

// aiBEST::EnsureNonEmpty @ 0x83298F80
// ?EnsureNonEmpty@aiBEST@@AAAXXZ (private)
//
// If the changeable stack is empty and the task module wants an empty stack defaulted, log a
// diagnostic and (timed) run the default "add on empty" behaviour hook, committing the change.
void aiBEST::EnsureNonEmpty()
{
    if (!IsEmpty())
        return;
    if (!pBrain->mind.task.pointee->IsNeedFillEmptyBhvStack())
        return;

    _apLog("~AI,Error~%s: no behavior to do. Defaulting.", pBrain->names.nameInst);

    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/init/SSL_OnEmpty", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> scopeTimer(&localTimeCounter);

    dbgLine = dsTSTRING<char>("add on empty is in process.");
    AddBehaviorOnEmptyStack();
    CommitChanges();
}
