/* ?GetParent@psSECTION_DATA@@QBA?AVpsSECTION@@H@Z @0x8251A6E0 */
#include "psSECTION_DATA.h"
#include "psSECTION.h"
#include "psSECTION_BRANCH.h"
// 0x8251A6E0 — parent section handle at index `idx`; empty when out of range. The returned handle
// takes a reference on the parent body (and its branch). sret return: the decompiler swaps the sret
// slot with `this` (the real `this` is the section body, spelled `result` there).

psSECTION psSECTION_DATA::GetParent(int idx) const
{
    psSECTION_DATA *body = nullptr;
    if (idx >= 0 && idx < this->psParents.nElem) {
        body = this->psParents[idx].pData;
        if (body) {
            ++body->refCount;
            ++body->pBranch->refCount;
        }
    }
    psSECTION parent;
    parent.pData = body;
    return parent;
}
