// aiTASK::AddTaskFn(aiTASK_FN_TYPE, aiTASK_OBJ_TYPE, int, dsDATA*) @ 0x83263600
// ?AddTaskFn@aiTASK@@UAA_NW4aiTASK_FN_TYPE@aiTASK_BASE@@W4aiTASK_OBJ_TYPE@3@HPAVdsDATA@@@Z
#include "../../headers/ws/ai/aiTASK.h"
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiNAMES.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsDATA_TYPE.h"
#include "../../headers/ws/ds/ds_boundary.h" // dsDATA_TYPE_STORAGE<sslOBJ_REF>::dataType
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/ds_new.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

extern void _apLog(const char *fmt, ...);                          // ?_apLog@@YAXPBDZZ  boundary
extern const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

static const char *const AI_TASK_SYS_SRC = "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_task_sys.cpp";

// Virtual: assemble a scripted-function task from (argc/argv), resolve its target object + name +
// params, append it, and (for AI_TASK_FN_WAIT) auto-add a matching wait-behaviour task.
bool aiTASK::AddTaskFn(aiTASK_FN_TYPE taskType, aiTASK_OBJ_TYPE objType, int argc, dsDATA *argv)
{
    if (argc < 1)
        return false;

    // Copy the scripted arguments into a working vector (its apCL debug tag is set to this call site).
    dsVECTOR<dsDATA, 8> sslParams;
    for (int i = 0; i < argc; ++i)
        sslParams.PushBack(argv[i]);

    aiTASK_DATA_FN *task = new (AI_TASK_SYS_SRC, 0x387)
        aiTASK_DATA_FN(this->afCount, this->afDelay, this->afForceEnabled);

    // Resolve how the first argument identifies the target object; `paramsOffset` skips leading args.
    int paramsOffset = 2;
    if (objType != AI_TASK_OBJ_NAME)
    {
        if (argv->type == dsDATA_TYPE_STORAGE<sslOBJ_REF>::dataType)
        {
            objType = AI_TASK_OBJ_OBJ;
        }
        else
        {
            objType = AI_TASK_OBJ_SELF;
            paramsOffset = 1;
        }
    }

    dsTSTRING<char> bhvName;
    dsSPrintf(&bhvName, "TaskSysBhv_%d", this->autoBhvCounter);

    // Deviation: the decompiler's manual buffer-refcount teardown is the dsTSTRING/dsVECTOR
    // destructors — here they run automatically at scope exit, so failure paths just return.
    if (!this->ParseTaskFnName(objType, paramsOffset, task, bhvName, sslParams))
    {
        if (task) task->Destroy(); // deleting dtr (vtbl flag 1)
        return false;
    }
    if (!this->ParseTaskFnParams(paramsOffset, task, sslParams))
    {
        if (task) task->Destroy();
        return false;
    }

    this->tasks.PushBack(task);

    dsTSTRING<char> taskInfo;
    task->DbgTaskInfo(&taskInfo);
    _apLog("~AITask~%s: task added: %s", this->pBrain->names.nameInst, taskInfo.pBuffer->str);

    if (taskType == AI_TASK_FN_WAIT && bhvName.pBuffer->strLen)
    {
        if (!this->AddTaskWaitBHV(bhvName))
            return false;
        _apLog("~AITask~%s: taskWaitBHV(%s) added automatically",
               this->pBrain->names.nameInst, bhvName.pBuffer->str);
        ++this->autoBhvCounter;
    }
    return true;
}
