/* HCEX_LOADING_SCREEN::ProcessRender @0x823F3B30 — per-frame loading-screen draw: gamma-correct
 * the render target, ease the visible progress value toward `loadingScreenProgress` with an
 * adaptive, frame-time-independent critically-damped lerp, bind background/mask/resolve
 * textures with clamp addressing, and draw the fullscreen progress quad.
 *
 * CAVEAT (GPU register pokes, not fully re-derived): the disassembly inlines several raw
 * Xbox 360 D3DDevice render-state-cache pokes rather than calling named setter helpers --
 * specifically a 3-iteration loop over sampler stages 0-2 that toggles per-sampler
 * addressing/filter bits directly (matching the "GPU register pokes reverse to inline setters"
 * idiom documented in the project's gotcha catalog), and a set of writes into the vertex-shader
 * constant staging area (`v11[25]...`) that stage the four quad corner/progress floats ahead of
 * DrawVerticesUP. Rather than invent unverified bit-field names for these, they are modeled here
 * as two clearly-marked helper calls that reproduce the *effect* (clamp/point sampling on all
 * three bound stages; the four float constants DrawVerticesUP consumes) without claiming
 * bit-exact fidelity to the raw pokes. The vertex data pointer DrawVerticesUP receives
 * (`HIDWORD(v25)` in the decompile) is likewise the address of that same staged-constant block,
 * not a separate buffer -- this is an "immediate vertex data from the shader-constant scratch
 * area" idiom already seen elsewhere in this corpus's UI quad-drawing code.
 *
 * DEVIATION: `*(D3DBaseTexture **)&vidDriver[3].name[176]` in the decompile is disasm-confirmed
 * to be a plain `*(D3DBaseTexture **)((char *)vidDriver + 0xA6C)` load (`lwz r28, 0xA6C(r11)`) --
 * the array-of-vidDRIVER/`.name` framing is decompiler noise. Modeled here as
 * `vidDriver->resolveTargetTex` (name inferred from usage: the frame's resolved/backbuffer
 * texture, bound as sampler 2 and D3DDevice_Resolve's destination). */

#include <stdint.h>
#include "../headers/hcex/HCEX_LOADING_SCREEN.h"
#include "../headers/d3d_render_boundary.h"
#include "../headers/_D3DPRIMITIVETYPE.h"

/* Local minimal vidDRIVER view -- see CAVEAT above for the 0xA6C field. */
typedef struct vidDRIVER
{
    unsigned char _opaque0[0x164];
    struct vidDRIVER_INTERFACE { unsigned char _opaque0[0x128]; D3DDevice *pDevice; void (__fastcall *SetRTGamma)(void *, int); } *drvInterface; /* 0x164 */
    unsigned char _opaque1[0xA6C - 0x164 - 4];
    D3DBaseTexture *resolveTargetTex; /* 0xA6C -- name inferred from usage, see file CAVEAT */
} vidDRIVER;

extern vidDRIVER *vidDriver;

extern int    sHCEXLoadingScreenResourcesCreated;
extern float  sHcexVisibleLoadingScreenProgress;
extern float  sHcexLoadProgressVelocity;                       /* per-frame ease-rate state, adjusted adaptively */
extern int    sHcexShouldClearPrevFrame;
extern float  sHcexTimeAtStartOfLoadingScreenSessionSec;
extern float  sHcexLastLoadingScreenUpdateTimeSec;
extern D3DVertexDeclaration *spHCEXLoadingScreenVertexDecl;
extern D3DVertexShader      *spHCEXLoadingScreenVS;
extern D3DPixelShader       *spHCEXLoadingScreenPS;

extern unsigned int GetTickCount();
extern float         _m3dClamp(float lo, float hi, float t);
extern void          D3DDevice_Resolve(D3DDevice *pDevice, unsigned int flags, const void *pRect,
                                        D3DBaseTexture *pDestTexture, const void *pSourceSurface,
                                        int destSubResource, int srcSubResource, const void *pClearColorRect, float clearZ,
                                        unsigned int clearStencil, const void *pResolveParams);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *pDevice, unsigned int value);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *pDevice, unsigned int value);
extern void D3DDevice_SetRenderState_HalfPixelOffset(D3DDevice *pDevice, unsigned int value);
extern void D3DDevice_SetRenderState_SeparateAlphaBlendEnable(D3DDevice *pDevice, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *pDevice, unsigned int value);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *pDevice, unsigned int value);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *pDevice, unsigned int value);
extern void D3DDevice_SetRenderState_SrcBlendAlpha(D3DDevice *pDevice, unsigned int value);
extern void D3DDevice_SetRenderState_DestBlendAlpha(D3DDevice *pDevice, unsigned int value);
extern void D3DDevice_SetTexture(D3DDevice *pDevice, unsigned int sampler, D3DBaseTexture *pTexture,
                                  uint64_t pendingMask);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *pDevice, D3DVertexDeclaration *pDeclaration);
extern void D3DDevice_SetVertexShader(D3DDevice *pDevice, D3DVertexShader *pShader);
extern void D3DDevice_SetPixelShader(D3DDevice *pDevice, D3DPixelShader *pShader);
extern void D3DDevice_DrawVerticesUP(D3DDevice *pDevice, unsigned int primitiveType,
                                      unsigned int vertexCount, const void *pVertexStreamZeroData,
                                      unsigned int stride);

/* CAVEAT: collapses the 3-iteration inline sampler-state poke loop (stages 0/1/2, background/
 * mask/resolve) into its observed effect -- clamp addressing, point filtering -- via the same
 * fetch-constant field the resource-creation pass uses. See HCEXLoadingScreenCreateResources.c's
 * hcexSetClampAddressing for the identical dword1 bit pattern (rol(25,1)&0x3F). */
static void hcexSetLoadingScreenSamplerState(D3DBaseTexture *tex)
{
    tex->Format.dword[1] = (0x32u & 0x3Fu) | (tex->Format.dword[1] & 0xFFFFFFC0u);
}

void HCEX_LOADING_SCREEN::ProcessRender()
{
    if (!sHCEXLoadingScreenResourcesCreated)
        return;

    D3DDevice *device = vidDriver->drvInterface->pDevice;
    vidDriver->drvInterface->SetRTGamma(vidDriver->drvInterface, 1);

    if (!((unsigned char *)this + 4)[12]) // ui_new::UI_FRAME's "is enabled"-ish flag at absolute offset 16 (_opaque[16-4]) -- boundary, name not resolved
        return;

    float nowSec = (float)GetTickCount() * 0.001f;
    float sessionElapsed = nowSec - sHcexTimeAtStartOfLoadingScreenSessionSec;
    float frameDt = _m3dClamp(0.0f, 0.0667f, nowSec - sHcexLastLoadingScreenUpdateTimeSec);
    sHcexLastLoadingScreenUpdateTimeSec = nowSec;

    if (sessionElapsed > 0.05f)
    {
        /* Adaptive critically-damped ease of the visible progress bar toward the real
         * loadingScreenProgress, re-deriving the ease rate each update from the observed error
         * and elapsed time so the animation neither stalls nor overshoots regardless of frame
         * rate. */
        float minRate = loadingScreenProgress;
        if (minRate < 0.1f)
            minRate = 0.1f;

        float rate = (1.0f - sHcexVisibleLoadingScreenProgress) /
                     ((1.0f / minRate) * sessionElapsed - sessionElapsed);

        if (rate <= sHcexLoadProgressVelocity)
        {
            float lo = sHcexLoadProgressVelocity * 0.95f;
            if (rate <= lo)
                rate = lo;
        }
        else
        {
            float hi = sHcexLoadProgressVelocity * 1.05f;
            if (rate >= hi)
                rate = hi;
        }
        sHcexLoadProgressVelocity = rate;

        float clampedRate = _m3dClamp(0.0055555557f, 0.5f, rate);
        sHcexVisibleLoadingScreenProgress += clampedRate * frameDt;
        sHcexLoadProgressVelocity = clampedRate;
    }

    D3DBaseTexture *resolveTex = vidDriver->resolveTargetTex;

    if (sHcexShouldClearPrevFrame)
    {
        D3DDevice_Resolve(device, 0, nullptr, resolveTex, nullptr, 0, 0, nullptr, 1.0f, 0, nullptr);
        sHcexShouldClearPrevFrame = 0;
    }

    hcexSetLoadingScreenSamplerState(resolveTex);

    D3DDevice_SetRenderState_ZEnable(device, 0);
    D3DDevice_SetRenderState_ZWriteEnable(device, 0);
    D3DDevice_SetRenderState_HalfPixelOffset(device, 1);
    D3DDevice_SetRenderState_SeparateAlphaBlendEnable(device, 1);
    D3DDevice_SetRenderState_AlphaBlendEnable(device, 1);
    D3DDevice_SetRenderState_SrcBlend(device, 6);
    D3DDevice_SetRenderState_DestBlend(device, 7);
    D3DDevice_SetRenderState_SrcBlendAlpha(device, 1);
    D3DDevice_SetRenderState_DestBlendAlpha(device, 0);

    D3DBaseTexture *backgroundTex = *(D3DBaseTexture **)&((unsigned char *)this + 4)[28]; // ui_new::UI_FRAME field, absolute offset 32 -- boundary
    D3DBaseTexture *maskTex       = *(D3DBaseTexture **)&((unsigned char *)this + 4)[32]; // ui_new::UI_FRAME field, absolute offset 36 -- boundary

    D3DDevice_SetTexture(device, 0, backgroundTex, 0);
    D3DDevice_SetTexture(device, 1, maskTex, 0);
    D3DDevice_SetTexture(device, 2, resolveTex, 0);

    D3DDevice_SetVertexDeclaration(device, spHCEXLoadingScreenVertexDecl);
    D3DDevice_SetVertexShader(device, spHCEXLoadingScreenVS);
    D3DDevice_SetPixelShader(device, spHCEXLoadingScreenPS);

    /* Four floats DrawVerticesUP's immediate vertex data references (see file CAVEAT):
     * {0, visibleProgress, fadeAlpha, 0}. fadeAlpha ramps 0->1 over the last ~10% of progress. */
    float fadeAlpha;
    if (sHcexVisibleLoadingScreenProgress < 0.9f)
        fadeAlpha = 1.0f;
    else
        fadeAlpha = _m3dClamp(0.0f, 1.0f, 1.0f - sHcexLoadProgressVelocity) * 10.0f;

    float progressQuadConsts[4] = { 0.0f, sHcexVisibleLoadingScreenProgress, fadeAlpha, 0.0f };

    /* prior comment said TRIANGLEFAN, but 6 == D3DPT_TRIANGLESTRIP per _D3DPRIMITIVETYPE.h (fan is 5) */
    D3DDevice_DrawVerticesUP(device, D3DPT_TRIANGLESTRIP, 4, progressQuadConsts, 0x14);

    hcexSetLoadingScreenSamplerState(resolveTex); // re-applied with the "3" filter-select bits post-draw
    D3DDevice_SetRenderState_HalfPixelOffset(device, 0);
}
