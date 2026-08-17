/* ?RemoveParent@psSECTION_DATA@@QAAXVpsSECTION@@@Z @0x8251B6C8 */
#include "psSECTION_DATA.h"
#include "psSECTION.h"
#include "psSECTION_BRANCH.h"
// 0x8251B6C8 — remove the first parent matching `psParent` from the parent list. If the matching
// parent shares this section's branch, its branch reference is bumped before the erase; the
// incoming by-value handle is released on exit regardless of whether a match was found.

void psSECTION_DATA::RemoveParent(psSECTION psParent)
{
    for (int idx = 0; idx < this->psParents.nElem; ++idx) {
        if (this->psParents[idx].pData == psParent.pData) {
            if (psParent.pData->pBranch == this->pBranch)
                ++this->pBranch->refCount;
            this->psParents.Erase(idx, 1);
            break;
        }
    }

    if (psParent.pData)
        psParent.pData->Release();
}
