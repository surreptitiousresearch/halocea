/* ?pctGetMipMapOffsetPtr@@YAPAXPAUpctPICTURE@@HH@Z @0x827C1240 */
#include "../../headers/pctPICTURE.h"
#include "../../headers/pct_boundary.h"

// 0x827C1240 — pointer to the pixel data of mip level `mipMap` on face `face`
// within a decoded picture: the face's base pointer plus the mip's byte offset.
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp
char *pctGetMipMapOffsetPtr(pctPICTURE *pPict, int mipMap, int face)
{
    return (char *)pPict->pFaces[face] + pctGetMipMapOffset(&pPict->hdr, mipMap);
}
