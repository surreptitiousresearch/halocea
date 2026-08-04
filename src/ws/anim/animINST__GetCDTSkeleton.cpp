#include "animINST.h"
#include "animTPL.h"
#include "tplSKIN.h"

// animINST::GetCDTSkeleton @ 0x825E1958
// Return the instance's own collision skeleton if it has one; otherwise fall back to the
// template's skin skeleton, but only once the instance's skin has been resolved
// (state & 0x4000000).
cdtSKEL *animINST::GetCDTSkeleton()
{
    cdtSKEL *pSkel = this->pSkel;
    if (pSkel)
        return pSkel;

    animTPL *pTpl = this->pTpl;
    if (pTpl) {
        tplSKIN *pSkin = pTpl->pSkin;
        if (pSkin && (this->state & 0x4000000) != 0)
            return pSkin->pCdtSkel;
    }
    return nullptr;
}
