// aiTASK::ParseTaskFnName(aiTASK_OBJ_TYPE, int, aiTASK_DATA_FN*, dsTSTRING<char>&, dsVECTOR<dsDATA,8>&) @ 0x83262D60
// ?ParseTaskFnName@aiTASK@@IAA_NW4aiTASK_OBJ_TYPE@aiTASK_BASE@@HPAVaiTASK_DATA_FN@@AAV?$dsTSTRING@D@@AAV?$dsVECTOR@VdsDATA@@$07@@@Z
#include "../../headers/ws/ai/aiTASK.h"
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiNAMES.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsDATA_TYPE.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h" // DSD_CONV_RETRIEVE
#include "../../headers/ws/ds/dsFUNC_PTR_SSL.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/ds_new.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

extern void _apLog(const char *fmt, ...);                                            // boundary
extern const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

static const char *const AI_TASK_SYS_SRC =
    "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_task_sys.cpp";

// Protected: resolve the SSL task-fn call. Argument [paramsOffset-1] carries the fn name; the target
// object is resolved per `objType` (SELF = the brain itself, OBJ = arg0 as an object ref, NAME = arg0
// as a string). Builds and installs the appropriate function pointer on `task`, then applies the
// descriptor row via ParseTaskFnSpec. Returns false on an invalid object / non-string name argument.
bool aiTASK::ParseTaskFnName(aiTASK_OBJ_TYPE objType, int paramsOffset, aiTASK_DATA_FN *task,
                             dsTSTRING<char> &bhvName, dsVECTOR<dsDATA, 8> &sslParams)
{
    if (sslParams.nElem < paramsOffset)
        return false;

    // The function name lives in the last "object-selection" argument.
    dsDATA &fnParam = sslParams[paramsOffset - 1];
    const dsTSTRING<char> *fnStr = fnParam.type
        ? static_cast<const dsTSTRING<char> *>(fnParam.type->GetPtr(&fnParam.storage))
        : nullptr;
    // Deviation: the decompiler dereferences the raw GetPtr result unconditionally; guard the null
    // (empty-param) case with an empty id rather than reproducing the UB.
    dsSTRID fnName = fnStr ? dsSTRID(*fnStr, false) : dsSTRID();

    if (objType == AI_TASK_OBJ_SELF)
    {
        sslOBJ_REF selfRef(pBrain->sslObject);
        if (!selfRef.pObject)
            return false;                       // brain has no script object

        dsTSTRING<char> objName = selfRef.GetName();
        dsFUNC_PTR_SSL *fp = new (AI_TASK_SYS_SRC, 0x4E1) dsFUNC_PTR_SSL(selfRef.operator dsDATA(), fnName);
        task->Init(fp, objName, fnName);
    }
    else if (objType == AI_TASK_OBJ_OBJ)
    {
        sslOBJ_REF objRef;
        if (!sslParams[0].GetValue<sslOBJ_REF>(objRef, DSD_CONV_RETRIEVE) || !objRef.pObject)
        {
            _apLog("~AITask,Error~%s: tDo()/tDoWait(): invalid object!!!", pBrain->names.nameInst);
            return false;
        }

        dsTSTRING<char> objName = objRef.GetName();
        dsDATA objData;
        objData.type = nullptr;
        objData.StoreValue(sslParams[0]);       // box the object argument
        dsFUNC_PTR_SSL *fp = new (AI_TASK_SYS_SRC, 0x4EC) dsFUNC_PTR_SSL(objData, fnName);
        task->Init(fp, objName, fnName);
    }
    else if (objType >= 3)
    {
        // Already-initialised task kinds: fall straight through to the descriptor pass.
    }
    else // AI_TASK_OBJ_NAME
    {
        dsTSTRING<char> name("");
        if (!sslParams[0].GetValue<dsTSTRING<char>>(name, DSD_CONV_RETRIEVE))
        {
            _apLog("~AITask,Error~%s: tNDo()/tNDoWait(): first parameter must be string (name of object)!!!",
                   pBrain->names.nameInst);
            return false;
        }
        task->Init(name, fnName);               // resolve the object by name at run time
    }

    ParseTaskFnSpec(paramsOffset, task, fnName, bhvName, sslParams);
    return true;
}
