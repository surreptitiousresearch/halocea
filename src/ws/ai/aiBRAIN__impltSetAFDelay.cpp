#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/ai/aiTASK_BASE.h" // mind.task pointee ops

// aiBRAIN::impltSetAFDelay @ 0x8325FDB0
// ?impltSetAFDelay@aiBRAIN@@UAAXM@Z
//
// SSL "tSetAFDelay" op: log the call, then set the task module's action-frame delay (seconds).
void aiBRAIN::impltSetAFDelay(float delayTime)
{
    _apLog("~AITask~%s: tSetAFDelay(%.2f)", names.nameInst, delayTime);
    mind.task.pointee->SetAFDelay(delayTime);
}
