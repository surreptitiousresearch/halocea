#pragma once

#include "txmTEXTURE.h"
#include "../strm/strmVID_MEMMOVE_CB.h"
#include "../strm/strmRESOURCE_TEXTURE_XENON.h"
#include "../../_D3DFORMAT.h"
#include "../../d3d_render_boundary.h" /* D3DBaseTexture -- D3D9 boundary, not decompiled */

/* txmD3D_TEX -- ws-engine hardware-texture wrapper (txm subsystem); derives from txmTEXTURE
 * (base @0, size 0x94) and strmVID_MEMMOVE_CB (second base @0x94, 4B), modeled as public
 * inheritance per the corpus convention. DB-verified layout (types_members txmD3D_TEX, total
 * size 0x278 (632)): format@152 (_D3DFORMAT), pFmt@156 (pctFORMAT*), expAdjustBias@160,
 * hwDefaultSwizzle@164 (TXM_TEX_SWIZZLE_SRC[6]), lpSysSurf@188, lockedLevel@192,
 * lpSysSurfLowRes@196, lpSysSurfHiRes@200, memHandle@204 (strmVID_BUFFER*), strmRes@208
 * (strmRESOURCE_TEXTURE_XENON, 424B, embedded by value). */

struct strmVID_BUFFER; // ../strm/strmVID_BUFFER.h — forward decl only
struct pctFORMAT;      // pct subsystem — boundary (pointer only)

struct pctPICTURE; // pct boundary — used only as a pointer by LoadDrv

struct txmD3D_TEX : public txmTEXTURE
{
    /* 0x094 (148) second base class (strmVID_MEMMOVE_CB, 4B) — spelled as a leading member
     * because the verifier's header parser handles only single inheritance; same layout. */
    strmVID_MEMMOVE_CB memmoveCb;
    _D3DFORMAT       format;                /* 0x098 (152) */
    pctFORMAT       *pFmt;                  /* 0x09C (156) */
    int              expAdjustBias;         /* 0x0A0 (160) */
    TXM_TEX_SWIZZLE_SRC hwDefaultSwizzle[6];/* 0x0A4 (164) */
    D3DBaseTexture *lpSysSurf;             /* 0x0BC (188) -- the hardware texture handle */
    int              lockedLevel;           /* 0x0C0 (192) */
    D3DBaseTexture  *lpSysSurfLowRes;        /* 0x0C4 (196) -- DB-verified (types_members txmD3D_TEX) */
    D3DBaseTexture  *lpSysSurfHiRes;         /* 0x0C8 (200) */
    strmVID_BUFFER  *memHandle;              /* 0x0CC (204) */
    strmRESOURCE_TEXTURE_XENON strmRes;      /* 0x0D0 (208) -- embedded by value, 424B */

    // 0x827BB8E8 -- reversed in txmD3D_TEX__GetD3DTex.cpp.
    D3DBaseTexture *GetD3DTex() const;

    // 0x827BC510 / 0x827BC548 (?PrepareSurfForColorResolve@ / ?ReleaseSurfForColorResolve@) --
    // prepare/release the hardware surface for a color-resolve (EDRAM->texture) operation.
    // boundary (external to this batch); used by d3dDRIVER::CopyBackBufferToTexture.
    void PrepareSurfForColorResolve();
    void ReleaseSurfForColorResolve();

    // Fill the hardware texture header (via the XDK's XGSetTextureHeader) for the given usage /
    // format into pDstTex, returning the packed texture size in bytes and writing pBaseSize /
    // pMipSize when non-null. boundary (external to this batch; called from Resize).
    unsigned int SetXGHeader(unsigned long usage, _D3DFORMAT fmt, D3DBaseTexture *pDstTex,
                             unsigned int *pBaseSize, unsigned int *pMipSize);

    // 0x827BC270 -- reversed in txmD3D_TEX__Resize.cpp. Virtual (UAA). Resize the render target to
    // w x h: stores the new dimensions (truncated to short), re-derives the format packing, rebuilds
    // the XG texture header, re-points the system surface at pData, and recreates the split-screen
    // siblings. Always returns 0.
    unsigned int Resize(unsigned int w, unsigned int h);

    // 0x827BC5B0 -- reversed in txmD3D_TEX__EnableGamma.cpp. Virtual (UAA). Toggle gamma-correct
    // hardware sampling: records the flag in the base, patches the GPU fetch-constant's format
    // fields (when the texture's state/filter bits allow), and re-pushes the sRGB texture state.
    void EnableGamma(int enable);

    // reversed in txmD3D_TEX__LoadDrv.cpp -- upload a decoded pctPICTURE into the driver texture.
    int LoadDrv(pctPICTURE *pPict);
};
