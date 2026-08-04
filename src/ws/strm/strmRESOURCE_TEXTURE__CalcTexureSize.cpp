#include "../../headers/ws/strm/strmRESOURCE_TEXTURE.h"

// strmRESOURCE_TEXTURE::CalcTexureSize @ 0x82BF6BB8
// Memoized hardware-swizzled byte size for (sizeX, sizeY, mipCount) at this texture's pixel
// format (header.format); a cache hit (same inputs as the last call) returns the cached size
// without touching the hardware size formula.
unsigned int strmRESOURCE_TEXTURE::CalcTexureSize(unsigned int sizeX, unsigned int sizeY, unsigned int mipCount)
{
    if (sizeX == (unsigned int)sizeCalc.cacheSizeX &&
        sizeY == (unsigned int)sizeCalc.cacheSizeY &&
        mipCount == (unsigned int)sizeCalc.cacheMipCount) {
        return sizeCalc.cacheSize;
    }

    sizeCalc.cacheSizeX = sizeX;
    sizeCalc.cacheSizeY = sizeY;
    sizeCalc.cacheMipCount = mipCount;

    unsigned int size = txmCalcTexureSizeHW(sizeX, sizeY, mipCount, header.format);
    sizeCalc.cacheSize = size;
    return size;
}
