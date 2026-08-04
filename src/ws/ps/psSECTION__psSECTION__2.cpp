#include "psSECTION.h"
#include "psSECTION_DATA.h"
#include "psSECTION_BRANCH.h"
// 0x825168C8 (??0psSECTION@@QAA@ABV0@@Z) — referenced copy ctor: share `sect`'s section body,
// bumping both the body's and its owning branch's refcounts (a no-op for an empty source handle).

psSECTION::psSECTION(const psSECTION &sect)
{
    psSECTION_DATA *pData = sect.pData;
    if (pData) {
        ++pData->refCount;
        ++pData->pBranch->refCount;
    }
    this->pData = sect.pData;
}
