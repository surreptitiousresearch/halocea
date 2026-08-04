#pragma once
#include "vidMODE.h"                 // vidMODE *pMode / modeList
#include "vidDRIVER_INTERFACE.h"     // vidDRIVER_INTERFACE *drvInterface
#include "ws/ap/apSTATE_T.h"
#include "ws/ds/dsVECTOR.h"
#include "ws/ds/dsCONST_ARRAY.h"
#include "ws/m3d/m3dCOLOR.h"
#include "ws/vid/vidCONFIG.h"
#include "ws/vid/vidPASS_PLATFORM.h"
#include "ws/vid/vidDIP.h"
#include "ws/vid/VID_TQ.h"

// vidDRIVER -- ws-engine video driver base. Full DB-verified layout (types_members
// vidDRIVER, size 828) and its by-value support records (vidRT_STACK_ENTRY, vidCLEAR,
// vidGAMMA_CONFIG, VID_ADAPTER_CATEGORY). Method bodies are boundaries.
// NOTE: the DB types drvInterface as vidDRIVER_INTERFACE*; d3d consumers reach the
// d3dDRIVER_INTERFACE derivative (which derives vidDRIVER_INTERFACE at offset 0) by cast.

struct vidDRIVER_vtbl;              // boundary -- driver vtable
struct vidCOMMAND_BUFFER_MANAGER;   // boundary -- command-buffer manager
struct txmTEXTURE;                  // txm subsystem texture
struct pctPICTURE;                  // pct (picture library) subsystem

// DB-verified (types_enum_values VID_ADAPTER_CATEGORY).
enum VID_ADAPTER_CATEGORY
{
    NV_DEPRECATED  = 0,
    NV_LOW         = 1,
    NV_MID         = 2,
    NV_MODERATE    = 3,
    NV_HIGH        = 4,
    ATI_DEPRECATED = 5,
    ATI_LOW        = 6,
    ATI_MID        = 7,
    ATI_MODERATE   = 8,
    ATI_HIGH       = 9,
    UNKNOWN_2_0    = 10,
    UNKNOWN_1_1    = 11,
    XENON          = 12,
    PS3            = 13,
    VID_ADAPTER_CATEGORY_LAST = 14,
};

// DB-verified layout (types_members vidRT_STACK_ENTRY, size 28).
typedef struct vidRT_STACK_ENTRY
{
    apSTATE_T<unsigned long> state[2];        /* 0x00 */
    txmTEXTURE              *pTexRendTarg[2]; /* 0x08 */
    txmTEXTURE              *pTexRendTargZS;  /* 0x10 */
    int                      sx;              /* 0x14 */
    int                      sy;              /* 0x18 */
} vidRT_STACK_ENTRY;

// DB-verified layout (types_members vidCLEAR, size 24).
typedef struct vidCLEAR
{
    m3dCOLOR color;      /* 0x00 */
    float    zRef;       /* 0x10 */
    int      stencilRef; /* 0x14 */
} vidCLEAR;

// DB-verified layout (types_members vidGAMMA_CONFIG, size 12).
typedef struct vidGAMMA_CONFIG
{
    float gamma;      /* 0x00 */
    float contrast;   /* 0x04 */
    float brightness; /* 0x08 */
} vidGAMMA_CONFIG;

typedef struct vidDRIVER
{
    vidDRIVER_vtbl                        *__vftable;                 /* 0x000 */
    float                                  brightnessOffset;          /* 0x004 */
    char                                   name[260];                 /* 0x008 */
    vidMODE                               *pMode;                     /* 0x10C (268) */
    void                                  *renderWnd;                 /* 0x110 */
    apSTATE_T<unsigned long>               state;                     /* 0x114 */
    int                                    bindProcessor;             /* 0x118 */
    int                                    acquiredThreadProcessor;   /* 0x11C */
    dsVECTOR<vidRT_STACK_ENTRY, 8>         rendTargStack;             /* 0x120 */
    int                                    suspendCounter;            /* 0x134 */
    vidRT_STACK_ENTRY                      rendTargCur;               /* 0x138 */
    float                                  rendTargCurParam[4];       /* 0x154 */
    vidDRIVER_INTERFACE                   *drvInterface;              /* 0x164 (356) */
    dsCONST_ARRAY<vidDRIVER_INTERFACE *, 2> drvComBufInterface;       /* 0x168 */
    vidCOMMAND_BUFFER_MANAGER             *comBufManager;             /* 0x174 */
    int                                    nPolyFrame;                /* 0x178 */
    int                                    nSplitFrame;               /* 0x17C */
    int                                    nPolyFrameCur;             /* 0x180 */
    int                                    nSplitFrameCur;            /* 0x184 */
    float                                  fps;                       /* 0x188 */
    vidCLEAR                               clearParams;               /* 0x18C */
    float                                  vsVerMax;                  /* 0x1A4 */
    float                                  psVerMax;                  /* 0x1A8 */
    unsigned int                           vidMemSize;                /* 0x1AC */
    int                                    maxAnisotropy;             /* 0x1B0 */
    VID_ADAPTER_CATEGORY                   category;                  /* 0x1B4 */
    int                                    modeSize;                  /* 0x1B8 */
    int                                    nModes;                    /* 0x1BC */
    vidMODE                               *modeList;                  /* 0x1C0 */
    vidCONFIG                              curConfig;                 /* 0x1C4 */
    vidPASS_PLATFORM                       sdrPlatform;               /* 0x288 (648) */
    unsigned char                          _pad28F[1];                /* 0x28F db-verified padding */
    vidDIP_STAT                            dipStat;                   /* 0x290 */
    float                                  stereo3DSeparation;        /* 0x30C */
    float                                  stereo3DConvergence;       /* 0x310 */
    int                                    isStereo3D;                /* 0x314 */
    vidGAMMA_CONFIG                        gammaCfg;                  /* 0x318 */
    int                                    isGammaChanged;            /* 0x324 */
    pctPICTURE                            *pQueuedScreenshot;         /* 0x328 */
    int                                    isQueuedScreenReady;       /* 0x32C */
    txmTEXTURE                            *pDepthStencilTexture;      /* 0x330 */
    txmTEXTURE                            *pFogMaskTexture;           /* 0x334 */
    txmTEXTURE                            *pDepthStencilHalfresTexture; /* 0x338 */

    // Per-frame draw counter incremented at end-of-scene. DB static (?frameNumber@vidDRIVER@@0HA).
    static int frameNumber;

    // Virtuals / helpers touched by the reversed BeginScene/EndScene below. ConfigureGamma
    // (vtbl 0xB8) applies the pending gamma config; SetViewport (vtbl 0x98) sets the D3D viewport;
    // ResetAllStreams unbinds all vertex streams. boundary (external to this batch).
    void ConfigureGamma();
    void SetViewport(int x, int y, int w, int h, float minZ, float maxZ);
    void ResetAllStreams();

    // 0x82667B10 -- reversed in vidDRIVER__BindToProcessor.cpp. Pins the render thread's processor
    // and binds the video lock's platform primitive to it.
    void BindToProcessor(int threadProcessorNmb);

    // Set the active scissor rectangle (screen-space clip). boundary (external to this batch).
    void SetScissorRect(int x, int y, int w, int h);

    // ReadBackBuffer is virtual in origin (DB: overridden as
    // ?ReadBackBuffer@d3dDRIVER@@MAAHPAUpctPICTURE@@PAVtxmTEXTURE@@@Z); called by
    // FlushScreenshotQueue. boundary (external).
    int ReadBackBuffer(pctPICTURE *pic, txmTEXTURE *backbuf);

    // 0x82669190 -- reversed in vidDRIVER__BeginScene.cpp. Virtual (UAA). Opens a render frame:
    // applies gamma, latches last frame's poly/split counters and resets the running ones, sets
    // the in-scene state bit 0x10000000.
    void BeginScene();

    // 0x82668EE0 / 0x82668F78 -- reversed in vidDRIVER__AcquireThreadOwnership.cpp /
    // vidDRIVER__ReleaseThreadOwnership.cpp. Virtual (UAA). Outermost vidLOCK Lock/Unlock hooks:
    // record/clear the owning processor and forward to drvInterface; no-op while suspended.
    void AcquireThreadOwnership();
    void ReleaseThreadOwnership();

    // 0x82667BA0 -- reversed in vidDRIVER__Suspend.cpp. Virtual (UAA). Increments suspendCounter.
    void Suspend();

    // 0x8266E418 -- reversed in vidDRIVER__EndScene.cpp. Virtual (UAA). Closes a render frame:
    // unbinds shaders/decl/streams, discards dynamic geometry, invalidates + clears the device
    // interface, restores the full-mode viewport, finishes DIP stats, clears the in-scene bit and
    // bumps frameNumber.
    void EndScene();

    // 0x82668C38 -- reversed in vidDRIVER__SetRenderTarget.cpp. Public wrapper: builds a
    // single-color-target vidRT_STACK_ENTRY (state[0]=stateDefUse) and applies it via the virtual
    // SetRenderTarget slot, then latches rendTargCur.sx/sy and the texel-size shader params.
    // 4th param is apSTATE_T<unsigned long> BY VALUE (DB mangling ?...V?$apSTATE_T@K@@@Z).
    int SetRenderTarget(txmTEXTURE *pTexRend, txmTEXTURE *pTexDepth,
                        apSTATE_T<unsigned long> stateDefUse);

    // 0x82668D70 -- reversed in vidDRIVER__UnsetRenderTarget.cpp. Public wrapper: builds an entry
    // (state[0]=3) naming the textures to unbind and applies it via the virtual UnsetRenderTarget.
    void UnsetRenderTarget(txmTEXTURE *pTexRend, txmTEXTURE *pTexDepth);

    // Virtual (vtable) render-target apply/unbind slots taking a fully-built stack entry. The
    // public wrappers above delegate to these; PopRenderTarget re-applies the saved top entry.
    // boundary (bodies external to this batch).
    int SetRenderTarget(const vidRT_STACK_ENTRY *entry);
    void UnsetRenderTarget(const vidRT_STACK_ENTRY *entry);

    // 0x8266BA08 -- reversed in vidDRIVER__PushRenderTarget.cpp. Push rendTargCur onto rendTargStack.
    void PushRenderTarget();

    // 0x8266B3A8 -- reversed in vidDRIVER__PopRenderTarget.cpp. Restore the top-of-stack render
    // target (or the default when empty) and pop it.
    int PopRenderTarget();

    // GPU hardware-timer virtuals — base-class no-op stubs (the d3dDRIVER override does the real
    // D3D timestamp-query work). Kept identical to the disassembly's constant returns.
    // 0x8266B9B8 — reversed in vidDRIVER__GpuTimeQueryGetTime.cpp. Base returns 0.
    unsigned __int64 GpuTimeQueryGetTime(VID_TQ query, int split_index);
    // 0x8266B9C0 — reversed in vidDRIVER__GpuTimeQueryGetFreq.cpp. Base returns 1 (avoid /0).
    unsigned __int64 GpuTimeQueryGetFreq();
    // 0x8266B9C8 — reversed in vidDRIVER__GpuTimeIdlePercent.cpp. Base returns 0.0f.
    float GpuTimeIdlePercent();

    // ?SwapBuffers@vidDRIVER@@UAAXH@Z — present the back buffer (virtual). boundary.
    void SwapBuffers(int needVSync);
    // ?QueueScreenshot@vidDRIVER@@QAAXPAUpctPICTURE@@@Z — queue a screenshot capture. boundary.
    void QueueScreenshot(pctPICTURE *pic);
    // ?FlushScreenshotQueue@vidDRIVER@@QAAXPAVtxmTEXTURE@@@Z — flush queued screenshot. boundary.
    void FlushScreenshotQueue(txmTEXTURE *backbuf);

    // Device hardware bring-up / teardown, chained by the d3dDRIVER overrides. Virtual in origin
    // (DB ?InitHW@vidDRIVER@@UAAHXZ / ?TermHW@vidDRIVER@@UAAXXZ) but declared non-virtual here to
    // preserve the DB manual-__vftable layout, matching this header's other virtuals. boundary (2026-07-31)
    int InitHW();
    void TermHW();

    // Base ctor tagging the video-driver category (56 = the d3d category); base dtor. Declared for
    // the explicit base-subobject ctor/dtor chains in d3dDRIVER. DB has only the int ctor
    // (??0vidDRIVER@@QAA@H@Z, no default ctor); the `= 0` default lets the base sub-object
    // default-construct under the base-as-member composition model. DB ??1vidDRIVER@@UAA@XZ. boundary (2026-07-31)
    vidDRIVER(int category = 0);
    ~vidDRIVER();
} vidDRIVER; /* 828 bytes */
