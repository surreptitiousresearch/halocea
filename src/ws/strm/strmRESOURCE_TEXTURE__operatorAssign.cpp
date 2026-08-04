#include "../../headers/ws/strm/strmRESOURCE_TEXTURE.h"

// strmRESOURCE_TEXTURE::operator= @ 0x8266F5B8
// Field-by-field copy-assign. DEVIATION: the decompiler renders the (pakId..header) run and the
// (curData.list[5].pixels..lastData) run as raw 4-byte-word copy loops (7 and 8 iterations
// respectively) rather than named field assignments; DB-verified offsets confirm the first loop
// is exactly `header` (pctHEADER, 28 bytes = 7 words immediately after pakId) and the second is
// exactly `accDataVector.lastData` (32 bytes = 8 words immediately after curData.list[5].pixels)
// — reconstructed here as the equivalent named-field copies.
strmRESOURCE_TEXTURE &strmRESOURCE_TEXTURE::operator=(const strmRESOURCE_TEXTURE &that)
{
    state = that.state;
    idRec = that.idRec;
    pack = that.pack;
    resourceName.id = that.resourceName.id;
    pakId = that.pakId;
    header = that.header;

    maxMip = that.maxMip;
    minMip = that.minMip;
    curMip = that.curMip;
    reqMip = that.reqMip;
    viewMip = that.viewMip;
    loadingMip = that.loadingMip;
    removingMip = that.removingMip;
    skipMips = that.skipMips;
    matPriority = that.matPriority;
    dup_dvp = that.dup_dvp;
    ownerTex = that.ownerTex;
    counter = that.counter;
    lastBeh = that.lastBeh;

    accDataVector.curData = that.accDataVector.curData;
    accDataVector.lastData = that.accDataVector.lastData;

    mipAdd = that.mipAdd;
    sizeCalc.cacheSizeX = that.sizeCalc.cacheSizeX;
    sizeCalc.cacheSizeY = that.sizeCalc.cacheSizeY;
    sizeCalc.cacheMipCount = that.sizeCalc.cacheMipCount;
    sizeCalc.cacheSize = that.sizeCalc.cacheSize;

    return *this;
}
