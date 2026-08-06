#pragma once

/* Direct3D boundary types. These are the Xbox 360 D3D SDK types; we do NOT
   reverse them — they are treated as external library types per session scope.
   Only the fields actually touched by the rasterizer are declared.

   Split pass: the individual D3D SDK types now live one-per-file (see #includes below);
   this header aggregates them plus the extern SDK wrapper declarations. */

typedef long HRESULT;

/* D3D SDK enum types (external boundary; 4-byte enums modeled as unsigned int). */
typedef unsigned int _D3DRENDERSTATETYPE;
typedef unsigned int _D3DSAMPLERSTATETYPE;

#include <stdint.h>
#include "_D3DDEVTYPE.h"
#include "_D3DFORMAT.h"
#include "D3DDevice.h"
#include "tagRECT.h"
#include "D3DVIEWPORT9.h"
#include "D3DSURFACE_DESC.h"
#include "D3DLOCKED_RECT.h"
#include "D3DLOCKED_BOX.h"
#include "D3DBOX.h"
#include "_D3DCAPS9.h"

typedef struct Direct3D              Direct3D;
typedef struct D3DVertexShader       D3DVertexShader;
typedef struct D3DVertexDeclaration  D3DVertexDeclaration;
typedef struct D3DPixelShader        D3DPixelShader;
typedef struct D3DIndexBuffer        D3DIndexBuffer;
typedef struct D3DVertexBuffer       D3DVertexBuffer;
typedef struct D3DTexture            D3DTexture;
typedef struct D3DSurface            D3DSurface;
typedef struct D3DCubeTexture        D3DCubeTexture;
typedef struct D3DVolumeTexture      D3DVolumeTexture;
typedef struct D3DQuery              D3DQuery;

typedef unsigned int D3DCUBEMAP_FACES;

typedef struct D3DResource D3DResource;

#include "_D3DMULTISAMPLE_TYPE.h"
#include "_D3DSWAPEFFECT.h"
#include "_D3DCOLORSPACE.h"
#include "_D3DRING_BUFFER_PARAMETERS.h"
#include "_D3DVIDEO_SCALER_PARAMETERS.h"
#include "HWND__.h"

/* D3DPRESENT_PARAMETERS — full Xbox 360 layout, DB-verified (types_members D3DPRESENT_PARAMETERS,
   identical to _D3DPRESENT_PARAMETERS_). Left inline (orphan): no dedicated home file in this
   split pass. */
typedef struct D3DPRESENT_PARAMETERS
{
    unsigned int BackBufferWidth;                  /* 0x00 */
    unsigned int BackBufferHeight;                 /* 0x04 */
    _D3DFORMAT BackBufferFormat;                   /* 0x08 */
    unsigned int BackBufferCount;                  /* 0x0C */
    _D3DMULTISAMPLE_TYPE MultiSampleType;          /* 0x10 */
    unsigned int MultiSampleQuality;               /* 0x14 */
    _D3DSWAPEFFECT SwapEffect;                     /* 0x18 */
    HWND__ *hDeviceWindow;                         /* 0x1C */
    int Windowed;                                  /* 0x20 */
    int EnableAutoDepthStencil;                    /* 0x24 */
    _D3DFORMAT AutoDepthStencilFormat;             /* 0x28 */
    unsigned int Flags;                            /* 0x2C */
    unsigned int FullScreen_RefreshRateInHz;       /* 0x30 (48) */
    unsigned int PresentationInterval;             /* 0x34 (52) */
    int DisableAutoBackBuffer;                     /* 0x38 */
    int DisableAutoFrontBuffer;                    /* 0x3C */
    _D3DFORMAT FrontBufferFormat;                  /* 0x40 */
    _D3DCOLORSPACE FrontBufferColorSpace;          /* 0x44 */
    _D3DRING_BUFFER_PARAMETERS RingBufferParameters;   /* 0x48 */
    _D3DVIDEO_SCALER_PARAMETERS VideoScalerParameters; /* 0x60 */
} D3DPRESENT_PARAMETERS; /* 124 bytes */
typedef D3DPRESENT_PARAMETERS _D3DPRESENT_PARAMETERS_; /* DB struct-tag alias */

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned int D3DResource_Release(D3DResource *resource); /* DB funcs: unsigned int __fastcall D3DResource_Release(D3DResource*) */

extern HRESULT Direct3D_GetDeviceCaps(unsigned int Adapter, _D3DDEVTYPE DeviceType, _D3DCAPS9 *pCaps);

/* Xbox 360 D3D render-state wrappers */
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *pDevice, unsigned int Value);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *pDevice, unsigned int Value);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *pDevice, unsigned int Value);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *pDevice, unsigned int Value);
extern void D3DDevice_SetRenderState_AlphaRef(D3DDevice *pDevice, unsigned int Value);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *pDevice, unsigned int Value);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *pDevice, unsigned int Value);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *pDevice, unsigned int Value);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *pDevice, unsigned int Value);
extern void D3DDevice_SetRenderState_ZFunc(D3DDevice *pDevice, unsigned int Value);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *pDevice, unsigned int Value);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *pDevice, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_SetRenderState_AlphaFunc(D3DDevice *pDevice, unsigned int Value);
extern void D3DDevice_SetRenderState_FillMode(D3DDevice *pDevice, unsigned int Value);

/* Xbox 360 XDK SDK function (external library boundary, not decompiled). Computes/queries the
   tiled-texture header layout for a 2D texture; only the pBaseSize out-param is consumed by
   txmCalcTexureSizeHW (the rest -- BaseOffset/MipOffset/pTexture/pMipSize -- are passed as
   0/nullptr there). Return value is the base-size in bytes per that call site's usage. */
extern unsigned int XGSetTextureHeader(unsigned int Width, unsigned int Height, unsigned int Levels,
        unsigned int Usage, _D3DFORMAT Format, unsigned int ExpBias, unsigned int Flags,
        unsigned int BaseOffset, unsigned int MipOffset, D3DTexture *pTexture,
        unsigned int *pBaseSize, unsigned int *pMipSize);
#ifdef __cplusplus
}
#endif
