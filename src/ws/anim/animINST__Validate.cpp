/* ?Validate@animINST@@QAAXH@Z @0x825E4848 */
#include "animINST.h"
#include "animTPL.h"
#include "tplSKIN.h"
#include "animTRACKER.h"
#include "anim_validate_boundary.h"

// ?Validate@animINST@@QAAXH@Z  (0x825E4848) — bring the requested validity bits `vState` up to date
// under the per-instance lock: follow tracker, animation pose, local-to-world matrices, OBB,
// modifiers and hidden-skin state, clearing each bit as it is serviced. Dispatch mirrors the
// resolved cdt skeleton (own, skin, or none).
void animINST::Validate(int vState)
{
    if ((vState & this->stateVld) == 0)
        return;

    osLOCK *pPerInstLock = &this->perInstLock;
    this->state2 |= 8u;
    this->perInstLock.Lock(nullptr, 0);

    unsigned int expanded = this->UpdateValidateState(vState);
    if (this->pObj)
    {
        if ((this->stateVldModHier.state & expanded) != 0)
            expanded = this->stateVldModHierReq.state | expanded;
        expanded = this->UpdateValidateState(expanded);
    }

    int stateVld = this->stateVld;
    if ((stateVld & 0x20) != 0)
        this->stateVld = stateVld & ~0x20u;

    // resolve the collision skeleton driving this instance (own, skin-provided, or none)
    cdtSKEL *pSkel;
    if (this->pSkel)
    {
        pSkel = this->pSkel;
    }
    else
    {
        animTPL *pTpl = this->pTpl;
        if (pTpl && pTpl->pSkin && (this->state & 0x4000000) != 0)
            pSkel = pTpl->pSkin->pCdtSkel;
        else
            pSkel = nullptr;
    }

    // follow tracker (0x800)
    if ((expanded & 0x800) != 0 && (this->stateVld & 0x800) != 0)
    {
        animTRACKER *followTracker = this->followTracker;
        m3dMATR trackMatr;
        if (followTracker && followTracker->GetMatrix(&trackMatr))
        {
            this->matrL2W.Transform(&trackMatr, 0);
            this->matrL2W.Check();
        }
        this->stateVld &= ~0x800u;
    }

    // animation pose (0x2), else fall through to the deferred-anim path (0x1000)
    if ((expanded & 2) != 0 && (this->stateVld & 2) != 0)
    {
        if ((this->state2 & 0x4000000) == 0)
        {
            if ((this->stateVld & 0x1000) != 0 || !pSkel)
                this->ApplyAnim();
            else if (this->pAnimMng)
                pSkel->CalcAnimExcl(this);
        }
        this->stateVld &= ~0x1002u;
    }
    else if ((expanded & 0x1000) != 0 && (this->stateVld & 0x1000) != 0
             && (this->state2 & 0x4000000) == 0 && pSkel)
    {
        if (this->pAnimMng)
            pSkel->CalcAnim(this);
        this->stateVld &= ~0x1000u;
    }

    // local-to-world (0x1), else the deferred-LTM path (0x2000)
    if ((expanded & 1) != 0 && (this->stateVld & 1) != 0)
    {
        if ((this->state2 & 4) == 0)
        {
            if (pSkel && (this->stateVld & 0x2000) == 0)
                pSkel->CalcLTMExcl(this);
            else
                objCalcLTM(this->pObj);
        }
        this->stateVld &= ~0x2001u;
    }
    else if ((expanded & 0x2000) != 0 && (this->stateVld & 0x2000) != 0 && pSkel)
    {
        pSkel->CalcLTM(this);
        this->stateVld &= ~0x2000u;
    }

    // oriented bounding box (0x100)
    if ((expanded & 0x100) != 0 && (this->stateVld & 0x100) != 0)
    {
        this->ValidateOBB();
        this->stateVld &= ~0x100u;
    }

    // object modifiers (0x20)
    if ((expanded & 0x20) != 0)
    {
        this->stateVld &= ~0x20u;
        this->ApplyModifiers(expanded);
    }

    // hidden-skin state (0x200000)
    if ((expanded & 0x200000) != 0 && (this->stateVld & 0x200000) != 0)
    {
        this->ValidateHidden();
        this->stateVld &= ~0x200000u;
    }

    pPerInstLock->Unlock(nullptr, 0);
    this->state2 &= ~8u;
}
