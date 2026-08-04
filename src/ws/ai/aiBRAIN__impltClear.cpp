#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/ai/aiTASK_BASE.h" // mind.task pointee ops

// aiBRAIN::impltClear @ 0x8325FBD8
// ?impltClear@aiBRAIN@@UAAXXZ
//
// SSL "tClear" op: log the call under this unit's instance name, then clear the task mind module.
void aiBRAIN::impltClear()
{
    _apLog("~AITask~%s: tClear()", names.nameInst);
    mind.task.pointee->Clear();
}
