// aiTASK::AddTaskWaitFlag(const dsTSTRING<char>&) @ 0x83263B20
// ?AddTaskWaitFlag@aiTASK@@UAA_NABV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiTASK.h"
#include "../../headers/ws/ai/aiTASK_DATA_WAITFLAG.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiNAMES.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/ds_new.h"

extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

static const char *const AI_TASK_SYS_SRC = "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_task_sys.cpp";

// Virtual: append a "wait for a named flag" task.
bool aiTASK::AddTaskWaitFlag(const dsTSTRING<char> &flagName)
{
    if (!flagName.pBuffer->strLen)
    {
        _apLog("~AITask,Error~%s: TWait(): flag name not specified!!!", this->pBrain->names.nameInst);
        return false;
    }

    aiTASK_DATA *task = new (AI_TASK_SYS_SRC, 0x3E9) aiTASK_DATA_WAITFLAG(flagName);
    this->tasks.PushBack(task);
    return true;
}
