// ?pctCreate@@YAPAUpctPICTURE@@HHHHHH@Z  (@0x827C62F0)
// Convenience overload: fill a stack pctHEADER from six explicit parameters and forward to
// pctCreate(pctHEADER*).
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp

#include "../../headers/pct_create_boundary.h"

pctPICTURE *pctCreate(int idxFmt, int sx, int sy, int nMipMap, int nFaces, int sz)
{
    pctHEADER hdr;
    hdr.sign    = PCT_SIGN;
    hdr.sx      = sx;
    hdr.sy      = sy;
    hdr.sz      = sz;
    hdr.nFaces  = nFaces;
    hdr.nMipMap = nMipMap;
    hdr.format  = idxFmt;
    return pctCreate(&hdr);
}
