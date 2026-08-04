#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/ai/aiTASK_BASE.h" // mind.task pointee ops

// aiBRAIN::impltUnlock @ 0x8325FCF0
// ?impltUnlock@aiBRAIN@@UAAXH@Z
//
// SSL "tUnlock" op: log the call, then unlock the task mind module `count` times.
void aiBRAIN::impltUnlock(int count)
{
    _apLog("~AITask~%s: tUnlock(%d)", names.nameInst, count);
    mind.task.pointee->Unlock(count);
}
