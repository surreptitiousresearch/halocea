#pragma once
#include <stdint.h>
#include "../ap/apSTATE_T.h"
#include "../m3d/m3dPLANE.h"
#include "../m3d/m3dMATR.h"
#include "../m3d/m4dV.h"
#include "rendCAM_LIST.h"
// ws-engine rend: the render driver singleton. Only the packed render-state word (`state`) and
// the running frame counter (`curFrameNmb`) are read/written by the HCEX bridge; the large
// embedded blocks (clip planes, projection matrix, camera list, poly-batch scratch) are opaque
// padding here — out of scope for this batch, left as boundary per the project's re-source
// conventions. DB-verified layout (types_members rendDRIVER): __vftable@0, clipPlane@16 (24B
// m3dPLANE), clipPlaneCCS@40 (24B m3dPLANE), matrProj@64 (64B m3dMATR), cameraOrg@128 (16B m4dV),
// depthScale@144 (float), stencilID@148 (int), stencilIDMask@152 (int), state@156
// (apSTATE_T<unsigned long>), curSmLightCount@160 (byte), curScenePtr@164 (scnSCENE*),
// camList@176 (47136B rendCAM_LIST), curCameraPtr@47312, elapsedTime@47316, curFrameNmb@47320,
// polyBatchFVF@47324 (8B), polyBatchVertOffset@47332, nPolyBatchVert@47336, nPolyBatchFace@47340,
// curRenderBlock@47344 (byte), curCamIdx@47348 — size 47352.

struct rendDRIVER_vtbl; // boundary — rend subsystem vtable
struct rendV3D;         // rendV3D.h — screen-space textured vertex (pointer only here)
struct m3dPOLY;         // ../m3d/m3dPOLY.h — convex polygon (nVert + m3dV[20])  pointer only here
struct txmTEXTURE;      // ../txm/txmTEXTURE.h — texture (pointer only here)  boundary

// rend immediate-mode primitive type passed to DynGeomDraw. boundary — full enum lives in the
// rend subsystem; only the two values used by DrawIndexedPrimitive/DrawPoly are named here.
enum REND_PRIMTYPE : int {
    REND_PRIMTYPE_TRILIST = 0,
    REND_PRIMTYPE_TRIFAN  = 1,
};

// vid base render-pass descriptor (carries PASS_DESC_BASE::BLENDMODE and friends). Full
// DB-verified layout in PASS_DESC_BASE.h; RenderPoly builds one on the stack.
#include "PASS_DESC_BASE.h"

struct scnSCENE;
namespace vidPASS_ENV { enum RENDERBLOCK : unsigned char; } // vid subsystem — 1-byte enum, boundary        // boundary — scene subsystem, only used by pointer here
struct camCAMERA;       // boundary — camera subsystem, only used by pointer here

typedef struct rendDRIVER {
    rendDRIVER_vtbl          *__vftable;      // 0x000
    unsigned char             _pad004[12];    // 0x004 gap before clipPlane
    m3dPLANE                  clipPlane;         // 0x010 (24B)
    m3dPLANE                  clipPlaneCCS;      // 0x028 (24B)
    m3dMATR                   matrProj;          // 0x040 (64B)
    m4dV                      cameraOrg;         // 0x080 (16B)
    float                     depthScale;        // 0x090
    int                       stencilID;         // 0x094
    int                       stencilIDMask;     // 0x098
    apSTATE_T<unsigned long>  state;             // 0x09C — bit 8 = ?, bit 18 = paused (read by hcexCallFrame)
    unsigned char             curSmLightCount;   // 0x0A0
    unsigned char             _pad0A1[3];        // 0x0A1
    scnSCENE                 *curScenePtr;       // 0x0A4
    unsigned char             _pad0A8[8];        // 0x0A8 gap before camList
    rendCAM_LIST              camList;           // 0x0B0 (47136B, see rendCAM_LIST.h)
    camCAMERA                *curCameraPtr;      // 0xB890
    float                     elapsedTime;       // 0xB894
    unsigned int              curFrameNmb;       // 0xB898 — incremented once per rendered frame
    uint64_t          polyBatchFVF;        // 0xB8DC (0xB89C rel-region) poly-batch scratch
    int                       polyBatchVertOffset; // 0xB8E4
    int                       nPolyBatchVert;      // 0xB8E8
    int                       nPolyBatchFace;      // 0xB8EC
    vidPASS_ENV::RENDERBLOCK  curRenderBlock;      // 0xB8F0 (1 byte, vid subsystem enum)
    unsigned char             _padB8F1[3];         // 0xB8F1 db-verified padding
    int                       curCamIdx;           // 0xB8F4
    unsigned char             _padB8F8[8];         // 0xB8F8 db-verified tail padding

    // rendDRIVER::RenderLine — draws a debug line between two world-space points, tinted `color`
    // (packed ulong). Body belongs to the rend subsystem — boundary (called by the ai08 debug
    // visualisers reversed this wave).
    void RenderLine(const struct m3dV *from, const struct m3dV *to, unsigned long color);

    // 0x836BD928-ish (rendDRIVER::RenderCross) — draws a debug "+" gizmo at world-space `pos`
    // with arm-length `size`, tinted `color` (0xAARRGGBB-style packed ulong). Body belongs to
    // the rend subsystem, a sibling of the HALO_SOUND_SYSTEM bridge — boundary in this
    // batch (only called by HALO_SOUND_SYSTEM::DbgRenderSounds here). boundary.
    void RenderCross(const struct m3dV *pos, float size, unsigned long color);

    // rendDRIVER::RenderDom — draw a debug outline of a scene domain (waypoint/volume region)
    // scaled by `scale`, tinted `color`. Body in the rend subsystem — boundary.
    void RenderDom(struct scnDOMAIN *dom, float scale, unsigned long color);

    // rendDRIVER::RenderSphere — draw a debug wire sphere (center+radius) tinted `color`. Body in
    // the rend subsystem — boundary (called by aiDEBUG::RenderHOGroup reversed this wave).
    void RenderSphere(const struct m3dSPHERE *sphere, unsigned long color);

    // rendDRIVER::RenderCS — draw a coordinate-system gizmo for the transform `matr` at axis length
    // `size`. Body in the rend subsystem — boundary (aiDEBUG::RenderEnemyGroup reversed this wave).
    void RenderCS(const struct m3dMATR *matr, float size);

    // rendDRIVER::RenderCyl — draw a debug wire cylinder (axis from->to, radius) tinted `color`.
    // Body in the rend subsystem — boundary (aiDEBUG::RenderCDTGroup reversed this wave).
    void RenderCyl(const struct m3dCYL *cyl, unsigned long color);

    // rendDRIVER::RenderOBB — draw a debug oriented bounding box tinted `color`. Body in the rend
    // subsystem — boundary (aiDEBUG::RenderCDTGroup reversed this wave).
    void RenderOBB(const struct m3dOBB *obb, unsigned long color);

    // 0x827C91D8 — draw an indexed triangle list. Tail-calls DynGeomDraw with
    // REND_PRIMTYPE_TRILIST and an index count of 3*nFace. `a6` is an opaque pass-through
    // (unused by this thunk); `callerID` is a FourCC debug tag. Reversed this batch.
    void DrawIndexedPrimitive(void *vertList, int nVert, unsigned short *indList,
                              int nFace, int a6, uint64_t fvf, unsigned int callerID);

    // 0x827C91E8 — draw an unindexed triangle fan (a single convex polygon). Resets the
    // transform (SetTransform(nullptr)) then DynGeomDraw with REND_PRIMTYPE_TRIFAN. Reversed
    // this batch.
    void DrawPoly(void *vertList, int nVert, int a4, uint64_t fvf);

    // 0x827CA218 — build a 4-vertex textured quad (two screen-space corners (x1,y1)-(x2,y2)
    // with texcoords (s1,t1)-(s2,t2) at depth z) and submit it via RenderPoly. Reversed this
    // batch.
    void RenderQuad(float x1, float y1, float s1, float t1,
                    float x2, float y2, float s2, float t2, float z,
                    PASS_DESC_BASE::BLENDMODE blendMode, unsigned int color, txmTEXTURE *texture);

    // 0x8273F0F0 — configure the driver for a render pass. Empty stub in this build (the body
    // is `;`), retained so the vtable/call sites resolve. Reversed this batch.
    void Configure(int vidPassID, const void *vidPassDesc, int a4, uint64_t fvf,
                   int space, unsigned char rend_block, int nDynLights, int nSMLights);

    // rend immediate-mode helper used by the draw thunks above. Body lives in the rend
    // subsystem — boundary.
    void SetTransform(const void *matr);                                                // boundary

    // 0x827C9D8C — draw a screen-space, color-modulated, optionally-textured convex polygon as a
    // triangle fan. Builds a PASS_DESC_BASE (unpacking `color` 0xAARRGGBB into colorConst,
    // no cull / no depth), configures the driver, clears the transform, and submits the vertex
    // list through DynGeomDraw. Reversed in src/ws/rend/rendDRIVER__RenderPoly_rendV3D.cpp.
    void RenderPoly(int nVert, rendV3D *vlist, PASS_DESC_BASE::BLENDMODE blendMode,
                    unsigned int color, txmTEXTURE *texture);

    // 0x827C9F40 — same as above, but triangulates an m3dPOLY (fan around vertex 0) into a
    // stack vertex buffer first. Reversed in src/ws/rend/rendDRIVER__RenderPoly_m3dPOLY.cpp.
    void RenderPoly(m3dPOLY *poly, PASS_DESC_BASE::BLENDMODE blendMode,
                    unsigned int color, txmTEXTURE *texture);

    // ?EndFrame@rendDRIVER@@UAAHXZ — end-of-frame render submission (virtual). boundary.
    int EndFrame();
} rendDRIVER;

// 0x827... rend immediate-mode geometry submission. boundary — rend subsystem.
extern "C" void DynGeomDraw(REND_PRIMTYPE primType, void *vertList, int nVert,
                            unsigned short *indList, int nInd, uint64_t fvf,
                            unsigned int callerID);
