#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/m3d/m3dMATR.h"

// aiWATCHER_BRAIN::GetHOMatrSnap(m3dMATR& mSnap) @ 0x83172B00
// ?GetHOMatrSnap@aiWATCHER_BRAIN@@UAA_NAAVm3dMATR@@@Z
//
// Write the current hideout snap matrix into `mSnap` and return true when the tracked model has a
// live hideout ITRC; otherwise write identity and return false. (An invalid watcher also yields
// identity + false.)
bool aiWATCHER_BRAIN::GetHOMatrSnap(m3dMATR &mSnap)
{
    if (!IsValid())
    {
        mSnap = m3dMatrIdentity;
        return false;
    }

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > itrc = pBrain->body.pointee->GetCurrentItrcHO();
    if (itrc.pointee && itrc.pointee->IsHO())
    {
        itrc.pointee->GetSnapMatr(&mSnap);
        return true;
    }

    mSnap = m3dMatrIdentity;
    return false;
}
