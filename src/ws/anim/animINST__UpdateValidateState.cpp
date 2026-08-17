/* ?UpdateValidateState@animINST@@AAAHH@Z @0x825E1FB0 */
#include "animINST.h"
#include "animTPL.h"
#include "tplSKIN.h"
// cdtSKEL is used only as an opaque pointer here (forward-declared in animINST.h).

// ?UpdateValidateState@animINST@@AAAHH@Z  (0x825E1FB0) — private.
// Expand a set of requested validity bits (`state`) into the full internal validity mask the
// instance must recompute. The mask depends on whether the instance has a collision skeleton
// (its own, or the template's skin skeleton when state bit 0x4000000 is set).
int animINST::UpdateValidateState(int state)
{
    cdtSKEL *pSkel = this->pSkel;
    if (!pSkel)
    {
        animTPL *pTpl = this->pTpl;
        tplSKIN *pSkin;
        if (pTpl && (pSkin = pTpl->pSkin) != nullptr && (this->state & 0x4000000) != 0)
            pSkel = pSkin->pCdtSkel;
        else
            pSkel = nullptr;
    }
    bool hasSkel = pSkel != nullptr;

    int mask = 0;
    if ((state & 0x1) != 0)
        mask = 2051; // 0x803
    if ((state & 0x2) != 0)
    {
        mask |= 0x2;
        if (hasSkel)
            mask |= 0x800;
    }
    if ((state & 0x100) != 0)
    {
        mask |= 0x900;
        if (hasSkel)
            mask |= 0x3000;
    }
    if ((state & 0x20) != 0)
        mask |= 0x20;
    if ((state & 0x200000) != 0)
        mask |= 0x200000;
    if ((state & 0x2000) != 0)
    {
        mask |= 0x3800;
        if (!hasSkel)
            mask |= 0x3;
    }
    if ((state & 0x1000) != 0)
    {
        mask |= 0x1000;
        if (!hasSkel)
            mask |= 0x2;
    }
    if ((state & 0x800) != 0)
        return mask | 0x800;
    return mask;
}
