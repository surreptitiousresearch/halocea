// pctGetPixel @0x827C1B28 — read one texel from a picture as an unpacked pctPIXEL.
// Dispatches by format class: palettized (format 18..21 or 34/35) -> palette lookup;
// block-compressed (pctFORMAT.state bit 3) -> DXT decode; otherwise -> linear buffer unpack.
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp
#include "../../headers/pctPICTURE.h"
#include "../../headers/pctFORMAT.h"
#include "../../headers/ws/gs/gsRENDER_SYSTEM_boundary.h" // pctPIXEL definition
#include "../../headers/pct_boundary.h"                    // pctGetFormat
#include "../../headers/pct_format.h"

// pct pixel-access helpers — reversed elsewhere / lower level. boundary decls.
extern void  pctGetPalPixel(pctPICTURE *pPict, int x, int y, int mip, pctPIXEL *pPix, int z, int face);
extern void  pctGetDXTPixel(pctPICTURE *pPict, int x, int y, int mip, pctPIXEL *pPix, int z, int face);
extern void *pctGetPixelInfoPtr(pctPICTURE *pPict, int x, int y, int mip, int z, int face);
extern void  pctGetPixelBuf(int format, void *pPixelInfo, pctPIXEL *pPix);

void pctGetPixel(pctPICTURE *pPict, int x, int y, int mipmapNmb, pctPIXEL *pPix, int z, int face)
{
    int format = pPict->hdr.format;

    // palettized formats (P8/P4/AP8/AP4 and their swizzled variants) go through the palette lookup path
    if (!(format > PCT_FORMAT_SWIZ_AP8
          || (format < PCT_FORMAT_SWIZ_P8 && (format < PCT_FORMAT_P8 || format > PCT_FORMAT_AP4))))
    {
        pctGetPalPixel(pPict, x, y, mipmapNmb, pPix, z, face);
        return;
    }

    if ((unsigned int)format < PCT_FORMAT_MAX /* deviation: literal <= 0x25 (=37) */ && (pctGetFormat(format)->state & 8) != 0)
    {
        pctGetDXTPixel(pPict, x, y, mipmapNmb, pPix, z, face);
    }
    else
    {
        void *pPixelInfo = pctGetPixelInfoPtr(pPict, x, y, mipmapNmb, z, face);
        pctGetPixelBuf(pPict->hdr.format, pPixelInfo, pPix);
    }
}
