#include "../../headers/pctHEADER.h"
#include "../../headers/pct_boundary.h"

// 0x827C1C20 — total pixel-data byte size for the whole picture: one face's
// full mip chain (offset past the last mip) times the face count.
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp
int pctGetMemSize(pctHEADER *pHdr)
{
    return pctGetMipMapOffset(pHdr, pHdr->nMipMap) * pHdr->nFaces;
}
