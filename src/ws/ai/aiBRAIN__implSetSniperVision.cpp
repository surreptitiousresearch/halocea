#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ai/aiVISION_SNIPER.h"
#include "../../headers/ws/ai/aiVISION_BATTLE.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/ds_new.h"

// aiBRAIN::implSetSniperVision @ 0x83292938
// ?implSetSniperVision@aiBRAIN@@UAAX_N@Z
//
// Swap this brain's vision arm between the sniper (long-range) and battle variants. Refused when the
// descriptor disables the vision subsystem. The binary inlines the aiVISION_COMMON/SNIPER/BATTLE
// constructors here; the `new aiVISION_*` form below is the equivalent.
//
// NOTE: the decompiler reports "local variable allocation has failed" for this function; the log
// argument (`names.nameInst`, the '%s') was recovered from the sibling aiBRAIN logging convention.
void aiBRAIN::implSetSniperVision(bool enable)
{
    aiBRAIN_DESC *desc = reinterpret_cast<aiBRAIN_DESC *>(spDesc.pointee);

    if (desc->ssTypeVision == aiBRAIN_DESC::aiSUBSYS_NULL)
    {
        _apLog("~AI,Ssl,Error~'%s': can't set/clear sniper vision: vision system is disabled",
               names.nameInst);
        return;
    }

    if (enable)
    {
        mind.vision.Reset<aiVISION_SNIPER>(new aiVISION_SNIPER(this));
        _apLog("~AI,Ssl~'%s': vision system is set to SNIPER", names.nameInst);
    }
    else
    {
        mind.vision.Reset<aiVISION_BATTLE>(new aiVISION_BATTLE(this));
        _apLog("~AI,Ssl~'%s': vision system is set to BATTLE", names.nameInst);
    }
}
