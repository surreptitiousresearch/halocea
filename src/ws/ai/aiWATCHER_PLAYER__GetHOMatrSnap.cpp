#include "../../headers/ws/ai/aiWATCHER_PLAYER.h"
#include "../../headers/ws/ai/aiWATCHER_PLAYER_fsm_boundary.h" // propFSM + fsmBODY_HO_INFO
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"    // mdlITRC_BASE + m3dMatrIdentity
#include "../../headers/ws/m3d/m3dMATR.h"

// aiWATCHER_PLAYER::GetHOMatrSnap @ 0x83290A50
// ?GetHOMatrSnap@aiWATCHER_PLAYER@@UAA_NAAVm3dMATR@@@Z
// Virtual: query the body FSM for the current hideout ITRC (via the get_ho_info event callback);
// when that ITRC is live and is a hideout, write its snap matrix into `mSnap` and return true.
// Otherwise write identity and return false. Invalid watcher -> identity + false.
bool aiWATCHER_PLAYER::GetHOMatrSnap(m3dMATR &mSnap)
{
    if (IsValid())
    {
        dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > itrcHold;
        itrcHold.pointee = nullptr;
        itrcHold.counter = 0;

        if (bodyFsm)
        {
            fsmBODY_HO_INFO hoInfo;
            bodyFsm->eventHandler.ActivateCB(fsmBODY_FSM_EVENT_events::get_ho_info(), &hoInfo);
            itrcHold = hoInfo.itrc;
            hoInfo.itrcMpSafe.Reset();
            hoInfo.itrcFrom.Reset();
            hoInfo.itrc.Reset();

            mdlITRC_BASE *itrc = itrcHold.pointee;
            if (itrc && itrc->IsValid() && itrc->IsHO())
            {
                itrc->GetSnapMatr(&mSnap);
                itrcHold.Reset();
                return true;
            }
        }

        mSnap = m3dMatrIdentity;
        itrcHold.Reset();
    }
    else
    {
        mSnap = m3dMatrIdentity;
    }
    return false;
}
