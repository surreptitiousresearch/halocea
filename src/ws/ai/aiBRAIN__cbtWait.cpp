// aiBRAIN::cbtWait @ 0x83260A10
// ?cbtWait@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiTASK_BASE.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/ds_boundary.h" // dsDATA_TYPE_STORAGE<T>::dataType

extern void _apLog(const char *fmt, ...); // ws/ap — engine log  boundary

// SSL "tWait" callback: the single argument is either a flag name (string → wait-for-flag task) or a
// duration (float → wait-for-time task); anything else logs an error. `caller` self-releases on return.
void aiBRAIN::cbtWait(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    if (argv->type == dsDATA_TYPE_STORAGE<dsTSTRING<char> >::dataType) {
        dsTSTRING<char> flagName;
        if (argv->GetValue<dsTSTRING<char> >(flagName, DSD_CONV_RETRIEVE)) {
            _apLog("~AITask~%s: tWait(%s)", names.nameInst, flagName.CStr());
            mind.task.pointee->AddTaskWaitFlag(flagName);
        }
        return;
    }

    float waitTime;
    if (!argv->GetValue<float>(waitTime, DSD_CONV_RETRIEVE)) {
        _apLog("~AITask,Error~%s: tWait(): parameter must be flag name (string) or time (float)!!!",
               names.nameInst);
        return;
    }
    _apLog("~AITask~%s: tWait(%.2f)", names.nameInst, waitTime);
    mind.task.pointee->AddTaskWaitTime(waitTime);
}
