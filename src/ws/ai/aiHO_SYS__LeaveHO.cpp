#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"           // pBrain->st
#include "../../headers/ws/ai/aiSTATUS.h"           // aiSTATUS::peekerSelf
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"    // aiWATCHER_BRAIN::IsHideoutMode
#include "../../headers/ws/mdl/mdlITRC_BASE.h"      // ItrcPtr::pointee

// aiHO_SYS::LeaveHO @ 0x832A78A0
// ?LeaveHO@aiHO_SYS@@UAAXXZ
//
// Request leaving the current hideout (unless the subsystem is pinned always-in-HO). If still holding
// the HO, schedule the leave for next frame (countdown = 1). If not, and the owning watcher is peeking
// a hideout, force the leave event immediately.
void aiHO_SYS::LeaveHO()
{
    if (alwaysInHO)
        return;

    ItrcPtr current = GetCurrentItrc();
    bool inHO = current.pointee != nullptr;
    current.Reset();

    if (inHO) {
        framesToLeaveHO = 1;
    } else {
        aiWATCHER_BRAIN *peeker = pBrain->st.pointee->peekerSelf;
        if (peeker && peeker->IsHideoutMode())
            ForceEventLeaveHO();
    }
}
