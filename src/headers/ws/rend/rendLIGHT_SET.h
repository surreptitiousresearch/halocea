#pragma once
#include "../ds/dsREF_COUNT.h"
#include "../ap/apSTATE_T.h"
#include "../os/osLOCK.h"
#include "../m3d/m3dV.h"
#include "rendSR_DATA.h"
#include "rendSTATIC_LIGHTING_STATE.h"
// rend (renderer) light-set object. Reversed here to the extent the ds smart-pointer,
// animINST::AddLightSet, and rendLIGHT_SET_LIST need: the vtable, the intrusive ref-count,
// the lock, the state word, the owning instance/object, and the frame-update bookkeeping.
// DB-verified fields (types_members rendLIGHT_SET): __vftable@0, dsREF_COUNT@4,
// distCur2Cam@8, srData@12(44), lock@56(52), state@108, pInstOwner@112, pObjOwner@116,
// frameUpdNmb@120, posUpdate@124(12), isOwnerInst@136, desiredStaticLightState@140(236),
// curStaticLightState@376(236), interpolation_time@612 — total size 616 (0x268).
// rendSTATIC_LIGHTING_STATE resolved this batch (types_members) — see rendSTATIC_LIGHTING_STATE.h.

struct rendLIGHT_SET;
struct objOBJ;    // objOBJ.h — owning model object (pointer only)          boundary
struct animINST;  // ../anim/animINST.h — owning animated instance (pointer only)  boundary

// DB-verified (types_members rendLIGHT_SET_vtbl): dtr_rendLIGHT_SET@0.
typedef struct rendLIGHT_SET_vtbl {
    void (*dtr_rendLIGHT_SET)(rendLIGHT_SET *self, int deleteFlag); // 0x00 (scalar-deleting dtr; deleteFlag bit0 => free)
} rendLIGHT_SET_vtbl;

typedef struct rendLIGHT_SET {
    static rendLIGHT_SET_vtbl vftable; // `rendLIGHT_SET::`vftable'' -- boundary, not decompiled

    rendLIGHT_SET_vtbl      *__vftable;   // 0x00
    dsREF_COUNT              ref;         // 0x04 (DB: unnamed embedded dsREF_COUNT)
    float                    distCur2Cam; // 0x08
    rendSR_DATA              srData;      // 0x0C (44 bytes) shader-replacement data
    osLOCK                   lock;        // 0x38 (56)
    apSTATE_T<unsigned long> state;       // 0x6C (108) light-set state flags
    animINST                *pInstOwner;  // 0x70 (112) owning animated instance
    objOBJ                  *pObjOwner;   // 0x74 (116) owning model object
    unsigned int              frameUpdNmb; // 0x78 (120) last-updated frame number
    m3dV                      posUpdate;   // 0x7C (124)
    bool                      isOwnerInst; // 0x88 (136) true while pInstOwner is the live owner
    unsigned char             _pad137[3];  // 0x89 alignment gap before the static-light states
    rendSTATIC_LIGHTING_STATE desiredStaticLightState; // 0x8C (140)
    rendSTATIC_LIGHTING_STATE curStaticLightState;     // 0x178 (376)
    float                     interpolation_time; // 0x264 (612)

    // 0x82ABDC70 — construct a light set owned by `_pInstOwner`/`_pObjOwner` (registers itself
    // in the global rendLightSetsList). Reversed this batch.
    rendLIGHT_SET(animINST *_pInstOwner, objOBJ *_pObjOwner);

    // 0x82ABBFA8 — clear the owning-instance back-pointer when `pInst` is the current owner
    // (called by animINST teardown so a destroyed instance never leaves a dangling owner).
    void NotifyDestroyInst(animINST *pInst);

    // 0x82ABBFF0 — set the owning instance back-reference; clears isOwnerInst when the new
    // owner is null (i.e. the light set is being detached from any instance).
    void SetInstOwner(animINST *pInst);

    // 0x82ABDE20 — recompute the baked static lighting at `pos` into `res`: gather the
    // candidate static lights (big lights, voxel-grid cell lights, and the special
    // "neg_tunnel_0" lights), evaluate per-light attenuation/visibility, then accumulate them
    // into the SH color and shader-directional-response ambient. Reversed this batch.
    void UpdateStatic(rendSTATIC_LIGHTING_STATE *res, m3dV *pos);

    // Used by rendCullAndValidateInst. Reversed in src/ws/rend/rendLIGHT_SET__*.cpp.
    // 0x82ABBDC8 — latch the given frame number; true when it differs from the last-latched one
    // (i.e. this light set has not yet been updated this frame).
    int NeedUpdate(unsigned int _frameUpdNmb);
    // 0x82ABEE10 — advance the light set toward its desired static-lighting state, sampling at
    // `pos` and lerping by an elapsed-time-scaled coefficient (no-op while active+bit3 are set).
    void Update(m3dV *pos, float elapsedTime);
    // DB overloads: Update(animINST*, float) @ ?Update@rendLIGHT_SET@@QAAXPAVanimINST@@M@Z and
    // Update(objOBJ*, float) — resolve the owner's position then delegate. boundary.
    void Update(animINST *pInst, float elapsedTime);
    void Update(objOBJ *pObj, float elapsedTime);

    // Internal helpers invoked by Update. Bodies live in the rend subsystem — boundary.
    void SetPosUpdate(m3dV *pos);                          // boundary (protected)
    void UpdateCurStaticLightState(float lerpCoeff);       // boundary (protected)
} rendLIGHT_SET;
