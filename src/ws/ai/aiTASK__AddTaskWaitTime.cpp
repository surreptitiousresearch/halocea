// aiTASK::AddTaskWaitTime(float) @ 0x83263A58
// ?AddTaskWaitTime@aiTASK@@UAA_NM@Z
#include "../../headers/ws/ai/aiTASK.h"
#include "../../headers/ws/ai/aiTASK_DATA_WAITTIME.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiNAMES.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/ds_new.h"

extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

static const char *const AI_TASK_SYS_SRC = "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_task_sys.cpp";

// Virtual: append a "wait a fixed time" task. The WAITTIME ctor is inlined at the call site.
bool aiTASK::AddTaskWaitTime(float time)
{
    if (time < 0.000001f)
    {
        _apLog("~AITask,Error~%s: TWait(): wrong time to wait:%.3f!!!", this->pBrain->names.nameInst, time);
        return false;
    }

    aiTASK_DATA *task = new (AI_TASK_SYS_SRC, 0x3D6) aiTASK_DATA_WAITTIME(time);
    this->tasks.PushBack(task);
    return true;
}
