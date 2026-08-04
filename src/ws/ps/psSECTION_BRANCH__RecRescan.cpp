#include "psSECTION_BRANCH.h"
#include "psSECTION_DATA.h"
#include "psSECTION.h"
// 0x825190B8 — recompute this branch's reference count over `pData` and its sub-tree: add each
// section's own refCount, then subtract one for every intra-branch parent link and one for every
// sub-section (which is recursed into). Protected (IAA).

void psSECTION_BRANCH::RecRescan(psSECTION_DATA *pData)
{
    this->refCount += pData->refCount;

    for (int i = 0; i < pData->psParents.nElem; ++i) {
        if (pData->psParents[i].pData->pBranch == this)
            --this->refCount;
    }

    for (int i = 0; i < pData->orderedRecords.nElem; ++i) {
        psSECTION_DATA *subSection = pData->GetSubSection(i);
        if (subSection) {
            --this->refCount;
            this->RecRescan(subSection);
        }
    }
}
