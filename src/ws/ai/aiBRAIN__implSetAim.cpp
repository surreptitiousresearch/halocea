#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiNAMES.h"
#include "../../headers/ws/ai/aiAIM_BASE.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/ent/entTRACKER.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::implSetAim @ 0x8329A130
// ?implSetAim@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z
//
// Set a scripted aim onto the tracker named `nameTrk`. Resolve the tracker; on success install it
// into the aim module, clear the tracked enemy/selene, reset sparsings and record the given name.
// On a missing tracker, log an error.
void aiBRAIN::implSetAim(const dsTSTRING<char> &nameTrk)
{
    entTRACKER tracker;
    if (tracker.Init(nameTrk))
    {
        mind.aim.pointee->SetTrk(&tracker);
        mind.ZeroEnemy();
        mind.ZeroSelene();
        ResetAllSparsings();
        st.pointee->aim.nameGiven = nameTrk;
    }
    else
    {
        _apLog("~AI,Ssl,Error~'%s': trk '%s' is not found ", names.nameInst, nameTrk.CStr());
    }
}
