// pctSetPixel @0x827C1A80 — write one texel into a picture from an unpacked pctPIXEL.
// Only linear (non-palettized, non-block-compressed) formats are writable here; palettized and
// DXT formats fall through and are silently ignored (matches the raw guard).
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp
#include "../../headers/pctPICTURE.h"
#include "../../headers/pctFORMAT.h"
#include "../../headers/pct_format.h"
#include "../../headers/ws/gs/gsRENDER_SYSTEM_boundary.h" // pctPIXEL definition
#include "../../headers/pct_boundary.h"                    // pctGetFormat

extern void *pctGetPixelInfoPtr(pctPICTURE *pPict, int x, int y, int mip, int z, int face);
extern void  pctSetPixelBuf(int format, void *pPixelInfo, const pctPIXEL *pPix);

void pctSetPixel(pctPICTURE *pPict, int x, int y, int mipmapNmb, const pctPIXEL *pPix, int z, int face)
{
    int format = pPict->hdr.format;

    // not palettized (18..21 / 34..35) AND not block-compressed -> linear, writable
    bool notPalettized = (format > 35 || (format < 34 && (format < 18 || format > 21)));
    bool notCompressed = ((unsigned int)format >= PCT_FORMAT_MAX || (pctGetFormat(format)->state & 8) == 0);
    if (notPalettized && notCompressed)
    {
        void *pPixelInfo = pctGetPixelInfoPtr(pPict, x, y, mipmapNmb, z, face);
        pctSetPixelBuf(pPict->hdr.format, pPixelInfo, pPix);
    }
}
