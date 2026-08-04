// aiTASK::AddTaskWaitBHV(const dsTSTRING<char>&) @ 0x832639B0
// ?AddTaskWaitBHV@aiTASK@@UAA_NABV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiTASK.h"
#include "../../headers/ws/ai/aiTASK_DATA_WAITBHV.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiNAMES.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/ds_new.h"

extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

static const char *const AI_TASK_SYS_SRC = "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_task_sys.cpp";

// Virtual: append a "wait for a named behaviour to end" task.
bool aiTASK::AddTaskWaitBHV(const dsTSTRING<char> &bhvName)
{
    if (!bhvName.pBuffer->strLen)
    {
        _apLog("~AITask,Error~%s: TWaitBHV(): bhv name not specified!!!", this->pBrain->names.nameInst);
        return false;
    }

    aiTASK_DATA *task = new (AI_TASK_SYS_SRC, 0x3C3) aiTASK_DATA_WAITBHV(bhvName);
    this->tasks.PushBack(task);
    return true;
}
