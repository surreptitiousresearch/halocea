#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/ai/aiTASK_BASE.h" // mind.task pointee ops

// aiBRAIN::impltWaitBHV @ 0x8325FC28
// ?impltWaitBHV@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z
//
// SSL "tWaitBHV" op: log the call (unit name + behaviour name), then append a wait-for-behaviour
// task to the task mind module.
void aiBRAIN::impltWaitBHV(const dsTSTRING<char> &bhv)
{
    _apLog("~AITask~%s: tWaitBHV(%s)", names.nameInst, bhv.pBuffer->str);
    mind.task.pointee->AddTaskWaitBHV(bhv);
}
