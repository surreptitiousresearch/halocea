/* ?UpdateInvalidateState@animINST@@AAAHH@Z @0x825E1E38 */
#include "animINST.h"
#include "animTPL.h"
#include "tplSKIN.h"

// ?UpdateInvalidateState@animINST@@AAAHH@Z  (0x825E1E38, private) — expand a raw set of requested
// validity bits `vState` into the full set of dependent bits that must be re-validated, taking the
// instance's skeleton / skin-collision / physics configuration into account. Returns the expanded
// mask.
int animINST::UpdateInvalidateState(int vState)
{
    animTPL *pTpl = this->pTpl;
    tplSKIN *pSkin = pTpl ? pTpl->pSkin : nullptr;
    bool skinHasCdtSkel = pSkin && (this->state & 0x4000000) != 0 && pSkin->pCdtSkel != nullptr;
    bool hasCdtSkeleton = this->pSkel != nullptr || skinHasCdtSkel;

    int expanded = 0;
    if ((vState & 1) != 0)
        expanded = hasCdtSkeleton ? 8449 : 8193;

    if ((vState & 2) != 0)
        expanded |= 0x3103u;
    if ((vState & 0x800) != 0)
        expanded |= 0x2901u;
    if ((vState & 0x100) != 0)
        expanded |= 0x100u;
    if ((vState & 0x20) != 0)
        expanded |= 0x20u;
    if ((vState & 0x200000) != 0)
        expanded |= 0x200000u;

    if ((vState & 0x1000) != 0)
    {
        expanded |= 0x3100u;
        if (!hasCdtSkeleton && this->pPhysChar)
            expanded |= 3u;
    }

    if ((vState & 0x2000) == 0)
        return expanded;

    expanded |= 0x2000u;
    if (hasCdtSkeleton)
        return expanded | 0x100;
    if (this->pPhysChar)
        return expanded | 1;
    return expanded;
}
