#pragma once
// ws-engine strm (streaming): a one-entry memoization cache for
// strmRESOURCE_TEXTURE::CalcTexureSize (avoids re-deriving the hardware-swizzled size when
// called repeatedly with the same (sizeX, sizeY, mipCount)).
// DB-verified layout (types_members strmRES_TEX_SIZE_CALC): size 16.

typedef struct strmRES_TEX_SIZE_CALC {
    int cacheSizeX;    // 0x00
    int cacheSizeY;    // 0x04
    int cacheMipCount; // 0x08
    int cacheSize;      // 0x0C last computed size, returned verbatim on a cache hit

    // 0x82BF6600-family — boundary, external to this batch.
    int CalcTexureSize(int sizeX, int sizeY, int mipCount, int format);
} strmRES_TEX_SIZE_CALC;
