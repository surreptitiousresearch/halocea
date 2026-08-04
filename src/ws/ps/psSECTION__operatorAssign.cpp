#include "psSECTION.h"
#include "psSECTION_DATA.h"
#include "psSECTION_BRANCH.h"
// 0x8251A1B4 — referenced copy-assign: bump `sect`'s body/branch refcounts (when non-null), release
// this handle's prior body (when non-null), then adopt `sect`'s body pointer.

psSECTION &psSECTION::operator=(const psSECTION &sect)
{
    psSECTION_DATA *newData = sect.pData;
    if (newData) {
        ++newData->refCount;
        ++newData->pBranch->refCount;
    }

    if (this->pData)
        this->pData->Release();

    this->pData = sect.pData;
    return *this;
}
