// aiTASK::DbgBuildTaskList(dsTSTRING<char>&) @ 0x83262120
// ?DbgBuildTaskList@aiTASK@@UAAXAAV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiTASK.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// dsSPrintf: ds string formatter — writes into `out` and returns it.  boundary (ds free function).
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// Virtual: build a human-readable multi-line dump of the task queue into `str`. Each task line is
// prefixed by its module-status tag ("[LOCKED nn]" / "[WAIT INIT]" / "==DEAD AI==" if any apply,
// otherwise "  [RUN]  "), and the run tag flips to "[BLOCKED]" after the first blocking task.
void aiTASK::DbgBuildTaskList(dsTSTRING<char> &str)
{
    if (!tasks.nElem)
    {
        str.Insert(str.pBuffer->strLen, "no tasks", -1);
        return;
    }

    // Module-status prefix, in ascending priority (later wins).
    dsTSTRING<char> statusTag("");
    if (nLocks > 0)
    {
        dsTSTRING<char> tmp;
        statusTag = *dsSPrintf(&tmp, "[LOCKED %2d]", nLocks);
    }
    if ((pBrain->st.pointee->gen.val >> 5) & 1)   // AI_GEST bit 5 — awaiting init
        statusTag = dsTSTRING<char>("[WAIT INIT]");
    if (pBrain->IsDead())
        statusTag = dsTSTRING<char>("==DEAD AI==");

    dsTSTRING<char> runTag("  [RUN]  ");
    bool haveStatusTag = statusTag.pBuffer->strLen != 0;

    int nElem = tasks.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiTASK_DATA *task = tasks[i];
        const dsTSTRING<char> &tag = haveStatusTag ? statusTag : runTag;
        task->DbgBuildTaskInfo(str, tag);
        if (i < nElem - 1)
            str.Insert(str.pBuffer->strLen, "\n", -1);
        if (task->IsBlocker())
            runTag = dsTSTRING<char>("[BLOCKED]");
    }
}
