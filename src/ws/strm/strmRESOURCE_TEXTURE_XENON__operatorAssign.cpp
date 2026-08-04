#include "../../headers/ws/strm/strmRESOURCE_TEXTURE_XENON.h"

// strmRESOURCE_TEXTURE_XENON::operator= @ 0x8266FF50
// Copy-assign the base strmRESOURCE_TEXTURE, then this class's own fields (the D3DTexture
// descriptor verbatim, and the mip/size/handle bookkeeping).
strmRESOURCE_TEXTURE_XENON &strmRESOURCE_TEXTURE_XENON::operator=(const strmRESOURCE_TEXTURE_XENON &that)
{
    strmRESOURCE_TEXTURE::operator=(that);
    d3dTex = that.d3dTex;
    mipCount = that.mipCount;
    sizeX = that.sizeX;
    sizeY = that.sizeY;
    size = that.size;
    memHandle = that.memHandle;
    headerCache = that.headerCache;
    return *this;
}
