#include "headers/ws/txm/txm_functions.h"

// 0x827BBC48 -- computes the tiled hardware size (in bytes) of a 2D texture via the XDK's
// XGSetTextureHeader, returning only the base-size out-param. DEVIATION: the decompiler left
// the trailing 4 XGSetTextureHeader out-params (BaseOffset/MipOffset/pTexture/pMipSize slots)
// as uninitialised locals with unresolved types; reconstructed here as the real XDK parameter
// shapes (D3DTexture* / unsigned int*) per d3d_boundary.h's XGSetTextureHeader declaration.
unsigned int txmCalcTexureSizeHW(unsigned int sizeX, unsigned int sizeY, unsigned int mipCount, int pctFormat)
{
    _D3DFORMAT   d3dFormat = txmPctFmtToD3dFmt(pctFormat, 0);
    D3DTexture  *pTexture  = nullptr;
    unsigned int baseSize  = 0;
    unsigned int mipSize   = 0;

    return XGSetTextureHeader(sizeX, sizeY, mipCount, 0, d3dFormat, 0, 0, 0xFFFFFFFFu,
                               0, pTexture, &baseSize, &mipSize);
}
