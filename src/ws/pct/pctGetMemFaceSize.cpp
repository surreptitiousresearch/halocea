/* ?pctGetMemFaceSize@@YAHPAUpctHEADER@@@Z @0x827C1C58 */
#include "../../headers/pctHEADER.h"
#include "../../headers/pct_boundary.h"

// 0x827C1C58 — byte size of a single face's full mip chain (offset past the last
// mip level).
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp
int pctGetMemFaceSize(pctHEADER *pHdr)
{
    return pctGetMipMapOffset(pHdr, pHdr->nMipMap);
}
