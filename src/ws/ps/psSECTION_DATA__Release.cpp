#include "psSECTION_DATA.h"
#include "psSECTION_BRANCH.h"
// 0x8251A000 — drop one reference on this section body (and its branch). When a rescan is pending
// (and the branch is not being deleted) it consumes the flag and re-scans the tree first; the
// branch is freed once its reference count reaches zero.
//
// CAVEAT: on the pending-rescan path the branch's refCount is written to 0 before the reference
// decrements below (r10 == 0 in this code path). Reproduced verbatim from the disassembly; the
// subsequent --branch->refCount then underflows relative to that reset, which is the binary's
// behavior, not a translation error.

void psSECTION_DATA::Release()
{
    psSECTION_BRANCH *branch = this->pBranch;
    if (branch->isDeleting)
        return;

    if (branch->isNeedRescan) {
        branch->refCount = 0;        // raw: reset before the rescan (r10==0 on this path)
        branch->isNeedRescan = 0;
        if (!branch->isDeleting)     // always true under the outer guard; preserved from the binary
            branch->RecRescan(branch->pTopSection);
    }

    --this->refCount;
    --this->pBranch->refCount;
    if (this->pBranch->refCount == 0)
        this->pBranch->Delete();
}
