#include "psSECTION_DATA.h"
#include <cstddef>
#include "psSECTION_BRANCH.h"
#include "psSECTION.h"
// 0x82519938 — re-parent this section under `pNewOwner`, moving it (and its whole sub-tree) onto the
// owner's branch. A null owner detaches the section into a freshly-allocated branch it roots. The
// old branch is deleted if this section rooted it, else flagged rescan-pending; the new branch is
// likewise flagged rescan-pending.

extern void *operator new(size_t size, const char *file, unsigned int line); // boundary — debug allocator
extern void operator delete(void *ptr);                                             // boundary

void psSECTION_DATA::SetOwner(psSECTION_DATA *pNewOwner)
{
    if (this->pOwner == pNewOwner)
        return;

    psSECTION_BRANCH *oldBranch = this->pBranch;

    // No-op when re-adopting the owner that already roots this section's branch.
    if (pNewOwner && pNewOwner->pBranch == oldBranch && this == oldBranch->pTopSection)
        return;

    this->pOwner = pNewOwner;

    psSECTION_BRANCH *newBranch;
    if (pNewOwner) {
        newBranch = pNewOwner->pBranch;
    } else {
        psSECTION_BRANCH *branch = (psSECTION_BRANCH *)operator new(
            sizeof(psSECTION_BRANCH), "D:\\Projects\\code\\common\\src.sys\\ps\\ps_section.cpp", 0x64Du);
        if (branch) {
            branch->pTopSection = this;
            branch->refCount = 0;
            branch->isDeleting = 0;    // raw: __ROL4__(1,7)&0xC0 sets only the 0x80 bit
            branch->isNeedRescan = 1;
        }
        newBranch = branch;
    }
    this->pBranch = newBranch;

    // Re-home every directly-owned sub-section onto the new branch.
    for (int i = 0; i < this->orderedRecords.nElem; ++i) {
        psSECTION_DATA *subSection = this->GetSubSection(i);
        if (subSection) {
            subSection->pBranch = nullptr;
            subSection->pOwner = nullptr;
            subSection->SetOwner(this);
        }
    }

    if (oldBranch) {
        if (this == oldBranch->pTopSection)
            operator delete(oldBranch);
        else
            oldBranch->isNeedRescan = 1; // |= 0x80
    }
    this->pBranch->isNeedRescan = 1;     // |= 0x80
}
