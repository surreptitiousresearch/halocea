#pragma once

#include "d3d_boundary.h"
#include "d3d_render_boundary.h"  /* D3DSurface */
#include "vidDRIVER.h"
#include "_D3DADAPTER_IDENTIFIER9.h"
#include "_XGSTEREOPARAMETERS.h"
#include "XENON_RT.h"
#include "ws/ds/dsVECTOR.h"

#include "vidPAGE_MEMORY_ALLOCATOR.h" // pAllocator target (full layout used by TermHW)

/* Pointer-only dependencies (boundary — not decompiled here). */
typedef struct txmD3D_TEX txmD3D_TEX;
typedef struct d3dOCCLUSION_QUERY d3dOCCLUSION_QUERY;
typedef struct pctPICTURE pctPICTURE;
typedef struct d3dDRIVER_vtbl d3dDRIVER_vtbl;

/* d3dDRIVER is the engine's D3D wrapper class (boundary — not decompiled).
   Full DB layout (types_members d3dDRIVER, 2700 bytes); derives from vidDRIVER,
   spelled base-as-first-member per the corpus convention. */
typedef struct d3dDRIVER
{
    vidDRIVER base;                              /* 0x000 — vidDRIVER baseclass */
    int adapterNmb;                              /* 0x33C (828) */
    _D3DADAPTER_IDENTIFIER9 adapterIdentifier;   /* 0x340 (832) */
    _D3DDEVTYPE deviceType;                      /* 0x790 (1936) */
    _D3DCAPS9 caps;                              /* 0x794 (1940) */
    _D3DMULTISAMPLE_TYPE multiSampleType;        /* 0x8C4 (2244) */
    _D3DPRESENT_PARAMETERS_ presentParam;        /* 0x8C8 (2248) */
    Direct3D *pD3D;                              /* 0x944 (2372) — IDirect3D9 */
    D3DDevice *pDevice;                          /* 0x948 (2376) — IDirect3DDevice9 */
    D3DSurface *pBackBuffer;                     /* 0x94C (2380) */
    D3DSurface *pDepthBuffer;                    /* 0x950 (2384) — current depth/stencil surface */
    txmD3D_TEX *pDepthBufTex;                    /* 0x954 (2388) */
    _XGSTEREOPARAMETERS stereoParams;            /* 0x958 (2392) */
    int stereoCompatible;                        /* 0xA50 (2640) */
    int enablePWLGamma;                          /* 0xA54 (2644) */
    dsVECTOR<XENON_RT,8> RTPool;                 /* 0xA58 (2648) */
    D3DTexture *m_pFrontBufferTexture[2];        /* 0xA6C (2668) */
    vidPAGE_MEMORY_ALLOCATOR *pAllocator;        /* 0xA74 (2676) */
    dsVECTOR<d3dOCCLUSION_QUERY *,8> queryList;  /* 0xA78 (2680) */

    // 0x82695BBC -- reversed in d3dDRIVER__SetStartRenderingState.c.
    void SetStartRenderingState();
    // 0x82696538 -- reversed in d3dDRIVER__ConvertPWL2ProperGamma.c.
    float ConvertPWL2ProperGamma(double C) const;

    // 0x8268E518 -- reversed in d3dDRIVER__SetViewport.cpp. Virtual. Set the D3D viewport under the
    // video lock (only while the device-ready state bit 0x04000000 is set); a non-positive width or
    // height selects the full render-mode viewport with the default 0..1 depth range.
    void SetViewport(int x, int y, int width, int height, float minZ, float maxZ);

    // 0x8268E698 -- reversed in d3dDRIVER__CopyBackBufferToTexture.cpp. Virtual. Resolve the current
    // color render target (back buffer) into `pTex` over the given rect (or the whole texture when
    // width/height == -1). Returns 1 on success, 0 when the device isn't ready or the target is
    // unusable.
    int CopyBackBufferToTexture(txmD3D_TEX *pTex, int x, int y, int width, int height);

    // 0x8268F140 -- reversed in d3dDRIVER__ctor.cpp. Chains vidDRIVER(56); zeroes the D3D
    // caps/present-param blocks and device/surface pointers; enables PWL gamma.
    d3dDRIVER();
    // 0x8268F228 -- reversed in d3dDRIVER__dtor.cpp. Virtual. TermDrv, free the two vectors,
    // chain ~vidDRIVER.
    ~d3dDRIVER();

    // 0x82690EF0 -- reversed in d3dDRIVER__InitHW.cpp. Virtual. Bring up the Xenon device
    // (InitHWxenon), chain vidDRIVER::InitHW, seed render state + queries; sets ready bit 0x04000000.
    int InitHW();
    // 0x82691010 -- reversed in d3dDRIVER__ResetHW.cpp. Virtual. Re-run InitHWxenon.
    int ResetHW();
    // 0x82691730 -- reversed in d3dDRIVER__TermHW.cpp. Virtual. Tear down the device: release
    // queries, RT pool, front-buffer textures, com-buffer interfaces, page allocator, buffers.
    void TermHW();

    // --- boundary callees (deeper than this wave's 2-level descent; extern) ---
    void GpuTimeQueryInit();   // 0x8268... GPU timing query setup (boundary)
    void TermDrv();            // driver-object teardown (boundary)
    int  InitHWxenon();        // Xenon D3D device creation (boundary)
    void RecreateQueries();    // occlusion-query (re)creation (boundary)
    void ReleaseQueries();     // occlusion-query release (boundary)
    void XenonClearRTPool();   // release pooled render targets (boundary)
} d3dDRIVER; /* 2700 bytes */

extern d3dDRIVER *d3dDriver;

/* d3dDRIVER vtable instance emitted by the compiler (boundary). */
extern d3dDRIVER_vtbl d3dDRIVER_vftable;
