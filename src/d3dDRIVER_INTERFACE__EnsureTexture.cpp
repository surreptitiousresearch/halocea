// ?EnsureTexture@d3dDRIVER_INTERFACE@@UAAXPAVtxmTEXTURE@@HKK@Z  (0x82695FE8)
// D3D override of vidDRIVER_INTERFACE::EnsureTexture. Bind (or unbind) the texture at `stage`,
// skipping the work when the (texture, filter-flags, swizzle, gamma) tuple is unchanged from the
// cached per-stage state. On a change it syncs the sRGB/gamma hardware state, applies an optional
// swizzle override, sets the D3D texture, and pushes the full per-stage sampler state derived from
// the texture's ENGINE_TEX_DESC, the filter flags, and the rendering debug toggles.
//
// The `pTex` argument the decompiler typed as d3dDRIVER_INTERFACE_vtbl* is really a txmTEXTURE*
// (a txmD3D_TEX* at runtime); mangle PAVtxmTEXTURE confirms it. The direct D3DDevice fetch-constant
// / pending-mask register writes for address modes and filters are routed through the source-level
// XDK sampler setters they inline from (see d3d_ensure_texture_boundary.h).
// Source: D:\Projects\code\common\src.sys\drv\video\d3d_8\d3d_txm_8.cpp

#include "headers/d3dDRIVER_INTERFACE.h"
#include "headers/ws/txm/txmD3D_TEX.h"
#include "headers/ws/txm/txmMANAGER.h"      /* txmManager->whiteTex */
#include "headers/d3d_ensure_texture_boundary.h"

// txmTEXTURE::state bits.
enum {
    TXM_STATE_GAMMA_CAPABLE = 0x10,       // bit 4
    TXM_STATE_CREATED       = 0x400000,   // bit 22
    TXM_STATE_MIPS_PACKED   = 0x80,       // bit 7 (mip-map layout requires point mip filter)
    TXM_STATE_SRGB_ON       = 0x8000000,  // bit 27 (hardware sRGB sampling currently enabled)
};
// txmTEXTURE::stateFltr bits.
enum { TXM_FLTR_16BIT_CAPABLE = 0x10000000 }; // bit 28

// fltrFlags bits (per-draw sampler overrides).
enum {
    FLTR_FORCE_POINT_ANISO = 0x1, // force max-anisotropy 1
    FLTR_NO_MIP            = 0x2, // disable mip filtering
    FLTR_NO_FILTER        = 0x4, // point min/mag (no bilinear)
    FLTR_SEPARATE_Z       = 0x8, // separate-Z filter enable
};

// Map an ENGINE_TEX_ADDRESS_MODE (WRAP=0/CLAMP=1/MIRROR=2) to the Xenon sampler address code
// (wrap=0, mirror=1, clamp=2).
static inline unsigned int engine_to_gpu_address(int mode)
{
    return (mode == ETADDR_CLAMP) ? 2u : (unsigned int)(mode == ETADDR_MIRROR);
}

void d3dDRIVER_INTERFACE::EnsureTexture(txmTEXTURE *pTex, int stage, unsigned long fltrFlags,
                                        unsigned long swizzleOverride)
{
    txmD3D_TEX *ptr = (txmD3D_TEX *)pTex;

    unsigned int fltr = (unsigned int)fltrFlags;
    if (!dbg_useFiltration.value)
        fltr |= FLTR_NO_FILTER;

    unsigned short swizzle = 0;
    int gamma = 0;
    if (pTex) {
        swizzle = (swizzleOverride == (unsigned long)-1) ? (unsigned short)pTex->swizzle
                                                         : (unsigned short)swizzleOverride;
        if (this->shaderGamma && vidUseGammaCorrection && (pTex->state.state & TXM_STATE_GAMMA_CAPABLE))
            gamma = 1;
    }

    // Skip if this stage already holds exactly this binding + sampler config.
    if (this->texStage[stage] == pTex
        && this->texStageState[stage].state == fltr
        && this->texStageSwizzle[stage] == swizzle
        && this->texStageGamma[stage] == gamma)
        return;

    this->texStage[stage]            = pTex;
    this->texStageState[stage].state = fltr;
    this->texStageSwizzle[stage]     = swizzle;
    this->texStageGamma[stage]       = gamma;

    // Per-stage GPU pending-register mask bit (XDK GPU fence bookkeeping); passed through the
    // D3DDevice_SetTexture / sampler setters. Value verbatim from the origin's SetTexture call.
    const unsigned __int64 stagePendingMask = 0x80000000ull;

    if (!pTex) {
        D3DDevice_SetTexture(this->pDevice, stage, nullptr, stagePendingMask);
        return;
    }

    // Fall back to the white texture if the real surface hasn't been created yet.
    if (!ptr->GetD3DTex())
        ptr = (txmD3D_TEX *)txmManager->whiteTex.ptr;

    // Sync hardware sRGB/gamma sampling if it disagrees with the requested gamma and the texture
    // is created + gamma-capable.
    unsigned int state = ptr->state.state;
    if (gamma != (int)((state >> 27) & 1)
        && (state & TXM_STATE_CREATED)
        && (state & TXM_STATE_GAMMA_CAPABLE)) {
        if (gamma)
            ptr->state.state |= TXM_STATE_SRGB_ON;
        else
            ptr->state.state &= ~TXM_STATE_SRGB_ON;

        bool srgb = gamma != 0;
        _d3dFormatSetSRGB(&ptr->format, srgb);
        _d3dXenonTextureSetGammaEnable(ptr->GetD3DTex(), srgb);
        if (ptr->stateFltr.state & TXM_FLTR_16BIT_CAPABLE)
            _d3dXenonSet16BitFilterEnable(ptr->GetD3DTex(), srgb);
    }

    // Optional per-draw swizzle override (restored after the SetTexture).
    unsigned int savedSwizzle = (unsigned int)-1;
    if (swizzleOverride != (unsigned long)-1) {
        savedSwizzle = ptr->swizzle;
        ptr->SetSwizzle((unsigned int)swizzleOverride);
    }

    D3DDevice_SetTexture(this->pDevice, stage, ptr->GetD3DTex(), stagePendingMask);

    if (swizzleOverride != (unsigned long)-1)
        ptr->SetSwizzle(savedSwizzle);

    // --- Address modes ---
    D3DDevice_SetSamplerState_AddressU(this->pDevice, stage,
                                       engine_to_gpu_address(ptr->desc.mapping.addressU));
    D3DDevice_SetSamplerState_AddressV(this->pDevice, stage,
                                       engine_to_gpu_address(ptr->desc.mapping.addressV));

    // --- Anisotropy ---
    unsigned int maxAnisotropy = 1;
    if (ptr->desc.mapping.anisotropy <= 0 || dbg_overrideTexAnisotropy.value) {
        if (ptr->desc.usage.maps & 9)
            maxAnisotropy = (unsigned int)dbg_maxAnisotropy.value;
    } else {
        maxAnisotropy = (unsigned int)ptr->desc.mapping.anisotropy;
    }

    // --- Mip filter ---
    unsigned int mipFilter = 2; // default: no mip filtering
    if (ptr->nMipMap > 1 && dbg_useMipMaps.value) {
        if ((ptr->state.state & TXM_STATE_MIPS_PACKED) || !dbg_useMipMapTrilnear.value)
            mipFilter = 0;      // point mip select
        else
            mipFilter = 1;      // trilinear
    }
    unsigned int effectiveMip = (fltr & FLTR_NO_MIP) ? 0 : mipFilter;

    if (fltr & FLTR_FORCE_POINT_ANISO)
        maxAnisotropy = 1;

    // --- Min / mag filter ---
    unsigned int minFilter, magFilter;
    if (fltr & FLTR_NO_FILTER) {
        magFilter = 0; // point
        minFilter = 0;
    } else {
        minFilter = (maxAnisotropy <= 1) ? 1u : 4u; // 1 = linear, 4 = anisotropic
        magFilter = minFilter;
    }

    // --- Per-texture filter overrides (stateFltr) ---
    unsigned int fltrState = ptr->stateFltr.state;
    if (fltrState) {
        if (fltrState & 0x1)        effectiveMip = 2;
        else if (fltrState & 0x2)   effectiveMip = 0;
        else if (fltrState & 0x4)   effectiveMip = 1;

        if (fltrState & 0x8)        minFilter = 0;
        else if (fltrState & 0x10)  minFilter = 1;
        else if (fltrState & 0x20) { minFilter = 4; maxAnisotropy = (unsigned int)ptr->desc.mapping.anisotropy; }

        if (fltrState & 0x40)       magFilter = 0;
        else if (fltrState & 0x80)  magFilter = 1;
    }

    D3DDevice_SetSamplerState_MagFilter(this->pDevice, stage, magFilter);
    D3DDevice_SetSamplerState_MinFilter(this->pDevice, stage, minFilter);
    D3DDevice_SetSamplerState_MipFilter(this->pDevice, stage, effectiveMip);
    D3DDevice_SetSamplerState_MaxAnisotropy(this->pDevice, stage, maxAnisotropy);

    // Separate-Z filter (fltr bit 3): inlined GPU fetch-constant dword[5] write in origin. The bit
    // set CLEARS the enable bits (0x18) and clear SETS them -- so bit3 set => disable.
    D3DDevice_SetSamplerState_SeparateZFilterEnable(this->pDevice, stage, (fltr & FLTR_SEPARATE_Z) == 0);

    if (maxAnisotropy > 1)
        D3DDevice_SetSamplerState_AnisotropyBias(this->pDevice, stage,
                                                 (unsigned int)dbg_AnisotropyBias.value);

    D3DDevice_SetSamplerState_MipMapLodBias(
        this->pDevice, stage,
        (unsigned int)(this->curTexLODBias + ptr->desc.mapping.lod_bias));
}
