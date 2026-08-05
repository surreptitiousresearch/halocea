#pragma once
/* vidDRIVER_INTERFACE — ws-engine video-driver interface state. DB-verified full layout
 * (types_members vidDRIVER_INTERFACE, total size 296 / 0x128). Split out of
 * vidDRIVER_DYNGEOM_BUFFER.h, which previously modeled it as an opaque[0x124]+dynGeomBuffer slice. */

#include <stdint.h>
#include "ws/ap/apSTATE_T.h"   /* apSTATE_T<unsigned long> texStageState[19] (embedded by value) */
#include "ws/txm/txmTEXTURE.h" /* txmTEXTURE *texStage[19] (typed pointee) */

typedef struct vidDRIVER_DYNGEOM_BUFFER vidDRIVER_DYNGEOM_BUFFER;

/* forward decls for vtbl slot signatures (all boundary, pointer-only) */
struct vidSHADER;
struct vidFVF_DESCR;
struct vidSTREAM;
struct objOBJ;
struct vidCOMMAND_BUFFER;
struct rnsRENDERBUF_ENTRY;
struct m3dV;
struct vidOBJ_DIP;
struct vidPASS_RENDERSTATE;
struct vidSTREAM_CACHE;
struct m3dMATR;
struct objBONES_INFO;
struct m3dMATR34;
struct instSKIN_BONE_INFO;
struct vidCMD_BUF_ENV;

/* DB-verified (types_members vidDRIVER_INTERFACE_vtbl, 59 slots / 236 bytes). Nested-enum
 * parameters (vidPASS_RENDERSTATE::BLEND_RGB/BLEND_A/CULLMODE/Z_FUNC) are carried as int here. */
typedef struct vidDRIVER_INTERFACE_vtbl
{
    void  (*dtr_vidDRIVER_INTERFACE)(struct vidDRIVER_INTERFACE *self, int freeMemory); /* 0x00 deleting dtor (deviation: DB sig has no flag arg; call sites pass it) */
    void  (*InitHW)(struct vidDRIVER_INTERFACE *self);                                  /* 0x04 */
    void  (*TermHW)(struct vidDRIVER_INTERFACE *self);                                  /* 0x08 */
    void  (*EnsureVertShaderConst)(struct vidDRIVER_INTERFACE *self, int, const float *, int);          /* 0x0C */
    float *(*EnsureVertShaderConstStart)(struct vidDRIVER_INTERFACE *self, int, int);                   /* 0x10 */
    void  (*EnsureVertShaderConstEnd)(struct vidDRIVER_INTERFACE *self);                                /* 0x14 */
    void  (*EnsureVertShaderConstDirect)(struct vidDRIVER_INTERFACE *self, int, int, const float **, int *); /* 0x18 */
    void  (*EnsurePixShaderConst)(struct vidDRIVER_INTERFACE *self, int, const float *, int);           /* 0x1C */
    float *(*EnsurePixShaderConstStart)(struct vidDRIVER_INTERFACE *self, int, int);                    /* 0x20 */
    void  (*EnsurePixShaderConstEnd)(struct vidDRIVER_INTERFACE *self);                                 /* 0x24 */
    void  (*ConfigureAlphaBlend)(struct vidDRIVER_INTERFACE *self, int blendRGB, int blendA);           /* 0x28 */
    void  (*ConfigureCullMode)(struct vidDRIVER_INTERFACE *self, int cullMode);                         /* 0x2C */
    void  (*ConfigureBackBufferWrite)(struct vidDRIVER_INTERFACE *self, int, int, int, int);            /* 0x30 */
    void  (*ConfigureZ)(struct vidDRIVER_INTERFACE *self, int, int, int zFunc);                         /* 0x34 */
    void  (*ConfigureDepthBias)(struct vidDRIVER_INTERFACE *self, float, float);                        /* 0x38 */
    void  (*ConfigureFillMode)(struct vidDRIVER_INTERFACE *self, int);                                  /* 0x3C */
    void  (*ConfigureStencil)(struct vidDRIVER_INTERFACE *self, int, unsigned int, int, int, int);      /* 0x40 */
    void  (*ConfigureStencilCCW)(struct vidDRIVER_INTERFACE *self, int, unsigned int, int, int, int);   /* 0x44 */
    void  (*SetGPRAllocation)(struct vidDRIVER_INTERFACE *self, int, int);                              /* 0x48 */
    void  (*SetGammaCorrection)(struct vidDRIVER_INTERFACE *self, bool);                                /* 0x4C */
    bool  (*EnsureVertShader)(struct vidDRIVER_INTERFACE *self, vidSHADER *);                           /* 0x50 */
    bool  (*EnsurePixShader)(struct vidDRIVER_INTERFACE *self, vidSHADER *, bool);                      /* 0x54 */
    void  (*EnsureTexture)(struct vidDRIVER_INTERFACE *self, txmTEXTURE *, int, unsigned int, unsigned int); /* 0x58 */
    void  (*EnsureShaderDecl)(struct vidDRIVER_INTERFACE *self, vidFVF_DESCR *, int, int);              /* 0x5C */
    void  (*EnsureStream)(struct vidDRIVER_INTERFACE *self, int, const vidSTREAM *);                    /* 0x60 */
    void  (*RestartZPrepassObjConditionalSurvey)(struct vidDRIVER_INTERFACE *self);                     /* 0x64 */
    void  (*BeginZPrepassObjConditionalSurvey)(struct vidDRIVER_INTERFACE *self);                       /* 0x68 */
    void  (*EndZPrepassObjConditionalSurvey)(struct vidDRIVER_INTERFACE *self, objOBJ *, unsigned int *); /* 0x6C */
    void  (*ProcessObjConditionalSurveyResults)(struct vidDRIVER_INTERFACE *self);                      /* 0x70 */
    void  (*ClearObjConditionalSurveyResults)(struct vidDRIVER_INTERFACE *self);                        /* 0x74 */
    void  (*QueryBatchRelease)(struct vidDRIVER_INTERFACE *self);                                       /* 0x78 */
    void  (*QueryBatchCreate)(struct vidDRIVER_INTERFACE *self);                                        /* 0x7C */
    void  (*BeginConditionalSurvey)(struct vidDRIVER_INTERFACE *self, unsigned int, bool);              /* 0x80 */
    void  (*EndConditionalSurvey)(struct vidDRIVER_INTERFACE *self);                                    /* 0x84 */
    void  (*BeginConditionalRendering)(struct vidDRIVER_INTERFACE *self, unsigned int);                 /* 0x88 */
    void  (*EndConditionalRendering)(struct vidDRIVER_INTERFACE *self);                                 /* 0x8C */
    void  (*Invalidate)(struct vidDRIVER_INTERFACE *self);                                              /* 0x90 */
    void  (*ReleaseThreadOwnership)(struct vidDRIVER_INTERFACE *self);                                  /* 0x94 */
    void  (*AcquireThreadOwnership)(struct vidDRIVER_INTERFACE *self);                                  /* 0x98 */
    bool  (*StartRecord)(struct vidDRIVER_INTERFACE *self, vidCOMMAND_BUFFER *);                        /* 0x9C */
    void  (*StopRecord)(struct vidDRIVER_INTERFACE *self);                                              /* 0xA0 */
    void  (*PlayRecord)(struct vidDRIVER_INTERFACE *self, vidCOMMAND_BUFFER *);                         /* 0xA4 */
    void  (*ObjDrawIndexedPrimitivePrepare)(struct vidDRIVER_INTERFACE *self, const rnsRENDERBUF_ENTRY *, const m3dV *); /* 0xA8 */
    void  (*ObjDrawIndexedPrimitive)(struct vidDRIVER_INTERFACE *self, const vidOBJ_DIP *);             /* 0xAC */
    void  (*EnsureRenderState)(struct vidDRIVER_INTERFACE *self, const vidPASS_RENDERSTATE *);          /* 0xB0 */
    void  (*EnsureStreamSetup)(struct vidDRIVER_INTERFACE *self, vidSTREAM_CACHE *);                    /* 0xB4 */
    void  (*SetFVFPackParams)(struct vidDRIVER_INTERFACE *self, int16_t *, int16_t *, float *);         /* 0xB8 */
    void  (*SetTexCompressionOffset)(struct vidDRIVER_INTERFACE *self, objOBJ *, uint16_t *);   /* 0xBC */
    void  (*SetTransform)(struct vidDRIVER_INTERFACE *self, const m3dMATR *);                           /* 0xC0 */
    void  (*SetTransformSkinBoneIndex)(struct vidDRIVER_INTERFACE *self, objBONES_INFO *, uint16_t, int); /* 0xC4 */
    void  (*SetTransformSkinData)(struct vidDRIVER_INTERFACE *self, m3dMATR34 *, instSKIN_BONE_INFO *, objBONES_INFO *, int); /* 0xC8 */
    void  (*ConfigureAlphaTestValue)(struct vidDRIVER_INTERFACE *self, uint8_t);                /* 0xCC */
    void  (*ClearSource)(struct vidDRIVER_INTERFACE *self);                                             /* 0xD0 */
    const vidCMD_BUF_ENV *(*GetCmdBufEnv)(struct vidDRIVER_INTERFACE *self);                            /* 0xD4 */
    vidCMD_BUF_ENV *(*GetCmdBufEnv_2)(struct vidDRIVER_INTERFACE *self);                                /* 0xD8 */
    void  (*InvalidateGammaCorrection)(struct vidDRIVER_INTERFACE *self);                               /* 0xDC */
    void  (*SetRTGamma)(struct vidDRIVER_INTERFACE *self, bool);                                        /* 0xE0 */
    bool  (*GetRTGamma)(struct vidDRIVER_INTERFACE *self);                                              /* 0xE4 */
    void  (*ValidateGammaCorrection)(struct vidDRIVER_INTERFACE *self);                                 /* 0xE8 */
} vidDRIVER_INTERFACE_vtbl; /* 236 bytes */

typedef struct vidDRIVER_INTERFACE
{
    vidDRIVER_INTERFACE_vtbl   *__vftable;                /* 0x000 */
    txmTEXTURE                 *texStage[19];             /* 0x004 */
    apSTATE_T<unsigned long>    texStageState[19];        /* 0x050 */
    uint16_t            texStageSwizzle[19];      /* 0x09C */
    unsigned char _pad0[2]; /* db-verified padding */
    int                         texStageGamma[19];        /* 0x0C4 */
    bool                        shaderGamma;              /* 0x110 */
    unsigned char _pad1[3]; /* db-verified padding */
    /* 3 bytes padding */
    int                         curGamma;                 /* 0x114 */
    float                       curTexLODBias;            /* 0x118 */
    bool                        isIdentityMatr;           /* 0x11C */
    unsigned char _pad2[3]; /* db-verified padding */
    /* 3 bytes padding */
    int                         acquiredThreadProcessor;  /* 0x120 */
    vidDRIVER_DYNGEOM_BUFFER    *dynGeomBuffer;           /* 0x124 */

    // vidDRIVER_INTERFACE_vtbl virtuals (slots 0x50/0x54/0x5C/0x90/0xD0) driven at end-of-frame by
    // vidDRIVER::EndScene: force the bound vertex/pixel shader and shader-input declaration (null =
    // unbind), invalidate the cached device state, and clear the current vertex/index source
    // bindings. Argument shapes are the null/zero end-of-frame reset calls; boundary (external to
    // this batch).
    void EnsureVertShader(void *pShader);
    void EnsurePixShader(void *pShader, int flags);
    void EnsureShaderDecl(void *pDecl, int a2, int a3);
    void Invalidate();

    // Device-interface hardware bring-up / teardown (vtbl slots 0x04 / 0x08). Declared non-virtual
    // here to preserve the DB manual-__vftable layout, matching this header's other virtuals; driven
    // via base.drvInterface by d3dDRIVER::TermHW. DB ?InitHW@vidDRIVER_INTERFACE@@UAAXXZ /
    // ?TermHW@vidDRIVER_INTERFACE@@UAAXXZ. boundary (2026-07-31)
    void InitHW();
    void TermHW();

    // 0x82666DC0 -- reversed in vidDRIVER_INTERFACE__ClearSource.cpp. Unbinds all 19 texture
    // stages by calling EnsureTexture(nullptr, stage, 0, -1) for each.
    void ClearSource();

    // Bind (or, with a null texture, unbind) the texture at the given stage. Driven by
    // ClearSource; boundary (external to this batch).
    void EnsureTexture(txmTEXTURE *pTex, int stage, int a3, unsigned int a4);

    // Thread-ownership hooks forwarded by vidDRIVER::Acquire/ReleaseThreadOwnership.
    // boundary (external to this batch).
    void AcquireThreadOwnership();
    void ReleaseThreadOwnership();

    // Enable/disable hardware gamma correction on the driver interface. boundary (external).
    void SetGammaCorrection(int enable);

    // Re-apply the shared baseline render state at scene start. Called by
    // d3dDRIVER::SetStartRenderingState via base.drvInterface; boundary (external to this batch).
    void SetStartRenderingState();

    // vidDRIVER_INTERFACE_vtbl slots — push `count` float4 constants starting at `startReg` into
    // the vertex/pixel shader constant banks (DB: virtual, overridden by d3dDRIVER_INTERFACE
    // ?EnsureVertShaderConst@d3dDRIVER_INTERFACE@@UAAXHPBMH@Z). boundary (external).
    void EnsureVertShaderConst(unsigned int startReg, const float *data, unsigned int flags);
    void EnsurePixShaderConst(unsigned int startReg, const float *data, unsigned int flags);
} vidDRIVER_INTERFACE; /* 296 bytes */
