#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/ai/aiTASK_BASE.h" // mind.task pointee ops

// aiBRAIN::impltLock @ 0x8325FC90
// ?impltLock@aiBRAIN@@UAAXH@Z
//
// SSL "tLock" op: log the call, then lock the task mind module `count` times.
void aiBRAIN::impltLock(int count)
{
    _apLog("~AITask~%s: tLock(%d)", names.nameInst, count);
    mind.task.pointee->Lock(count);
}
