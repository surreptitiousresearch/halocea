#pragma once

/* Boundary surface for d3dDRIVER_INTERFACE::EnsureTexture
 * (src/d3dDRIVER_INTERFACE__EnsureTexture.cpp): the Xbox 360 D3D sampler-state setters, the
 * Xenon gamma/format helpers, and the rendering debug toggles. Several of the sampler setters
 * are emitted inline (direct GPU fetch-constant / pending-mask register writes) in the shipped
 * binary; they are declared here as the source-level XDK wrappers they inline from. */

#include "d3d_boundary.h"                /* D3DDevice */
#include "D3DBaseTexture.h"             /* D3DBaseTexture, GPUTEXTURE_FETCH_CONSTANT */
#include "_D3DFORMAT.h"
#include "ws/wb/dbgVAR_boundary.h"       /* dbgVAR_IMPL<T,N> (.value) */

/* --- rendering debug toggles (dbgVAR_IMPL<T,N>, value @0x0C). boundary. --- */
extern "C" dbgVAR_IMPL<bool, 1>  dbg_useFiltration;
extern "C" dbgVAR_IMPL<bool, 1>  dbg_useMipMaps;
extern "C" dbgVAR_IMPL<bool, 1>  dbg_useMipMapTrilnear;
extern "C" dbgVAR_IMPL<bool, 1>  dbg_overrideTexAnisotropy;
extern "C" dbgVAR_IMPL<int, 2>   dbg_maxAnisotropy;
extern "C" dbgVAR_IMPL<float, 3> dbg_AnisotropyBias;

/* Global gamma-correction enable (nonzero => hardware sRGB sampling is available). boundary. */
extern int vidUseGammaCorrection;

/* --- Xenon texture gamma / format helpers (inlined GPU-register pokes in origin). boundary. --- */
extern void _d3dFormatSetSRGB(_D3DFORMAT *fmt, bool srgb);
extern void _d3dXenonTextureSetGammaEnable(D3DBaseTexture *tex, bool enable);
extern void _d3dXenonSet16BitFilterEnable(D3DBaseTexture *tex, bool enable);

/* --- D3D sampler-state setters (Xbox 360 XDK). Those marked "inlined in origin" are register
 * writes the compiler folded into EnsureTexture; declared here as their source-level form. --- */
extern "C" void D3DDevice_SetSamplerState_MagFilter(D3DDevice *dev, unsigned int stage, unsigned int filter);
extern "C" void D3DDevice_SetSamplerState_MinFilter(D3DDevice *dev, unsigned int stage, unsigned int filter);
extern "C" void D3DDevice_SetSamplerState_MipFilter(D3DDevice *dev, unsigned int stage, unsigned int filter);   /* inlined in origin */
extern "C" void D3DDevice_SetSamplerState_AddressU(D3DDevice *dev, unsigned int stage, unsigned int mode);   /* inlined in origin */
extern "C" void D3DDevice_SetSamplerState_AddressV(D3DDevice *dev, unsigned int stage, unsigned int mode);   /* inlined in origin */
extern "C" void D3DDevice_SetSamplerState_MaxAnisotropy(D3DDevice *dev, unsigned int stage, unsigned int maxAniso);
extern "C" void D3DDevice_SetSamplerState_AnisotropyBias(D3DDevice *dev, unsigned int stage, unsigned int bias);
extern "C" void D3DDevice_SetSamplerState_MipMapLodBias(D3DDevice *dev, unsigned int stage, unsigned int bias);
extern "C" void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *dev, unsigned int stage, int enable); /* inlined in origin (v9 bit3) */
