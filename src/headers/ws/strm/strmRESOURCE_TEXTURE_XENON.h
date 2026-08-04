#pragma once
#include "strmRESOURCE_TEXTURE.h"
#include "../../d3d_render_boundary.h"
// ws-engine strm (streaming): the Xenon (Xbox 360) GPU-backed specialization of
// strmRESOURCE_TEXTURE — owns the live D3DTexture description and the strmVID_BUFFER/
// strmRES_TEX_HEADER_CACHE handles for its uploaded mip data.
// DB-verified layout (types_members strmRESOURCE_TEXTURE_XENON): base strmRESOURCE_TEXTURE@0
// (348B), d3dTex@348 (D3DTexture, 52B), mipCount@400, sizeX@404, sizeY@408, size@412,
// memHandle@416 (strmVID_BUFFER*), headerCache@420 (strmRES_TEX_HEADER_CACHE*) — size 424.

struct strmVID_BUFFER;           // strmVID_BUFFER.h
struct strmRES_TEX_HEADER_CACHE; // boundary — left opaque in this batch

struct strmRESOURCE_TEXTURE_XENON : strmRESOURCE_TEXTURE {
    D3DTexture                d3dTex;      // 0x15C
    int                         mipCount;    // 0x190
    int                         sizeX;        // 0x194
    int                         sizeY;        // 0x198
    unsigned int                size;          // 0x19C
    strmVID_BUFFER             *memHandle;      // 0x1A0
    strmRES_TEX_HEADER_CACHE   *headerCache;    // 0x1A4

    strmRESOURCE_TEXTURE_XENON();  // 0x82BF7F58 — boundary, external to this batch
    ~strmRESOURCE_TEXTURE_XENON() override; // boundary

    // 0x8266FF50 — copy-assign the base strmRESOURCE_TEXTURE, then this class's own fields
    // (d3dTex verbatim, mipCount/sizeX/sizeY/size/memHandle/headerCache).
    strmRESOURCE_TEXTURE_XENON &operator=(const strmRESOURCE_TEXTURE_XENON &that);

    int CalcNeededMemSize() override;    // boundary — external to this batch
    int GetAdditionalSeek() override;    // boundary — external to this batch
    void Clear();                        // boundary — external to this batch
    void CancelReadData();               // boundary — external to this batch
};
