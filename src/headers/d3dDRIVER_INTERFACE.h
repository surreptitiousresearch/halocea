#pragma once
#include "d3d_boundary.h"        // D3DDevice
#include "vidDRIVER_INTERFACE.h" // base class (296 bytes, DB-verified in its own header)
#include "ws/ds/dsVECTOR.h"      // dsVECTOR<d3dCOND_SURVEY_RES,8> conditionalSurveyResults
#include "ws/ds/dsCONST_ARRAY.h" // dsCONST_ARRAY<bool,64> condRenderDbgCheck
#include "d3dCOND_SURVEY_RES.h"
#include "_D3DTAGCOLLECTION.h"
#include "d3dCMD_BUF_ENV.h"

/* d3dDRIVER_INTERFACE — the D3D implementation of vidDRIVER_INTERFACE. Full DB layout
 * (types_members d3dDRIVER_INTERFACE, 732 bytes: base vidDRIVER_INTERFACE@0 (296),
 * pDevice@296 ... condSurveyDbgCheck@728). */

typedef struct d3dCOMMAND_BUFFER       d3dCOMMAND_BUFFER;
typedef struct D3DTexture              D3DTexture;
typedef struct D3DVertexShader         D3DVertexShader;
typedef struct D3DPixelShader          D3DPixelShader;
typedef struct D3DVertexBuffer         D3DVertexBuffer;
typedef struct D3DOcclusionQueryBatch  D3DOcclusionQueryBatch;
typedef struct D3DIndexBuffer          D3DIndexBuffer;
typedef struct vidFVF_DESCR            vidFVF_DESCR;

/* types_members d3dDRIVER_INTERFACE::d3dSTR — one cached vertex-stream binding (12 bytes).
 * DB nested type; defined at file scope here so it can be referenced by name. */
typedef struct d3dSTR
{
    D3DVertexBuffer *buf;    /* 0x00 */
    int              offset; /* 0x04 */
    int              stride; /* 0x08 */
} d3dSTR;

struct d3dDRIVER_INTERFACE : vidDRIVER_INTERFACE
{
    D3DDevice                     *pDevice;                  /* 0x128 (296) */
    bool                           isCommBuffDevice;         /* 0x12C (300) */
    unsigned char                  _pad12D[3];               /* 0x12D */
    d3dCOMMAND_BUFFER             *curCmdBuf;                /* 0x130 (304) */
    D3DTexture                    *localTexCopy;             /* 0x134 (308) */
    D3DVertexShader               *pShaderVert;              /* 0x138 (312) */
    D3DPixelShader                *pShaderPix;               /* 0x13C (316) */
    vidFVF_DESCR                  *pFVFDescr;                /* 0x140 (320) */
    d3dSTR                         streamCache[16];          /* 0x144 (324) */
    bool                           instancedDecl;            /* 0x204 (516) */
    unsigned char                  _pad205[3];               /* 0x205 */
    dsVECTOR<d3dCOND_SURVEY_RES,8> conditionalSurveyResults; /* 0x208 (520) */
    D3DOcclusionQueryBatch        *occlusionQueryBatch;      /* 0x21C (540) */
    D3DIndexBuffer                *pIBuf;                    /* 0x220 (544) */
    int                            vsGPRCount;               /* 0x224 (548) */
    int                            psGPRCount;               /* 0x228 (552) */
    _D3DTAGCOLLECTION              inherit;                  /* 0x22C (556) */
    d3dCMD_BUF_ENV                 cmdBufEnv;                /* 0x254 (596) */
    dsCONST_ARRAY<bool,64>         condRenderDbgCheck;       /* 0x294 (660) */
    bool                           condSurveyDbgCheck;       /* 0x2D8 (728) */
    unsigned char                  _pad2D9[3];               /* 0x2D9 */

    // 0x826942CC -- reversed in d3dDRIVER_INTERFACE__SetStartRenderingState.c. Establishes the
    // baseline D3D render state (Z test/write on, LessEqual Z func, alpha test off with a
    // NotEqual func left armed, no culling, solid fill).
    void SetStartRenderingState();
    // vidDRIVER_INTERFACE_vtbl slots 3/7 — push `count` float4 constants starting at `startReg`
    // into the vertex/pixel shader constant banks. boundary.
    void EnsureVertShaderConst(unsigned int startReg, const float *data, unsigned int flags);
    void EnsurePixShaderConst(unsigned int startReg, const float *data, unsigned int flags);

    // ?UpdateTextureStateSRGB@d3dDRIVER_INTERFACE@@QAAXPAVtxmTEXTURE@@@Z -- re-push the sRGB /
    // gamma sampler state for `pTex` into any texture stage currently bound to it. boundary
    // (external to this batch; called from txmD3D_TEX::EnableGamma).
    void UpdateTextureStateSRGB(struct txmTEXTURE *pTex);

    // 0x82695FE8 (?EnsureTexture@d3dDRIVER_INTERFACE@@UAAXPAVtxmTEXTURE@@HKK@Z, virtual UAA) --
    // reversed in d3dDRIVER_INTERFACE__EnsureTexture.cpp. The D3D override of
    // vidDRIVER_INTERFACE::EnsureTexture: binds (or, with pTex==nullptr, unbinds) the texture at
    // `stage`, caching the bind so redundant state changes are skipped; on a real change it syncs
    // sRGB/gamma, applies an optional swizzle override, sets the D3D texture, and pushes the full
    // per-stage sampler state (address modes, min/mag/mip filters, anisotropy, LOD bias) derived
    // from the texture's ENGINE_TEX_DESC, the filter flags, and the rendering debug toggles.
    void EnsureTexture(struct txmTEXTURE *pTex, int stage, unsigned long fltrFlags,
                       unsigned long swizzleOverride);

    // 0x826948B8 -- reversed in d3dDRIVER_INTERFACE__EnsureVertShader.cpp. Virtual. Bind the given
    // vertex shader if it differs from the currently cached one; return true iff a change was made.
    bool EnsureVertShader(D3DVertexShader *pShaderVert);

    // 0x82694910 -- reversed in d3dDRIVER_INTERFACE__EnsurePixShader.cpp. Virtual. Bind the given
    // pixel shader (or null when `isEmptyShader`) if it differs from the cached one; return true
    // iff a change was made. NOTE: the DB/disasm show the argument as the concrete D3DPixelShader*
    // (compared directly against the cached pShaderPix), not the vidSHADER* the batch prompt named.
    bool EnsurePixShader(D3DPixelShader *pShaderPix, bool isEmptyShader);

    // 0x82694898 -- reversed in d3dDRIVER_INTERFACE__EnsureShaderDecl.cpp. Virtual. Cache `pDescr`
    // and set the D3D vertex declaration it owns (stored at pDescr+0x94), or clear it when null.
    // NOTE: the freqVert/freqFace stream-frequency args exist in the vtable signature but are
    // unreferenced in this build's body.
    void EnsureShaderDecl(vidFVF_DESCR *pDescr, int freqVert, int freqFace);
}; /* 732 bytes */

typedef struct d3dDRIVER_INTERFACE d3dDRIVER_INTERFACE;
