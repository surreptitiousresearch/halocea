// ?pctDuplicate@@YAPAUpctPICTURE@@PAU1@@Z  (@0x827C67D8)
// Deep-copy a picture: create a new picture with the same header, then memcpy each face's pixel
// buffer (one mip-chain-sized block per face).
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp

#include "../../headers/pct_create_boundary.h"
#include <string.h> /* memcpy */

pctPICTURE *pctDuplicate(pctPICTURE *pPicSrc)
{
    pctHEADER hdr;
    hdr.sign    = PCT_SIGN;
    hdr.sx      = pPicSrc->hdr.sx;
    hdr.sy      = pPicSrc->hdr.sy;
    hdr.sz      = pPicSrc->hdr.sz;
    hdr.nFaces  = pPicSrc->hdr.nFaces;
    hdr.nMipMap = pPicSrc->hdr.nMipMap;
    hdr.format  = pPicSrc->hdr.format;

    pctPICTURE *pic = pctCreate(&hdr);
    if (!pic)
        return pic;

    unsigned int faceSize = (unsigned int)pctGetMipMapOffset(&pic->hdr, pic->hdr.nMipMap);
    for (int face = 0; face < pPicSrc->hdr.nFaces; ++face)
        memcpy(pic->pFaces[face], pPicSrc->pFaces[face], faceSize);

    return pic;
}
