// aiBRAIN::impltEnableForceAF @ 0x8325FE18
// ?impltEnableForceAF@aiBRAIN@@UAAX_N@Z
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiTASK_BASE.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog

// Virtual "tEnableForceAF" impl: log the transition and forward the enable flag to the task module.
void aiBRAIN::impltEnableForceAF(bool enable)
{
    _apLog("~AITask~%s: tEnableForceAF(%s)", names.nameInst, enable ? "enable" : "disable");
    mind.task.pointee->EnableForceAF(enable);
}
