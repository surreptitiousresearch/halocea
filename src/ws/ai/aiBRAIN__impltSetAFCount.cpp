#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/ai/aiTASK_BASE.h" // mind.task pointee ops

// aiBRAIN::impltSetAFCount @ 0x8325FD50
// ?impltSetAFCount@aiBRAIN@@UAAXH@Z
//
// SSL "tSetAFCount" op: log the call, then set the task module's action-frame count.
void aiBRAIN::impltSetAFCount(int count)
{
    _apLog("~AITask~%s: tSetAFCount(%d)", names.nameInst, count);
    mind.task.pointee->SetAFCount(count);
}
