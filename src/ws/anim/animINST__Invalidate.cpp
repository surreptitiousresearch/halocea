/* ?Invalidate@animINST@@QAAXHH@Z @0x825E20E8 */
#include "animINST.h"
#include "anim_validate_boundary.h"

// ?Invalidate@animINST@@QAAXHH@Z  (0x825E20E8) — mark validity bits `vState` dirty on this
// instance, expand them through the modifier hierarchy, and propagate the resulting invalidation to
// the collision scene and physics system as needed.
void animINST::Invalidate(int vState, int invalidateCullInfo)
{
    if ((vState & this->stateVld) == vState || (this->state2 & 1) != 0)
        return;

    if (invalidateCullInfo)
        osLockedSet(&this->cullInfo.isValid, 0);

    this->perInstLock.Lock(nullptr, 0);

    unsigned int expandedState = this->UpdateInvalidateState(vState);
    if (this->pObj && (this->stateVldModHierReq.state & expandedState) != 0)
        expandedState = this->stateVldModHier.state | expandedState;

    int newlyDirty = this->UpdateInvalidateState(expandedState);
    this->stateVld |= newlyDirty;
    this->perInstLock.Unlock(nullptr, 0);

    if ((newlyDirty & 0x100) != 0 && (((unsigned int)this->state >> 4) & 1) == 0)
    {
        scnSCENE *pScn = this->pScn;
        if (pScn)
        {
            cdtCOLL_ANIM_SCN *pCollAnim = pScn->pCollAnim;
            if (pCollAnim)
                pCollAnim->Invalidate(this, 0);
        }
    }

    if ((newlyDirty & 1) != 0)
    {
        physSYSTEM_BASE *pPhysSystem = this->pPhysSystem;
        if (pPhysSystem)
            pPhysSystem->state.state |= 4u;
    }
}
