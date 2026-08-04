// aiBRAIN::implSetForceSpeed @ 0x832AF3F8
// ?implSetForceSpeed@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h" // _apLog

// Ask the goal module to apply the pending forced gait; log an SSL error (naming the requested
// speed) and return false when it could not.
bool aiBRAIN::implSetForceSpeed(const dsTSTRING<char> &speedName)
{
    if (mind.goal.pointee->SetForceSpeed())
        return true;
    _apLog("~AI,Ssl,Error~'%s': SetForceSpeed(): can't set speed '%s'",
           names.nameInst, speedName.CStr());
    return false;
}
