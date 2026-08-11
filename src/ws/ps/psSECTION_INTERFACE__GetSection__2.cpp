#include "psSECTION_INTERFACE.h"
#include "psSECTION.h"
#include "psSECTION_DATA.h"
#include "psSECTION_BRANCH.h"
// @0x8251EDB0 — the section handle stored under `key`, returned BY VALUE as a fresh reference. When
// `key` is null the result is a referenced copy of this handle. sret return: the decompiler swaps
// the sret slot with `this` (the real `this` is spelled `result` there). Overload of the out-param
// GetSection.

psSECTION psSECTION_INTERFACE::GetSection(const char *key, unsigned int flags) const
{
    psSECTION out;

    if (key) {
        psSECTION found;
        found.pData = nullptr;
        this->GetSection(key, &found, flags); // out-param overload

        psSECTION_DATA *body = found.pData;
        if (found.pData) {
            ++found.pData->refCount;
            ++found.pData->pBranch->refCount;
            body = found.pData;
        }
        out.pData = body;
        if (body)
            body->Release();
    } else {
        if (this->pData) {
            ++this->pData->refCount;
            ++this->pData->pBranch->refCount;
        }
        out.pData = this->pData;
    }

    return out;
}
