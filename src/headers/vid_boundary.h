#pragma once

/* ws-engine video-driver subsystem (vid*) boundary. Like txm*, this is a large
   self-contained class reached from hcex through a handful of method calls; it
   is treated as an external boundary, not decompiled. The apSTATE_T<unsigned long>
   render-state argument is passed as an opaque flag value (1 or 4 here). */

#include "txm_d3d_tex.h"   /* txmD3D_TEX */

typedef struct vidDRIVER vidDRIVER;

// os-layer thread-id query (real body in src/ws/os/osGetCurThreadId.cpp); declared here for the
// vidLOCK TUs since os_boundary.h dropped its stand-in.
int osGetCurThreadId();

extern vidDRIVER *vidDriver;
extern txmD3D_TEX *hcxe_global_render_targets_tex[];

extern void vidDRIVER__UnsetRenderTarget(vidDRIVER *self, txmD3D_TEX *tex, void *a3);
extern void vidDRIVER__PushRenderTarget(vidDRIVER *self);
extern void vidDRIVER__PopRenderTarget(vidDRIVER *self);
extern void vidDRIVER__SetRenderTarget(vidDRIVER *self, txmD3D_TEX *tex, void *a3, void *state_flags);

// ?AcquireThreadOwnership@vidDRIVER@@UAAXXZ / ?ReleaseThreadOwnership@vidDRIVER@@UAAXXZ --
// virtual thread-ownership hooks invoked by vidLOCK on the outermost Lock/Unlock.
extern void vidDRIVER__AcquireThreadOwnership(vidDRIVER *self);
extern void vidDRIVER__ReleaseThreadOwnership(vidDRIVER *self);

/* --- vidVBUF_MNG::FitFVF / DynGeomDraw (call-graph-audit gap-fill) ---------------------------
 * Both are declared boundary rather than reversed: FitFVF's decompiler output is a
 * pointer-tagged MRU-cache-plus-linear-scan-plus-hash-bucket vtable-pointer-swap over a
 * ~200-entry FVF descriptor table (types the fvfCacheLastUsed/fvfList arrays as raw vtable slots
 * via HIDWORD/LODWORD splits -- a strong signal the decompiler's type inference broke down on
 * vidVBUF_MNG's real layout, which is not otherwise modeled anywhere in this corpus), and
 * DynGeomDraw's call site packs 7 logical int/pointer arguments into what the decompiler shows
 * as fewer __int64 HIDWORD/LODWORD-split parameters (register-pair aliasing artifacts) --
 * reconstructing its true per-argument ABI would require deeper vidVBUF_MNG/rend_vis.cpp
 * disassembly work than this batch scopes. Both sit on top of the already-boundary
 * vidDRIVER_DYNGEOM_BUFFER (vidDRIVER_DYNGEOM_BUFFER.h) state machine. */

typedef struct vidVBUF_MNG vidVBUF_MNG;
typedef struct vidFVF_DESCR vidFVF_DESCR;
typedef int REND_PRIMTYPE;

extern vidVBUF_MNG *vidVBMng;

// vidVBUF_MNG_vtbl slot 0x30 -- discard all dynamic vertex-buffer allocations for the frame
// (called from vidDRIVER::EndScene). boundary (external to this batch).
extern void vidVBUF_MNG__DiscardDynAll(vidVBUF_MNG *self);

// 0x827B78A4 / 0x827B7B?? -- find-or-create the cached FVF descriptor for the given fvf
// bitmask(s); the 4-mask overload additionally distinguishes multistream/interleaved/instanced
// variants. boundary (external to this batch).
extern vidFVF_DESCR *vidVBUF_MNG_FitFVF(vidVBUF_MNG *self, unsigned long long fvf);
extern vidFVF_DESCR *vidVBUF_MNG__FitFVF4(vidVBUF_MNG *self, unsigned long long fvf_full,
        unsigned long long fvf_multistream, unsigned long long fvf_interleaved,
        unsigned long long fvf_instanced);

// 0x827C9028 -- one-shot immediate-mode draw through the dynamic-geometry buffer. NOW REVERSED:
// the body lives in src/ws/rend/DynGeomDraw.cpp and the authoritative declaration (with the
// disasm-correct argument order primType, vertList, nVert, indList, nInd, fvf, callerID) is in
// src/headers/ws/rend/rendDRIVER.h. The earlier boundary prototype here had nVert/indList and
// nInd/nVert transposed (register-pair aliasing artifact) and is removed to avoid an ODR clash;
// include rendDRIVER.h for the real signature.
