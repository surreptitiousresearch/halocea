#include "../../headers/pctPICTURE.h"
#include "../../headers/ws/gs/gsRENDER_SYSTEM_boundary.h" /* pctPIXEL (dsVECTOR element) */

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// 0x827C19E8 — free a decoded picture and everything it owns: the palette vector
// (its element buffer via dlFree, then the vector object), each per-face pixel
// buffer, the face pointer array, and finally the pctPICTURE itself.
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp
void pctDestroy(pctPICTURE *pPict)
{
    if (!pPict)
        return;

    dsVECTOR<pctPIXEL, 8> *pPalette = pPict->pPalette;
    if (pPalette)
    {
        dlFree(pPalette->pData);
        ::operator delete(pPalette);
    }

    if (pPict->pFaces)
    {
        for (int face = 0; face < pPict->hdr.nFaces; ++face)
        {
            if (pPict->pFaces[face])
                dlFree(pPict->pFaces[face]);
        }
        dlFree(pPict->pFaces);
    }

    ::operator delete(pPict);
}
