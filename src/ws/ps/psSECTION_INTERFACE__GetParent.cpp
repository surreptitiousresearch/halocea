/* ?GetParent@psSECTION_INTERFACE@@QBA?AVpsSECTION@@H@Z @0x8251AC90 */
#include "psSECTION_INTERFACE.h"
#include "psSECTION.h"
#include "psSECTION_DATA.h"
#include "psSECTION_BRANCH.h"
// 0x8251AC90 — parent section handle at index `idx`, forwarding to the body. The body returns an
// owned handle; this re-references its body, stores it into the result, then releases the temp
// handle. sret return: the decompiler swaps the sret slot with `this`.

psSECTION psSECTION_INTERFACE::GetParent(int idx) const
{
    psSECTION result;
    if (this->pData) {
        psSECTION parent = this->pData->GetParent(idx);
        psSECTION_DATA *body = parent.pData;
        if (body) {
            ++body->refCount;
            ++body->pBranch->refCount;
        }
        result.pData = parent.pData;
        if (parent.pData)
            parent.pData->Release();
    } else {
        result.pData = nullptr;
    }
    return result;
}
