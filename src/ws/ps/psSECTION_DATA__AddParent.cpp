/* ?AddParent@psSECTION_DATA@@QAAXVpsSECTION@@@Z @0x8251C508 */
#include "psSECTION_DATA.h"
#include "psSECTION.h"
#include "psSECTION_BRANCH.h"
// 0x8251C508 — append `parent` to this section's parent list. Takes a reference on the parent body
// (and its branch), drops any existing entry for the same body, appends the handle, undoes the
// branch-ref bump when the parent shares this section's branch, then releases the incoming handle.

void psSECTION_DATA::AddParent(psSECTION parent)
{
    if (!parent.pData)
        return;

    psSECTION_DATA *parentBody = parent.pData;
    ++parentBody->refCount;
    ++parentBody->pBranch->refCount;

    psSECTION existing;
    existing.pData = parent.pData;
    this->RemoveParent(existing);

    int needed = this->psParents.nElem + 1;
    if (needed > this->psParents.allocated)
        this->psParents.Realloc(needed);
    this->psParents.PushBack(parent);

    psSECTION_BRANCH *thisBranch = this->pBranch;
    if (parent.pData->pBranch == thisBranch)
        --thisBranch->refCount;

    if (parent.pData)
        parent.pData->Release();
}
