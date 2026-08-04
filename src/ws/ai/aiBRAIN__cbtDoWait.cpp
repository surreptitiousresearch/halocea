// aiBRAIN::cbtDoWait @ 0x832600C8
// ?cbtDoWait@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiTASK_BASE.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// SSL "tDoWait" callback: add a WAIT task-fn on the given object; box the bool result into retVal.
// `caller` self-releases on return.
void aiBRAIN::cbtDoWait(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    bool res = mind.task.pointee->AddTaskFn(aiTASK_BASE::AI_TASK_FN_WAIT,
                                            aiTASK_BASE::AI_TASK_OBJ_OBJ, argc, argv);
    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(res);
    retVal.StoreValue(out);
}
